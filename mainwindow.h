#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <QDebug>
#include "settings.h"
#include "xorgconfparser.h"
#include "ui_about.h"

namespace Ui {
class MainWindow;
class About;
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

    void setWideMode(bool enabled);

private slots:
    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

    void on_actionForce_WideMan_triggered();

    void on_actionForce_SkinnyMan_triggered();

    void on_actionRestore_Backup_triggered();

    void on_actionRestart_Xorg_triggered();

    void on_actionGeneral_Settings_triggered();

    void on_actionSubmitBug_triggered();

    void on_actionTutorial_triggered();

private:
    Ui::MainWindow *ui;
    Settings *m_settings;
    QDialog *m_about_dialog;
    Ui::About *m_about_window;
    XOrgConfParser *m_configParser;
    bool m_config_exists=false;
    bool m_wideMode=false;
    qint64 m_lastTimeToggled=0;

};

#endif // MAINWINDOW_H
