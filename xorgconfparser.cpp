#include "xorgconfparser.h"

#include <QFile>
#include <iostream>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>

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
    for (int i = 0; i < lines.length(); i++) {
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
            QRegExp OptionXinerama("^[\\s\t]+Option[\\s\t]+\"Xinerama\"[\\s\t]+\"\\d\"");
            if (OptionXinerama.indexIn(curLine) >= 0) {
                line_is_good = false;
            }

        }
        if (line_is_good)
            newStr += curLine;
    }

//    m_textareaDialog.label->setText(m_confLocation);
//    m_textareaDialog.textarea->setText(newStr);
//    m_dialog->show();

    QFile file(m_confLocation);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << newStr;
    }

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
                qDebug() << "Wining line" << curLine;
                return true;
            }

        }
    }
    return false;
}
