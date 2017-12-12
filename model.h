#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QVector>
#include <QVector4D>
#include <QQuaternion>
#include <QPicture>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

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
private:
    //GLuint VBO, VAO, EBO;
    QVector<QPicture> textures;
    QVector4D location;
    QQuaternion rotation;
};

#endif // MODEL_H
