#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <QTime>
#include <game.hpp>
#include <QKeyEvent>
#include <QColor>


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    Game* g;

    int game_map[240];


    // Called when the keyboard is pressed
    virtual bool keypress(QKeyEvent *keyevent);

    void reset();
    void quit();
    void resetView();
    void newGame();

    void setFaceMode();
    void setWireFrameMode();
    void setMultiColorMode();

    void setSpeedInterval(int speed_interval);



    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
  
protected:
    
    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
    // Called when the mouse wheels
    virtual void wheelEvent (QWheelEvent * event );

    QColor cyan ;
    QColor green;
    QColor blue ;
    QColor magenta ;
    QColor yellow ;
    QColor red ;
    QColor black ;
    QColor c;
    QColor orange;


    void faceMode(int cube_index);
    void wireframeMode();
    void multiColorMode();



    void drawCubes();
    void drawFace(int fi,QColor qc);

    void modeDraw(int cube_index);  //mode control...get the right mode according to mode setting

    bool moveLeft();
    bool moveRight();
    bool drop();
    bool rotateCW();
    bool rotateCCW();


private:
 
    int alpha;
    int beta;
    int gama;

    int mode;   //1 wire_frame   //2 face   //3 multicolor
    bool gameStart;

    int speed_interval;

    QPoint lastPos;

    QPoint lastPos_move_1;
    QPoint lastPos_move_0;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;

  //   QOpenGLBuffer mColorBufferObject;
#else 
    QGLBuffer mVertexBufferObject;

  //  QGLBUffer mColorBufferObject;
#endif

    int mVertexLocation;
    int mMvpMatrixLocation;

    int mColorLocation;
//    QTime start_time;
//    QTime end_time;


    bool rotateX;
    bool rotateY;
    bool rotateZ;

    bool keepRotate;

    bool move;

    int move_n;

    float currScale;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[52];
    QMatrix4x4 mMapMatrices[240];
    QMatrix4x4 mMapOriMatrices;
    QMatrix4x4 mTransformMatrix;
    QMatrix4x4 mIdentityMatrix;

    
    QTimer* mTimer;
    QGLShaderProgram mProgram;

    QTimer* gTimer;


    void setMode(int m);
    int getMode();

    QMatrix4x4 getCameraMatrix();
    void translateWorld( float x, float y, float z);
    void rotateWorld(double alpha,float x, float y, float z);
    void scaleWorld(float x, float y, float z);


private slots:
    void tick();
};

#endif
