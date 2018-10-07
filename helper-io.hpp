#ifndef HELPERIO_HPP
#define HELPERIO_HPP

#include <QString>
#include <QFile>

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
};

#endif // HELPERIO_HPP
