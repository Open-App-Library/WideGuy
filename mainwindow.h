#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mainButtonClicked();
private:
    Ui::MainWindow *ui;
    bool m_wideMode=false;
    long double m_lastTimeToggled=0;
};

#endif // MAINWINDOW_H
