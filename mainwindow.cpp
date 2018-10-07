#include <QMessageBox>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(new Settings(this)),
    m_about_window(new Ui::About),
    m_configParser(new XOrgConfParser)
{
    ui->setupUi(this);
    m_about_dialog = new QDialog(this);
    m_about_window->setupUi(m_about_dialog);

    updateSettings();

    connect(ui->mainButton, &QPushButton::clicked,
            this, &MainWindow::mainButtonClicked);
}

void MainWindow::mainButtonClicked()
{
    setWideMode( !m_wideMode );
}

void MainWindow::CheckSetupAndSetLabel()
{
    m_config_exists = m_configParser->configureSystem();
    if (m_config_exists)
        setMainButtonLabel( m_configParser->xineramaIsEnabled() );
    else
        setMainButtonEnabled( false );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_configParser;
    delete m_settings;
    delete m_about_dialog;
    delete m_about_window;
}

void MainWindow::setMainButtonLabel(bool wideMode)
{
    m_wideMode = wideMode;
    if ( !m_config_exists ) {
        setMainButtonEnabled( m_config_exists );
        return;
    }
    if ( m_wideMode ) {
        ui->mainButton->setText(tr("Disable Wide Mode"));
    } else {
        ui->mainButton->setText(tr("Enable Wide Mode"));
    }

}

void MainWindow::setMainButtonEnabled(bool enabled)
{
    ui->mainButton->setEnabled(enabled);
    if (enabled)
        setMainButtonLabel( m_configParser->xineramaIsEnabled() );
    else
        ui->mainButton->setText(tr("Button disabled until\nxorg.conf is successfully\nloaded."));
}

void MainWindow::on_actionSettings_triggered()
{
    m_settings->show();
}

void MainWindow::updateSettings()
{
    m_configParser->setConfirmChanges( m_settings->confirmChanges() );
    m_configParser->setConfLocation( m_settings->customLocation() );
    m_configParser->setXorgRestartCmd( m_settings->customRestartXorgCmd() );

    qDebug() << "Settings:";
    qDebug() << "Confirm Xorg changes =" << m_settings->confirmChanges();
    qDebug() << "Xorg config location =" << m_settings->customLocation();
    qDebug() << "Xorg restart cmd     =" << m_settings->customRestartXorgCmd();

    CheckSetupAndSetLabel();
}

void MainWindow::setWideMode(bool enabled)
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastTimeToggled > 200) {
        m_lastTimeToggled = currentTime; // set the last-toggled time so we can make sure the user is not clicking too fast

        m_configParser->enableXinerama( enabled );
        setMainButtonLabel( m_configParser->xineramaIsEnabled() );

    } else {
        QMessageBox::warning(
            this,
            tr("Careful there..."),
            tr("You're pressing that button wayyyy too fast. Are you trying to kill your computer? If you're stuck in a mode, try using the 'Advanced' menu to select your desired mode.") );
    }
}

void MainWindow::on_actionAbout_triggered()
{
    m_about_dialog->show();
}

void MainWindow::on_actionForce_WideMan_triggered()
{
    setWideMode( true );
}

void MainWindow::on_actionForce_SkinnyMan_triggered()
{
    setWideMode( false );
}

void MainWindow::on_actionRestore_Backup_triggered()
{
    m_configParser->restoreBackup();
    updateSettings();
}

void MainWindow::on_actionRestart_Xorg_triggered()
{
    m_configParser->restartXorg();
}
