// #include <GL/glew.h>
#include <QtWidgets>
#include <QtOpenGL>
#include <QVector3D>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
// #include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>



#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent)
    : QGLWidget(format, parent)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else
    , mVertexBufferObject(QGLBuffer::VertexBuffer)
#endif
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(1000/30);

    gTimer = new QTimer(this);
    connect(gTimer, SIGNAL(timeout()), this, SLOT(tick()));
    gTimer->start(500);


    /*    mModelMatrices[0].translate(-5,-10,0);
    mModelMatrices[1].translate(5,-10,0);
    mModelMatrices[1].rotate(90, QVector3D(0,0,1));
    mModelMatrices[2].translate(-5,10,0);
    mModelMatrices[2].rotate(270, QVector3D(0,0,1));
    mModelMatrices[3].translate(5,10,0);
    mModelMatrices[3].rotate(180, QVector3D(0,0,1));*/

    gameStart = false;

    static int wall_x = -6;
    static int wall_y = 9;
    static int wall_z = 0;

    alpha = 0;
    beta = 0;
    gama = 0;

    move_n = 0;

    currScale = 1.0f;

    for (int i = 0; i < 52; i++) {
      mModelMatrices[i].translate(wall_x,wall_y,wall_z);
      if(i<20){
        wall_y--;
      }
      else if((i>=20)&&(i<=30)){
        wall_x++;
      }
      else{
        wall_y++;
      }
    }



    rotateX = false;
    rotateY = false;
    rotateZ = false;

    keepRotate = false;

    move = false;

    // game control
    g = new Game(10,20);

    cyan = QColor(152 , 251 , 152) ;
    green = QColor(34 , 139 , 34 );
    blue = QColor(135 , 206, 235 );
    magenta = QColor(255 , 192 , 203);
    yellow = QColor(255 , 185 , 15);
    red = QColor(238, 99, 99 );
    black = QColor(0,0,0);
    c = QColor(205 , 150 , 205 );
    orange = QColor(255 , 165 , 0);

}

Viewer::~Viewer() {

}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 600);
}

void Viewer::initializeGL() {

    setMode(2);//default //TBD
  //  glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 1.0, 0.0);

    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

//    float triangleData[] = {
//        //  X     Y     Z
//         0.0f, 0.0f, 0.0f,
//         1.0f, 0.0f, 0.0f,
//         0.0f, 1.0f, 0.0f,
//    };
    

    float cube_vertices_tri[] = {

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,0.0f,
        0.0f, 1.0f,0.0f,  //top

        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,0.0f,
        0.0f, 1.0f, 1.0f,  //top

        1.0f,0.0f, 1.0f,
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,   //bottom

        1.0f,0.0f, 1.0f,
        0.0f,0.0f, 1.0f,
        0.0f,0.0f,0.0f,   //bottom

        0.0f, 1.0f, 1.0f,
        0.0f,0.0f, 1.0f,
        1.0f,0.0f, 1.0f,    //front

        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f,0.0f, 1.0f ,   //front

        1.0f, 1.0f,0.0f, // triangle 2 : begin
        0.0f,0.0f,0.0f,
        0.0f, 1.0f,0.0f, // triangle 2 : end     //back

        1.0f, 1.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,   //back

        0.0f,0.0f,0.0f, // triangle 1 : begin
        0.0f,0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, // triangle 1 : end    //left

        0.0f,0.0f,0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f,0.0f,  //left

        1.0f, 1.0f, 1.0f,
        1.0f,0.0f,0.0f,
        1.0f, 1.0f,0.0f,   //right

        1.0f,0.0f,0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,0.0f, 1.0f , //right

//for draw in lines
        0.0f,0.0f,0.0f, // triangle 1 : begin
        0.0f,0.0f, 1.0f,

        0.0f,0.0f,0.0f,
        0.0f, 1.0f,0.0f, // triangle 2 : end

        1.0f,0.0f,0.0f,
        1.0f, 1.0f,0.0f,

        1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,

        0.0f,1.0f, 1.0f,
        0.0f,1.0f,0.0f,

        0.0f, 1.0f, 1.0f,
        0.0f,0.0f, 1.0f,

        1.0f,0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 1.0f,0.0f,
        0.0f, 1.0f,0.0f,    // game control


        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f , 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 1.0f,
        0.0f , 0.0f, 1.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,1.0f

        };




#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else
    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of
     * QOpenGLBuffer.
     */
    uint vao;

    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);

    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;

    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mVertexBufferObject.allocate(cube_vertices_tri, 12 * 3 * 3 * sizeof(float)+12*2*3*sizeof(float));
    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);


    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");

    mColorLocation = mProgram.uniformLocation("frag_color");
}

