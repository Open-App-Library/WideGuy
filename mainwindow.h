#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <QDebug>
#include "xorgconfparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setWideMode(bool wideMode);

    void mainButtonClicked();
private:
    Ui::MainWindow *ui;
    bool m_wideMode=false;
    qint64 m_lastTimeToggled=0;
    XOrgConfParser *m_configParser;

};

#endif // MAINWINDOW_H
