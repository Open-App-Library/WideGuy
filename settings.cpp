#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include "xorgconfparser.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    m_qsettings(new QSettings("OpenAppLibrary.org", "WideGuy"))
{
    ui->setupUi(this);

    ui->confirmChanges->setChecked( confirmChanges() );
    ui->customLocation->setText( customLocation() );

    initSettings();
}

bool Settings::confirmChanges()
{
    return getSetting("confirmChanges").toBool();
}

QString Settings::customLocation()
{
    return getSetting("customLocation").toString();
}

QString Settings::customRestartXorgCmd()
{
    return getSetting("customRestartXorgCmd").toString();
}

void Settings::initSettings(bool forceReset)
{
    if ( !m_qsettings->contains("confirmChanges") || forceReset ) {
        ui->confirmChanges->setChecked(false);
        m_qsettings->setValue( "confirmChanges", false);
    }

    if ( !m_qsettings->contains("customLocation") || forceReset ) {
        ui->customLocation->setText( DEFAULT_XORG_CONF_LOCATION);
        m_qsettings->setValue("customLocation", DEFAULT_XORG_CONF_LOCATION);
    }

    if ( !m_qsettings->contains("customRestartXorgCmd") || forceReset ) {
        ui->customRestartXorgCmd->setText("systemctl restart " + XOrgConfParser::getDisplayManager());
        m_qsettings->setValue("customRestartXorgCmd", "systemctl restart " + XOrgConfParser::getDisplayManager());
    }
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
    m_qsettings->setValue( "customLocation", ui->customLocation->text() );
    m_qsettings->setValue( "customRestartXorgCmd", ui->customRestartXorgCmd->text() );

    static_cast<MainWindow*>(this->parent())->updateSettings();
}

void Settings::on_buttonBox_rejected()
{

}

void Settings::on_buttonBox_clicked(QAbstractButton *button)
{
    // Reset button
    if ( reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Reset)) {
        initSettings(true);
    }
}