void Viewer::paintGL() {

 //   faceMode();
    if(gameStart){
        for(int i=0;i<24;i++){
            for(int j=0;j<10;j++){
                game_map[i*10+j]=g->get(i,j);
            }
        }
    }


//    GLuint cube_indices[]= {
//				0, 1, 2, 2, 3, 0,
//				3, 2, 6, 6, 7, 3,
//				7, 6, 5, 5, 4, 7,
//				4, 0, 3, 3, 7, 4,
//				0, 1, 5, 5, 4, 0,
//				1, 5, 6, 6, 2, 1
//                };
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.bind();
#endif

    drawCubes();

    if(keepRotate){
        if(rotateX){
            rotateWorld(beta*1.0f,1.0f,0.0f,0.0f);
        }
        else if(rotateY){
            rotateWorld(alpha*1.0f,0.0f,1.0f,0.0f);
        }
        else if(rotateZ){
            rotateWorld(gama*1.0f,0.0f,0.0f,1.0f);
        }
    }

}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {

    std::cerr << "Stub: button " << event->button() << " pressed\n";
    lastPos = event->pos();

    if(event->buttons() == Qt::RightButton){
      std::cerr << "right button ";
    }
    else if(event->buttons() == Qt::LeftButton){
      std::cerr << "left button ";
    }
    else if(event->buttons() == Qt::MiddleButton){
      std::cerr << "middle button ";     
    }

    rotateX = false;
    rotateY = false;
    rotateZ = false;

    lastPos_move_1.setX(0);
    lastPos_move_0.setX(0);

    keepRotate = false;
  //  beta_ac = beta;
    std::cerr<<"rotateX:"<<rotateX<<"\n";
    
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    int dx2 = event->x()-lastPos_move_1.x();
    int dx = lastPos_move_1.x() - lastPos_move_0.x();
    if((dx<3)||(dx2<3)){
            keepRotate = true;
    }

    std::cerr << "dx:"<<dx<<"mouse release rotatex: " << rotateX<<"\n";
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;
    int dx = event->x() - lastPos.x();
    bool toScale=false;

    if(event->modifiers()&Qt::SHIFT){
        toScale = true;
        int dlarge = event->x() - lastPos.x();
        currScale = currScale*(1.0f+0.001*dlarge);
        if((currScale<1.3)&&(currScale>0.7)){
            scaleWorld(1.0f+0.001*dlarge,1.0f+0.001*dlarge,1.0f+0.001*dlarge);
        }
    }

    if(!toScale && event->buttons()& Qt::MiddleButton){
        alpha = dx;
        rotateWorld(alpha*1.0f,0.0f,1.0f,0.0f);  //rotateY
        rotateY = true;
    }

    else if(!toScale && event->buttons()&Qt::RightButton){
        gama = dx;
        rotateWorld(gama*1.0f,0.0f,0.0f,1.0f);   //rotateZ
        rotateZ = true;
    }


    else if(!toScale && event->buttons()&Qt::LeftButton){
        beta = dx;
        std::cerr<<"beta:"<<beta<<"\n";
        rotateWorld(beta*1.0f,1.0f,0.0f,0.0f);    //rotateX
        rotateX = true;
    }

    if(move_n==0){
        lastPos_move_0 = event->pos();
    }
    if(move_n==5){
        lastPos_move_1 = event->pos();
    }

    lastPos = event->pos();
    toScale = false;
    move_n++;
    if(move_n==6){
        move_n = 0;
    }
 //   start_time = QTime::currentTime();
   // std::cerr<<"last pressed at:"<<start_time.second()<<"\n";mModelMatrices
}

void Viewer::wheelEvent (QWheelEvent * event) {
    /*
     * int delta = event -> delta();
    std::cerr << "Stub: Motion at " << delta << " rotated \n ";
       if(event -> orientation() == Qt::Vertical){
      
      if(delta<0){mModelMatrices
	rotateWorld(0,1,0);
      } 
      else if(delta>0){
	rotateWorld(0,-1,0);
	}
	}*/
    
}

