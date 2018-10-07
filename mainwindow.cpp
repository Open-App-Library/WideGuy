#include <QMessageBox>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(new Settings(this)),
    m_configParser(new XOrgConfParser)
{
    ui->setupUi(this);

    updateSettings();

    connect(ui->mainButton, &QPushButton::clicked,
            this, &MainWindow::mainButtonClicked);
}

void MainWindow::mainButtonClicked()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastTimeToggled > 200) {
        m_lastTimeToggled = currentTime; // set the last-toggled time so we can make sure the user is not clicking too fast

        m_configParser->enableXinerama(!m_wideMode);
        setMainButtonLabel( m_configParser->xineramaIsEnabled() );

    } else {
        QMessageBox::warning(
            this,
            tr("Careful there..."),
            tr("You're pressing that button wayyyy too fast. Are you trying to kill your computer? If you're stuck in a mode, try using the 'Advanced' menu to select your desired mode.") );
    }
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
    if ( m_settings->useCustomLocation() ) {
        m_configParser->setConfLocation( m_settings->customLocation() );
        CheckSetupAndSetLabel();
    } else {
        m_configParser->setConfLocation( DEFAULT_XORG_CONF_LOCATION );
        CheckSetupAndSetLabel();
    }
}
