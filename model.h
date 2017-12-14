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
    ~Model();
    QOpenGLVertexArrayObject* getVAO() const;
    QOpenGLTexture* getTexture(int index) const;
    void setTexture(QString filename, int index);
    void addTexture(QString filename);
    int getTexturesSize();
    int getVAOsize();
    int getTextureQueue(int index);
    void setTextureQueue(int index, float average);
private:
    std::unique_ptr<QOpenGLVertexArrayObject> VAO;
    std::unique_ptr<QOpenGLBuffer> VBO, EBO;
    QVector4D location;
    QQuaternion rotation;
    std::vector<std::unique_ptr<QOpenGLTexture>> textures;
    std::vector<std::pair<int, float>> textureQueue;
    std::unique_ptr<QOpenGLBuffer> TBO;
    int VAOsize;
    void sortTextures();
    float averageAlpha(QImage image);
};

#endif // MODEL_H
