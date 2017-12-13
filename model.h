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

#include <GL/glu.h>
#include <GL/gl.h>

#include "unanimation.h"

class Model
{
public:
    Model(){}
    Model(QString filename, QOpenGLFunctions *f, QOpenGLExtraFunctions *ef);
    GLuint getVAO();
    GLuint VBO, VAO, EBO;
    QOpenGLTexture* getTexture();
    void setTexture(QString filename);
private:
    //GLuint VBO, VAO, EBO;
    //QVector<QOpenGLTexture> textures;
    QVector4D location;
    QQuaternion rotation;
    std::unique_ptr<QOpenGLTexture> texture;
};

#endif // MODEL_H
