#include <QtWidgets>
#include <QGLFormat>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Tetrominoes on the Wall");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    m_viewer->show();

    createActions();
    createMenu();
}


 void AppWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_T) {
        std::cerr << "Hello!" << std::endl;
    } else {
        //QWidget::keyPressEvent(event);
        m_viewer->keypress(event);
    }
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* newGameAct = new QAction(tr("&New Game"),this);
    QAction* resetAct = new QAction(tr("&Reset"),this);
    QAction* quitAct = new QAction(tr("&Quit"), this);

  //  QActionGroup *menuActionGroup = new QActionGroup(this);

    m_menu_actions.push_back(newGameAct);
    m_menu_actions.push_back(resetAct);
    m_menu_actions.push_back(quitAct);

 //  menuActionGroup->setExclusive(true);
  //  menuActionGroup->addAction(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 

    newGameAct->setShortcut(QKeySequence(Qt::Key_N));
    resetAct->setShortcut(QKeySequence(Qt::Key_R));
    quitAct->setShortcut(QKeySequence::Quit);


    // Set the tip

    newGameAct->setStatusTip(tr("Starts a new game"));
    resetAct->setStatusTip(tr("Reset the display"));
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(newGameAct,SIGNAL(triggered()),this,SLOT(newGame()));
    connect(resetAct,SIGNAL(triggered()),this,SLOT(resetView()));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    //=======================

    QAction* wfAct = new QAction(tr("&Wire-frame mode"),this);
    QAction* fAct = new QAction(tr("&Face mode"),this);
    QAction* cAct = new QAction(tr("&Multicoloured mode"), this);

  //  QActionGroup *menuActionGroup = new QActionGroup(this);

    m_menu_actions_dr.push_back(wfAct);
    m_menu_actions_dr.push_back(fAct);
    m_menu_actions_dr.push_back(cAct);

 //  menuActionGroup->setExclusive(true);
  //  menuActionGroup->addAction(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P);

    wfAct->setShortcut(QKeySequence(Qt::Key_W));
    fAct->setShortcut(QKeySequence(Qt::Key_F));
    cAct->setShortcut(QKeySequence(Qt::Key_M));


    // Set the tip

    wfAct->setStatusTip(tr("Wire-frame mode"));
    fAct->setStatusTip(tr("Face mode"));
    cAct->setStatusTip(tr("Multicoloured mode"));

    // Connect the action with the signal and slot designated
    connect(wfAct,SIGNAL(triggered()),this,SLOT(setWireFrameMode()));
    connect(fAct,SIGNAL(triggered()),this,SLOT(setFaceMode()));
    connect(cAct, SIGNAL(triggered()), this, SLOT(setMultiColorMode()));
    //========================

    QAction* hsAct = new QAction(tr("&Fast"),this);
    QAction* msAct = new QAction(tr("&Medium"),this);
    QAction* lsAct = new QAction(tr("&Slow"), this);

    m_menu_actions_sp.push_back(lsAct);
    m_menu_actions_sp.push_back(msAct);
    m_menu_actions_sp.push_back(hsAct);


//    // We set the accelerator keys
//    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P);

//    newGameAct->setShortcut(QKeySequence(Qt::Key_N));
//    resetAct->setShortcut(QKeySequence(Qt::Key_R));
//    quitAct->setShortcuts(QKeySequence::Quit);


//    // Set the tip

    hsAct->setStatusTip(tr("Fast mode"));
    msAct->setStatusTip(tr("Medium mode"));
    lsAct->setStatusTip(tr("Low mode"));

//    // Connect the action with the signal and slot designated
    connect(hsAct,SIGNAL(triggered()),this,SLOT(setHighSpeed()));
    connect(msAct,SIGNAL(triggered()),this,SLOT(setMediumSpeed()));
    connect(lsAct, SIGNAL(triggered()), this, SLOT(setLowSpeed()));

}

//void AppWindow::createActionGroup(){
//    // Setup menu bar.
//   //////////////////
//   QAction *menuAction;
//   QActionGroup *menuActionGroup = new QActionGroup(this);
//   QMenuBar *menuBar = new QMenuBar(this);

//   QMenu *menu = menuBar->addMenu("File");
//   menuAction = menu->addAction("Load");
//   connect(menuAction, SIGNAL(triggered()), this, SLOT(Load_Configuration()));
//   menuAction = menu->addAction("Save");
//   connect(menuAction, SIGNAL(triggered()), this, SLOT(Save_Configuration()));

//   menuActionGroup->setExclusive(true);
//   menu = menuBar->addMenu("ComPort");
//   menuAction = menu->addAction("No Port");
//   menuAction->setChecked(true);
//   menuAction->setCheckable(true);
//   menuActionGroup->addAction(menuAction);
//   connect(menuAction, SIGNAL(triggered()), this, SLOT(Select_ComPort0()));
//   menuAction = menu->addAction("Com 1");
//   menuAction->setCheckable(true);
//   menuActionGroup->addAction(menuAction);
//   connect(menuAction, SIGNAL(triggered()), this, SLOT(Select_ComPort1()));
//   menuAction = menu->addAction("Com 2");
//   menuAction->setCheckable(true);
//   menuActionGroup->addAction(menuAction);
//   connect(menuAction, SIGNAL(triggered()), this, SLOT(Select_ComPort2()));
//}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_dr = menuBar()->addMenu(tr("&Draw Mode"));
    m_menu_sp = menuBar()->addMenu(tr("&Speed"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    for (auto& action : m_menu_actions_dr) {
        m_menu_dr->addAction(action);
    }

    for (auto& action : m_menu_actions_sp) {
        m_menu_sp->addAction(action);
    }


}

void AppWindow::resetView(){
    m_viewer->resetView();
}

void AppWindow::newGame(){
    m_viewer->newGame();
}

void AppWindow::setFaceMode(){
    m_viewer->setFaceMode();
}

void AppWindow::setWireFrameMode(){
    m_viewer->setWireFrameMode();
}

void AppWindow::setMultiColorMode(){
    m_viewer->setMultiColorMode();

}

void AppWindow::setHighSpeed(){
    m_viewer->setSpeedInterval(350);
}

void AppWindow::setMediumSpeed(){
    m_viewer->setSpeedInterval(500);
}

void AppWindow::setLowSpeed(){
    m_viewer->setSpeedInterval(750);
}
