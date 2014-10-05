#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();

public slots:
    void resetView();
    void newGame();

    void setFaceMode();
    void setWireFrameMode();
    void setMultiColorMode();

    void setHighSpeed();
    void setMediumSpeed();
    void setLowSpeed();

protected:
    void keyPressEvent(QKeyEvent *event);



private:
    void createActions();
    void createMenu();
    void createActionGroup();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_dr;
    QMenu* m_menu_sp;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*>m_menu_actions_dr;
    std::vector<QAction*> m_menu_actions_sp;

    Viewer* m_viewer;
};

#endif
