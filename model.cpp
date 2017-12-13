#include "model.h"

#include <QFile>
#include <memory>

Model::Model(QString filename, QOpenGLFunctions *f, QOpenGLExtraFunctions *ef)
{

    VChunkHeader GeneralHeader;
    VChunkHeader PointsHeader;
    std::unique_ptr<VPoint[]> PointsData;
    VChunkHeader WedgesHeader;
    std::unique_ptr<VVertex[]> WedgesData;
    VChunkHeader FacesHeader;
    std::unique_ptr<VTriangle[]> FacesData;
    VChunkHeader MaterialsHeader;
    std::unique_ptr<VMaterial[]> MaterialsData;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return;


    file.read((char*)&GeneralHeader, sizeof(VChunkHeader));

    qDebug() << GeneralHeader.ChunkID << " " << GeneralHeader.DataCount << " " << GeneralHeader.DataSize << " " << GeneralHeader.TypeFlags;


    file.read((char*)&PointsHeader, sizeof(VChunkHeader));

    qDebug() << PointsHeader.ChunkID << " " << PointsHeader.DataCount << " " << PointsHeader.DataSize << " " << PointsHeader.TypeFlags;

    PointsData = std::unique_ptr<VPoint[]>(new VPoint[PointsHeader.DataCount]);

    for (int i = 0; i < PointsHeader.DataCount; i++)
    {
        file.read((char*)&PointsData[i], PointsHeader.DataSize);
        //qDebug() << PointsData[i].X << " " << PointsData[i].Y << " " << PointsData[i].Z;
    }


    file.read((char*)&WedgesHeader, sizeof(VChunkHeader));

    qDebug() << WedgesHeader.ChunkID << " " << WedgesHeader.DataCount << " " << WedgesHeader.DataSize << " " << WedgesHeader.TypeFlags;

    WedgesData = std::unique_ptr<VVertex[]>(new VVertex[WedgesHeader.DataCount]);

    for (int i = 0; i < WedgesHeader.DataCount; i++)
    {
        file.read((char*)&WedgesData[i], WedgesHeader.DataSize);
        //qDebug() << WedgesData[i].PointIndex << " " << WedgesData[i].U << " " << WedgesData[i].V;
    }

    file.read((char*)&FacesHeader, sizeof(VChunkHeader));

    qDebug() << FacesHeader.ChunkID << " " << FacesHeader.DataCount << " " << FacesHeader.DataSize << " " << FacesHeader.TypeFlags;

    FacesData = std::unique_ptr<VTriangle[]>(new VTriangle[FacesHeader.DataCount]);

    for (int i = 0; i < FacesHeader.DataCount; i++)
    {
        file.read((char*)&FacesData[i], FacesHeader.DataSize);
        //qDebug() << FacesData[i].WedgeIndex[0] << " " << FacesData[i].WedgeIndex[1] << " " << FacesData[i].WedgeIndex[2];
    }

    file.read((char*)&MaterialsHeader, sizeof(VChunkHeader));

    qDebug() << MaterialsHeader.ChunkID << " " << MaterialsHeader.DataCount << " " << MaterialsHeader.DataSize << " " << MaterialsHeader.TypeFlags;

    MaterialsData = std::unique_ptr<VMaterial[]>(new VMaterial[MaterialsHeader.DataCount]);

    for (int i = 0; i < MaterialsHeader.DataCount; i++)
    {
        file.read((char*)&MaterialsData[i], MaterialsHeader.DataSize);
        //qDebug() << MaterialsData[i].MaterialName << " " << MaterialsData[i].TextureIndex;
    }


    std::unique_ptr<GLfloat[]> vertices(new GLfloat[WedgesHeader.DataCount*5]);

    for(int i=0; i<WedgesHeader.DataCount; i++)
    {
        vertices[i*5] = PointsData[WedgesData[i].PointIndex].X;
        vertices[i*5+1] = PointsData[WedgesData[i].PointIndex].Y;
        vertices[i*5+2] = PointsData[WedgesData[i].PointIndex].Z;
        vertices[i*5+3] = WedgesData[i].U;
        vertices[i*5+4] = WedgesData[i].V;
    }

    std::unique_ptr<GLuint[]> indices(new GLuint[FacesHeader.DataCount*3]);

    for(int i=0; i<FacesHeader.DataCount; i++)
    {
        for(int j=0; j<3; j++)
        {
            indices[i*3+j] = FacesData[i].WedgeIndex[j];
        }
    }


    ef->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);
    ef->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.get())*WedgesHeader.DataCount*5, vertices.get(), GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.get())*FacesHeader.DataCount*3, indices.get(), GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(1);

    f->glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    ef->glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    file.close();

    this->setTexture(MaterialsData[0].MaterialName);
    this->VAOsize = WedgesHeader.DataCount;
}

GLuint Model::getVAO()
{
    return VAO;
}

int Model::getVAOsize()
{
    return this->VAOsize;
}

QOpenGLTexture* Model::getTexture()
{
    return this->texture.get();
}

void Model::setTexture(QString filename)
{
    QImage image = QImage(filename);
    if(!image.isNull())
    {
        this->texture = std::unique_ptr<QOpenGLTexture>(new QOpenGLTexture(image));
    }
    else
    {
        //TODO: Load default image from texture manager
        this->texture = std::unique_ptr<QOpenGLTexture>(new QOpenGLTexture(QImage("./../ProjectMaricaCarGenerator/test.jpg")));
    }
}
