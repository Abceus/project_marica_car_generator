#pragma once

#include <vector>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QString>
#include <QWheelEvent>
#include <QMatrix4x4>

#include "physicworld.h"
#include "scene.h"

class OpenglSimulationWidget :public QOpenGLWidget
{
Q_OBJECT
public:
    explicit OpenglSimulationWidget( QWidget *parent = nullptr );
    ~OpenglSimulationWidget() override = default;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    std::unique_ptr<QOpenGLShaderProgram> ShaderProgram;
    void rewriteThisShit( const QString &filename );
    std::unique_ptr<Mesh> makeModel( const QString &filename );
private:
    QMatrix4x4 projection;
    QMap<int, bool> keys;
    PhysicWorld physicWorld;
    std::shared_ptr<Scene> scene;
    std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
};