bool Viewer::keypress(QKeyEvent *keyevent){

    switch(keyevent->key()){
        case Qt::Key_Up:
            rotateCCW();
            break;
        case Qt::Key_Down:
            rotateCW();
            break;
        case Qt::Key_Left:
            moveLeft();
            break;
        case Qt::Key_Right:
            moveRight();
            break;
        case Qt::Key_Space:
            drop();
            break;

        case Qt::Key_N:
            newGame();
            break;
        case Qt::Key_R:
            resetView();
            break;
        case Qt::Key_Q:
            quit();
            break;

    }


}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(double alpha,float x, float y, float z) {
    mTransformMatrix.rotate(alpha,x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    mTransformMatrix.scale(x, y, z);
}

void Viewer::faceMode(int cube_index){
//    mProgram.setUniformValue(mColorLocation,red);
    QColor color[] = {cyan,green,blue,magenta,yellow,red,orange,c};
    for(int i=0;i<6;i++){
        drawFace(i,color[cube_index]);
    }
}

void Viewer::multiColorMode(){

    drawFace(0,yellow);
    drawFace(1,magenta);
    drawFace(2,red);
    drawFace(3,green);
    drawFace(4,blue);
    drawFace(5,cyan);
}

void Viewer::wireframeMode(){

    for(int i=0;i<52;i++){
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
        glDrawArrays(GL_LINES,36,12*2);
    }
    if(gameStart){
        for(int i = 0; i < 24; i++){
            for(int j = 0; j<10 ; j++){
                if(game_map[i*10+j]!=-1){
                    QMatrix4x4 matrix;
                    matrix.translate(-5+j,-10+i,0);
                    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * matrix);
                    glDrawArrays(GL_LINES,36,12*2);
                }
            }
        }
    }

}



void Viewer::tick(){
    g->tick();
}

void Viewer::drawFace(int faceIndex, QColor color){
    mProgram.setUniformValue(mColorLocation,color);
    glDrawArrays(GL_TRIANGLES,6*faceIndex,6);
}


void Viewer::drawCubes(){
    for (int i = 0; i < 52; i++) {
      mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
    //  glDrawArrays(GL_LINES, 0, 12*2);
//      glDrawArrays(GL_TRIANGLES, 0, 3*12);//cube_indices
//      multiColorMode();
      modeDraw(5);
    }

    if(gameStart){
        for(int i = 0; i < 24; i++){
            for(int j = 0; j<10 ; j++){
                if(game_map[i*10+j]!=-1){
                    QMatrix4x4 matrix;
                    matrix.translate(-5+j,-10+i,0);
                    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * matrix);
                    modeDraw(game_map[i*10+j]);
                  //  glDrawArrays(GL    //    mProgram.setUniformValue(mMvpMatrixLocatioglDrawArrays(GL_TRIANGLES, 0, 3*12);//cube_indicesn, getCameraMatrix() * mModelMatrices[0]);
                    //glDrawArrays(GL_TRIANGLES, 0, 3*12);//cube_indices
                    //glDrawElements(GL_TRIANGLES,3*12,GL_UNSIGNED_INT,cube_indices);_LINES, 0, 12*2);
    //                glDrawArrays(GL_TRIANGLES, 0, 3*12);//cube_indices
                }
            }

        }
    }

}

bool Viewer::moveLeft(){
    bool r = false;
    if(gameStart)
        r= g->moveLeft();

    return r;
}
// game control

bool Viewer::moveRight(){
    bool r = false;
    if(gameStart)
        r = g->moveRight();
    return r;
}

bool Viewer::drop(){
    bool r = false;
    if(gameStart)
        r = g->drop();
    return r;
}

bool Viewer::rotateCW(){    // game control

    bool r = false;
    if(gameStart)
        r = g->rotateCW();
    return r;
}

bool Viewer::rotateCCW(){

    bool r = false;
    if(gameStart)
        r = g->rotateCCW();
    return r;
}

void Viewer::modeDraw(int cube_index){
    int mode = getMode();
    if(mode==1){
        wireframeMode();
    }    // game control

    else if(mode==2){
        faceMode(cube_index);
    }
    else if(mode == 3){
        multiColorMode();
    }

}

int Viewer::getMode(){
    return mode;
}

void Viewer::setMode(int m){
    mode = m;
}

void Viewer::reset(){
    g->reset();
}

void Viewer::quit(){
    gameStart = false;
}

void Viewer::resetView(){
    std::cerr<<"reset\n";
    QMatrix4x4 tmpTransformMatrix;
    mTransformMatrix = tmpTransformMatrix;
}

void Viewer::newGame(){
    g->reset();
    gameStart = true;
}

void Viewer::setFaceMode(){
    setMode(2);
}

void Viewer::setWireFrameMode(){
    setMode(1);
}

void Viewer::setMultiColorMode(){
    setMode(3);
}

void Viewer::setSpeedInterval(int s){
//        gTimer = new QTimer(this);
//        connect(gTimer, SIGNAL(timeout()), this, SLOT(tick()));
        gTimer->start(s);
}


