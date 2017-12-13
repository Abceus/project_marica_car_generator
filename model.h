#ifndef MODEL_H
#define MODEL_H

#include <memory>

#include <QString>
#include <QVector>
#include <QVector4D>
#include <QQuaternion>
#include <QPicture>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <GL/glu.h>
#include <GL/gl.h>

#include "unanimation.h"

class Model
{
public:
    Model(){}
    Model(QString filename, QOpenGLFunctions *f, QOpenGLExtraFunctions *ef);
    QOpenGLVertexArrayObject* getVAO() const;
    QOpenGLBuffer* getVBO() const;
    QOpenGLBuffer* getEBO() const;
    /*
    std::unique_ptr<QOpenGLVertexArrayObject> VAO;
    std::unique_ptr<QOpenGLBuffer> VBO, EBO;
*/
    QOpenGLTexture* getTexture() const;
    void setTexture(QString filename);
    int getVAOsize();
private:
    std::unique_ptr<QOpenGLVertexArrayObject> VAO;
    std::unique_ptr<QOpenGLBuffer> VBO, EBO;
    QVector4D location;
    QQuaternion rotation;
    std::unique_ptr<QOpenGLTexture> texture;
    int VAOsize;
};

#endif // MODEL_H
