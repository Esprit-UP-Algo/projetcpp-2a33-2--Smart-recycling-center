#ifndef MODIFIER_PC_H
#define MODIFIER_PC_H

#include <QDialog>

namespace Ui {
class modifier_PC;
}

class modifier_PC : public QDialog
{
    Q_OBJECT

public:
    explicit modifier_PC(QWidget *parent = nullptr);
    ~modifier_PC();

private slots:
    void on_B_ajouter_accepted();

    void on_B_ajouter_rejected();

private:
    Ui::modifier_PC *ui;
};

#endif // MODIFIER_PC_H
