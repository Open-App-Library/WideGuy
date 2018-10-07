#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <QDebug>
#include "settings.h"
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

    void setMainButtonLabel(bool wideMode);
    void setMainButtonEnabled(bool enabled);
    void mainButtonClicked();
    void CheckSetupAndSetLabel();
    void updateSettings();

private slots:
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    Settings *m_settings;
    XOrgConfParser *m_configParser;
    bool m_config_exists=false;
    bool m_wideMode=false;
    qint64 m_lastTimeToggled=0;

};

#endif // MAINWINDOW_H
