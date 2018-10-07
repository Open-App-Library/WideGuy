#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    m_qsettings(new QSettings("OpenAppLibrary.org", "WideGuy"))
{
    ui->setupUi(this);

    ui->confirmChanges->setChecked( confirmChanges() );
    ui->useCustomLocation->setChecked( useCustomLocation() );
    ui->customLocation->setText( customLocation() );
}

bool Settings::confirmChanges()
{
    return getSetting("confirmChanges").toBool();
}

bool Settings::useCustomLocation()
{
    return getSetting("useCustomLocation").toBool();
}

QString Settings::customLocation()
{
    return getSetting("customLocation").toString();
}

QVariant Settings::getSetting(QString key)
{
    return m_qsettings->value(key);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_buttonBox_accepted()
{
    m_qsettings->setValue( "confirmChanges", ui->confirmChanges->isChecked() );
    m_qsettings->setValue( "useCustomLocation", ui->useCustomLocation->isChecked() );
    m_qsettings->setValue( "customLocation", ui->customLocation->text() );

    static_cast<MainWindow*>(this->parent())->updateSettings();
}

void Settings::on_buttonBox_rejected()
{

}

void Settings::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Reset)) {
        ui->confirmChanges->setChecked(false);
        ui->useCustomLocation->setChecked(false);
        ui->customLocation->setText("");
    }
}
