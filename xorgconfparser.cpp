#include "xorgconfparser.h"

#include <QFile>
#include <iostream>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>
#include <QProcess>

XOrgConfParser::XOrgConfParser(QString confLocation, QObject *parent) :
    QObject(parent),
    m_confLocation(confLocation)
{
    m_dialog = new QDialog;
    m_textareaDialog.setupUi(m_dialog);
}

bool XOrgConfParser::configureSystem()
{
    QFile f(m_confLocation);
    if ( f.exists() ) {
        return true;
    }
    // TODO: Translations for not-found message
    QString distro_filename = ":/messages/xorg-conf-not-found.html";
    QString distro_html = QString(HelperIO::fileToQString(distro_filename)).arg(m_confLocation);
    QMessageBox box;
    box.setText(distro_html);
    box.exec();
    return false;
}

void XOrgConfParser::enableXinerama(bool enable)
{
    QString newStr;
    QStringList lines = readConfig().split("\n");
    bool inProperSection = false;
    int line_num = 0;
    for (int i = 0; i < lines.length(); i++) {
        line_num++;
        QString curLine = lines[i] + "\n";
        bool line_is_good = true;
        if (!inProperSection) {
            QRegExp SectionServerLayout("^Section \"ServerLayout\"");
            if (SectionServerLayout.indexIn(curLine) >= 0) {
                inProperSection = true;
            }
        } else {

            QRegExp EndSection("^EndSection");
            if (EndSection.indexIn(curLine) >= 0) {
                if(enable){
                    QString xinerama = "    Option         \"Xinerama\" \"1\"\n";
                    newStr += xinerama;
                } else {
                    QString xinerama = "    Option         \"Xinerama\" \"0\"\n";
                    newStr += xinerama;
                }
                inProperSection = false;
            }

            // The Money Code
            QRegExp OptionXinerama("^[\\s\t]+Option[\\s\t]+\"Xinerama\"[\\s\t]+\"[01]\"");
            if (OptionXinerama.indexIn(curLine) >= 0) {
                line_is_good = false;
            }

        }
        if (line_is_good)
            newStr += curLine;
    }

    bool safe_to_proceed = false;

    if ( m_confirmChanges ) {
        m_textareaDialog.label->setText("Confirm xorg.conf changes");
        m_textareaDialog.textarea->setText(newStr);
        m_dialog->exec();
        if (m_dialog->result() == 1) { // Ok button pressed
            safe_to_proceed = true;
        }
    } else {
        safe_to_proceed = true;
    }

    if (!safe_to_proceed) {
        qDebug("Cancelling operation.");
        return;
    }

    QString tmpFileLocation = "/tmp/xorg.conf.wideguy";
    QFile file(tmpFileLocation);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << newStr;
    } else {
        QMessageBox errorBox;
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setText(tr("No permissions to write to conf file: ") + tmpFileLocation);
        errorBox.exec();
    }
    file.close();


    QString scriptLocation = "/tmp/wideguy.sh";
    QFile script(scriptLocation);
    if (script.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&script);
        stream << "#!/bin/sh"                                           << "\n";
        stream << "tmpFileLocation=$1"                                  << "\n";
        stream << "m_confLocation=$2"                                   << "\n";
        stream << "if [ ! -f $m_confLocation.wideguy.backup ]; then"    << "\n";
        stream << "  cp $m_confLocation $m_confLocation.wideguy.backup" << "\n";
        stream << "fi"                                                  << "\n";
        stream << "mv $tmpFileLocation $m_confLocation"                 << "\n";
        stream << "if [ -f /usr/bin/lightdm ]; then"                    << "\n";
        stream << "  systemctl restart lightdm"                         << "\n";
        stream << "elif [ -f /usr/bin/sddm ]; then"                     << "\n";
        stream << "  systemctl restart sddm"                            << "\n";
        stream << "elif [ -f /usr/bin/gdm ]; then"                      << "\n";
        stream << "  systemct restart gdm"                              << "\n";
        stream << "fi"                                                  << "\n";
    } else {
        QMessageBox errorBox;
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setText(tr("No permissions to write script file: ") + scriptLocation);
        errorBox.exec();
    }
    script.close();

    QProcess p;
    QString cmd = QString("pkexec sh %1 %2 %3").arg(scriptLocation, tmpFileLocation, m_confLocation);
    qDebug() << cmd;
    p.start(cmd);
    p.waitForFinished();
    QString errors = p.readAllStandardError();
    QString msg    = p.readAllStandardOutput();
    if (errors.compare("") == -1) {
        qDebug() << "Error:" << errors;
    }
    if (msg.compare("") == -1) {
        qDebug() << msg;
    }
    p.close();
}

bool XOrgConfParser::xineramaIsEnabled()
{
    QStringList lines = readConfig().split("\n");
    bool inProperSection = false;
    for (int i = 0; i < lines.length(); i++) {
        QString curLine = lines[i] + "\n";
        if (!inProperSection) {
            QRegExp SectionServerLayout("^Section \"ServerLayout\"");
            if (SectionServerLayout.indexIn(curLine) >= 0) {
                inProperSection = true;
            }
        } else {
            QRegExp EndSection("^EndSection");
            if (EndSection.indexIn(curLine) >= 0) {
                inProperSection = false;
            }

            // The Money Code
            QRegExp OptionXinerama("^[\\s\t]+Option[\\s\t]+\"Xinerama\"[\\s\t]+\"1\"");
            if (OptionXinerama.indexIn(curLine) >= 0) {
                qDebug() << "Tis enabled" << curLine;
                return true;
            }

        }
    }
    return false;
}

void XOrgConfParser::setConfLocation(QString confLocation)
{
    m_confLocation = confLocation;
}

QString XOrgConfParser::confLocation()
{
    return m_confLocation;
}

QString XOrgConfParser::readConfig(bool reRead)
{
    if (m_conf.compare("") >= 0 || reRead) {
        QFile file(m_confLocation);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Xorg config" << m_confLocation << "does not exist";
            m_conf = "";
        } else if (!file.isReadable()) {
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText(tr("Don't have permissions to read conf file: ") + m_confLocation);
            errorBox.exec();
            m_conf = "";
        } else {
            m_conf = file.readAll();
        }
    }
    return m_conf;
}

void XOrgConfParser::setConfirmChanges(bool confirmChanges)
{
    m_confirmChanges = confirmChanges;
}
