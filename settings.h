#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

    bool confirmChanges();
    bool useCustomLocation();
    QString customLocation();

    ~Settings();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Settings *ui;
    QSettings *m_qsettings;
    QVariant getSetting(QString key);
};

#endif // DIALOG_H
