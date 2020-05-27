#include "render_system/batching.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "errorsystem.h"


void DrawBuffer::clear()
{
    vertices.clear();
    indices.clear();
}

DrawBuffer operator+(DrawBuffer lhs, const DrawBuffer& rhs)
{
    auto lastVertex = lhs.vertices.size();
    lhs.vertices.append(rhs.vertices);
    auto lastIndex = lhs.indices.size();
    lhs.indices.append(rhs.indices);
    std::for_each(lhs.indices.begin() + lastIndex, lhs.indices.end(), [lastVertex](unsigned& element){ element += lastVertex; });
    return lhs;
}

void Batching::init()
{
    auto textureVertexShader = new QOpenGLShader( QOpenGLShader::Vertex );
    textureVertexShader->compileSourceFile( "resources/shaders/texturevertexshader.vert" );

    if( !textureVertexShader )
    {
        ErrorSystem::showError("The texture vertex shader wasn't compiled", true);
        return;
    }

    auto textureFragmentShader = new QOpenGLShader( QOpenGLShader::Fragment );
    textureFragmentShader->compileSourceFile( "resources/shaders/texturefragmentshader.frag" );

    if( !textureFragmentShader )
    {
        ErrorSystem::showError("The texture fragment shader wasn't compiled", true);
        return;
    }

    m_textureShader = QSharedPointer<QOpenGLShaderProgram>( new QOpenGLShaderProgram() );
    m_textureShader->addShader( textureVertexShader );
    m_textureShader->addShader( textureFragmentShader );
    if( !m_textureShader->link() )
    {
        ErrorSystem::showError( "The texture shader program wasn't linked", true );
        return;
    }

    auto lineVertexShader = new QOpenGLShader( QOpenGLShader::Vertex );
    lineVertexShader->compileSourceFile( "resources/shaders/texturevertexshader.vert" );

    if( !lineVertexShader )
    {
        ErrorSystem::showError("The line vertex shader wasn't compiled", true);
        return;
    }

    auto lineFragmentShader = new QOpenGLShader( QOpenGLShader::Fragment );
    lineFragmentShader->compileSourceFile( "resources/shaders/texturefragmentshader.frag" );

    if( !lineFragmentShader )
    {
        ErrorSystem::showError("The line fragment shader wasn't compiled", true);
        return;
    }

    m_lineShader = QSharedPointer<QOpenGLShaderProgram>( new QOpenGLShaderProgram() );
    m_lineShader->addShader( lineVertexShader );
    m_lineShader->addShader( lineFragmentShader );
    if( !m_lineShader->link() )
    {
        ErrorSystem::showError( "The line shader program wasn't linked", true );
        return;
    }
}

void Batching::addElements( const DrawBuffer& drawbuffer )
{

    if( drawbuffer.drawType == DrawType::DT_Mesh )
    {
        if( drawbuffer.material.blendType == BlendType::BT_None )
        {
            auto found = std::find( m_meshes.begin(), m_meshes.end(), drawbuffer );
            if(found != m_meshes.end())
            {
                *found = *found + drawbuffer;
            }
            else
            {
                m_meshes.append( drawbuffer );
            }
        }
        else if( drawbuffer.material.blendType == BlendType::BT_Alpha )
        {
            auto found = std::find( m_transparentMeshes.begin(), m_transparentMeshes.end(), drawbuffer );
            if(found != m_transparentMeshes.end())
            {
                *found = *found + drawbuffer;
            }
            else
            {
                m_transparentMeshes.append( drawbuffer );
            }
        }
    }
    else if( drawbuffer.drawType == DrawType::DT_Overlay )
    {
        m_overlays = m_overlays + drawbuffer;
    }
}

void Batching::clear()
{
    m_meshes.clear();
    m_transparentMeshes.clear();
    m_overlays.clear();
}

void Batching::draw()
{
    auto f = QOpenGLContext::currentContext()->functions();
    for(const auto& mesh: m_meshes)
    {
        if( mesh.elementType == ElementsType::PT_Vertex )
        {
            m_textureShader->bind();
            m_textureShader->setUniformValue( "texture", 0 );
            m_textureShader->setUniformValue( "color", mesh.material.color );
        }
        else if( mesh.elementType == ElementsType::PT_Line )
        {
            m_lineShader->bind();
            m_lineShader->setUniformValue( "color", mesh.material.color );
        }
        QOpenGLVertexArrayObject VAO;
        QOpenGLBuffer VBO, EBO;

        VAO.create();
        VBO = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
        VBO.create();
        VBO.setUsagePattern( QOpenGLBuffer::StaticDraw );
        EBO = QOpenGLBuffer( QOpenGLBuffer::IndexBuffer );
        EBO.create();
        EBO.setUsagePattern( QOpenGLBuffer::StaticDraw );

        VAO.bind();

        VBO.bind();
        VBO.allocate( mesh.vertices.data(), sizeof( DrawBuffer::Vertex )*mesh.vertices.size() );

        EBO.bind();
        EBO.allocate( mesh.indices.data(), sizeof( unsigned )*mesh.indices.size() );

        f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( DrawBuffer::Vertex ), nullptr );
        f->glEnableVertexAttribArray( 0 );

        if( mesh.elementType == ElementsType::PT_Vertex )
        {
            f->glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( DrawBuffer::Vertex ),
                    reinterpret_cast<GLvoid*>( offsetof( DrawBuffer::Vertex, textureCoords ) ) );
            f->glEnableVertexAttribArray( 1 );

            mesh.material.texture->bind();
            f->glDrawElements( GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr );
        }
        else
        {
            f->glDrawElements( GL_LINE, mesh.indices.size(), GL_UNSIGNED_INT, nullptr );
        }
        VAO.release();

        VAO.destroy();
        VBO.destroy();
        EBO.destroy();

        if( mesh.elementType == ElementsType::PT_Vertex )
        {
            m_textureShader->release();
        }
        else if( mesh.elementType == ElementsType::PT_Line )
        {
            m_lineShader->release();
        }
    }
}
