#include <QMessageBox>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_configParser(new XOrgConfParser("/home/doug/xorg.conf"))
{
    ui->setupUi(this);

    setWideMode( m_configParser->xineramaIsEnabled() );
    qDebug() << "The mode is" << m_wideMode;

    connect(ui->mainButton, &QPushButton::clicked,
            this, &MainWindow::mainButtonClicked);
}

void MainWindow::mainButtonClicked()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastTimeToggled > 200) {
        m_lastTimeToggled = currentTime; // set the last-toggled time so we can make sure the user is not clicking too fast
        m_wideMode = !m_wideMode;        // Toggle m_wideMode variable

        // Adjust the label
        setWideMode(m_wideMode);

        m_configParser->enableXinerama(m_wideMode);

    } else {
        QMessageBox::warning(
            this,
            tr("Careful there..."),
            tr("You're pressing that button wayyyy too fast. Are you trying to kill your computer? If you're stuck in a mode, try using the 'Advanced' menu to select your desired mode.") );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_configParser;
}

void MainWindow::setWideMode(bool wideMode)
{
    m_wideMode = wideMode;
    if (m_wideMode) {
        ui->mainButton->setText(tr("Disable Wide Mode"));
    } else {
        ui->mainButton->setText(tr("Enable Wide Mode"));
    }

}
