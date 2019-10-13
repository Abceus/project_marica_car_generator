#include <QObject>
#include <QPoint>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Grid : public QObject
{
    Q_OBJECT
public:
    Grid( const QPoint& squadSize, const QPoint& gridSize );
private:
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO, EBO;
};
