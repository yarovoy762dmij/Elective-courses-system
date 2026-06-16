#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_btnRegister_clicked();
    void on_btnCancel_clicked();

private:
    Ui::RegisterDialog *ui;
    bool validateFields(QString &errorMsg);
};

#endif // REGISTERDIALOG_H
