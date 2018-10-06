#ifndef XORGCONFPARSER_H
#define XORGCONFPARSER_H

#include <QObject>
#include <QString>
#include <QDialog>
#include "ui_textareadialog.h"

namespace Ui {
    class TextareaDialog;
};

class XOrgConfParser : public QObject
{
    Q_OBJECT
public:
    explicit XOrgConfParser(QString confLocation="/etc/X11/xorg.conf", QObject *parent = nullptr);
    void enableXinerama(bool enable);
    QString readConfig(bool reRead=false);
    bool xineramaIsEnabled();
private:
    QDialog            *m_dialog;
    Ui::TextareaDialog m_textareaDialog;
    QString m_confLocation;
    QString m_conf=""; // The text content of the config
};

#endif // XORGCONFPARSER_H
