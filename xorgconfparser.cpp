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
                    newStr += "    Option         \"Xinerama\" \"1\"\n";
                } else {
                    newStr += "    Option         \"Xinerama\" \"0\"\n";
                }
                inProperSection = false;
            }

            // The Money Code
            QRegExp OptionXinerama("^[\\s\t]+Option[\\s\t]+\"Xinerama\"[\\s\t]+\"[01]\"");
            if (OptionXinerama.indexIn(curLine) >= 0) {
                qDebug() << "line #" << line_num << curLine;
                line_is_good = false;
            }

        }
        if (line_is_good)
            newStr += curLine;
    }

//    m_textareaDialog.label->setText(m_confLocation);
//    m_textareaDialog.textarea->setText(newStr);
//    m_dialog->show();

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

QString XOrgConfParser::readConfig(bool reRead)
{
    if (m_conf.compare("") >= 0 || reRead) {
        QFile file(m_confLocation);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText(tr("Unable to open conf file: ") + m_confLocation);
            errorBox.exec();
            m_conf = "";
        }
        if (!file.isReadable()) {
            QMessageBox errorBox;
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText(tr("Don't have permissions to read conf file: ") + m_confLocation);
            errorBox.exec();
            m_conf = "";
        }
        m_conf = file.readAll();
    }
    return m_conf;
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
                return true;
            }

        }
    }
    return false;
}
