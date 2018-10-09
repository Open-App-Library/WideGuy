#ifndef XORGCONFPARSER_H
#define XORGCONFPARSER_H

#include <QObject>
#include <QString>
#include <QDialog>
#include "ui_textareadialog.h"
#include "helper-io.hpp"

#define DEFAULT_XORG_CONF_LOCATION "/etc/X11/xorg.conf"

namespace Ui {
    class TextareaDialog;
};

class XOrgConfParser : public QObject
{
    Q_OBJECT
public:
    explicit XOrgConfParser(QString confLocation=DEFAULT_XORG_CONF_LOCATION, QObject *parent = nullptr);

    // Checks if Xorg configuration file exists. Returns false if it does not exist.
    bool configureSystem();

    void enableXinerama(bool enable, bool restartXorg=true);
    bool xineramaIsEnabled();

    static QString getDisplayManager();

    void setConfLocation(QString confLocation);
    void setXorgRestartCmd(QString restartCmd);

    QString confLocation();

    QString readConfig(bool reRead=false);

    void restoreBackup();
    void restartXorg();

    void setConfirmChanges(bool confirmChanges);
private:
    QDialog            *m_dialog;
    Ui::TextareaDialog m_textareaDialog;
    bool m_confirmChanges = false;
    QString m_confLocation;
    QString m_xorgRestartCmd;
    QString m_conf=""; // The text content of the config
};

#endif // XORGCONFPARSER_H
