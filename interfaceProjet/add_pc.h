#ifndef ADD_PC_H
#define ADD_PC_H

#include <QDialog>

namespace Ui {
class add_PC;
}

class add_PC : public QDialog
{
    Q_OBJECT

public:
    explicit add_PC(QWidget *parent = nullptr);
    ~add_PC();

private slots:
    void on_B_ajouter_accepted();

    void on_B_ajouter_rejected();

private:
    Ui::add_PC *ui;
};

#endif // ADD_PC_H
