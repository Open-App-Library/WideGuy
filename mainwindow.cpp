#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->mainButton, &QPushButton::clicked,
            this, &MainWindow::mainButtonClicked);
}

void MainWindow::mainButtonClicked()
{

    long double currentTime = time(nullptr)*1000;
    if (currentTime - m_lastTimeToggled > 100) {
        m_lastTimeToggled = currentTime; // set the last-toggled time so we can make sure the user is not clicking too fast
        m_wideMode = !m_wideMode;        // Toggle m_wideMode variable

        // Adjust the label
        if (m_wideMode) {
            ui->mainButton->setText(tr("Disable Wide Mode"));
        } else {
            ui->mainButton->setText(tr("Enable Wide Mode"));
        }
    } else {
        QMessageBox::information(
            this,
            tr("Careful there..."),
            tr("You're pressing that button wayyyy too fast. Are you trying to kill your computer? If you're stuck in a mode, try using the 'Advanced' menu to select your desired mode.") );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
