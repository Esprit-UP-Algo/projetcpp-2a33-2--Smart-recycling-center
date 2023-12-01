#include "modifier_pc.h"
#include "ui_modifier_pc.h"
#include "pointdecollect.h"
#include <QMessageBox>
#include <QIntValidator>
modifier_PC::modifier_PC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifier_PC)
{
    ui->setupUi(this);
    ui->lineEdit_idpc->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_quntite->setValidator(new QIntValidator(0, 9999999, this));

}

modifier_PC::~modifier_PC()
{
    delete ui;
}

void modifier_PC::on_B_ajouter_accepted()
{
        QMessageBox msgBox;
        int id=ui->lineEdit_idpc->text().toInt();
        QDate date= ui->dateEdit->date();
        QString lieu = ui->lineEdit_lieu->text();
        int quntte=ui->lineEdit_quntite->text().toInt();
        PointDeCollect p(id,date,lieu,quntte);
        bool test=p.modifier();

        if (test)
        msgBox.setText("modifier avec succe");
        else
            msgBox.setText("modifier suppreme ");
        msgBox.exec();
}

void modifier_PC::on_B_ajouter_rejected()
{
     this->close();
}
