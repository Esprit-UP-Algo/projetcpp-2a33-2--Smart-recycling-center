#ifndef activitydialog_H
#define activitydialog_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class activitydialog;
}

class activitydialog : public QDialog
{
    Q_OBJECT

public:
    explicit activitydialog(QWidget *parent = nullptr, QDate data= QDate(), std::vector<attivita>* v = nullptr, bool edit = false, int pos = -1);
    ~activitydialog();
    attivita edited_activity;

private slots:


    void on_inizio_userTimeChanged(const QTime &time);

    void on_NoEnd_clicked(bool checked);

    void on_ripeti_currentIndexChanged(int index);

    void on_fine_dateTimeChanged(const QDateTime &dateTime);

    void on_tipo_currentIndexChanged(int index);

    void on_tipo_activated();

    void on_buttonBox_accepted();

private:
    Ui::activitydialog *ui;
    std::vector<attivita> *punt;
    QDate data;
    QDate data_fine;
    int pos;
    bool edit;
    void setAttivita();
    bool checkFree();
    void finishCheck();

};

#endif // activitydialog_H
