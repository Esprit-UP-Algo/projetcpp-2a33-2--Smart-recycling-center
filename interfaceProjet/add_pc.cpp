#include "add_pc.h"
#include "ui_add_pc.h"
#include "pointdecollect.h"
#include <QMessageBox>
#include <QIntValidator>
add_PC::add_PC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_PC)
{

    ui->setupUi(this);
    ui->lineEdit_idpc->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_quntite->setValidator(new QIntValidator(0, 9999999, this));

}

add_PC::~add_PC()
{
    delete ui;
}

void add_PC::on_B_ajouter_accepted()
{



}

void add_PC::on_B_ajouter_rejected()
{
     this->close();
}
