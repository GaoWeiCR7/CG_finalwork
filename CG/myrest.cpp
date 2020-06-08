#include "myrest.h"
#include "ui_myrest.h"

myrest::myrest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myrest)
{
    ui->setupUi(this);
}

myrest::~myrest()
{
    delete ui;
}

void myrest::on_button_ok_clicked()
{
    height = ui->lineEdit_h->text().toInt();
    width = ui->lineEdit_w->text().toInt();
    accept();
}
