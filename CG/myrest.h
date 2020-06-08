#ifndef MYREST_H
#define MYREST_H

#include <QDialog>

namespace Ui {
class myrest;
}

class myrest : public QDialog
{
    Q_OBJECT

public:
    explicit myrest(QWidget *parent = nullptr);
    int height;
    int width;
    ~myrest();

private slots:
    void on_button_ok_clicked();

private:
    Ui::myrest *ui;
};

#endif // MYREST_H
