#ifndef HELPERIO_HPP
#define HELPERIO_HPP

#include <QString>
#include <QFile>
#include <QProcess>
#include <QDebug>

class HelperIO {
public:
    static QString fileToQString(QString filename)
    {
        QString val;
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        return val;
    }

    static void shellCommand(QString cmd) {
        QProcess p;
        qDebug() << cmd;
        p.start(cmd);
        p.waitForFinished();
        QString errors = p.readAllStandardError();
        QString msgs    = p.readAllStandardOutput();
        if (errors.compare("") == -1) {
            qDebug() << "Error:" << errors;
        }
        if (msgs.compare("") == -1) {
            qDebug() << msgs;
        }
        p.close();
    }
};

#endif // HELPERIO_HPP
