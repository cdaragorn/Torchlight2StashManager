#include "addgroupdialog.h"

#include <QPushButton>
#include <QLineEdit>

AddGroupDialog::AddGroupDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(200, 100);
    this->setWindowTitle("Group Name");

    QPushButton* okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->move(10, this->height() - okButton->height() - 10);

    QPushButton* cancelButton = new QPushButton(this);
    cancelButton->setText("Cancel");
    cancelButton->move(10 + okButton->width(), this->height() - cancelButton->height() - 10);

    QLineEdit* textBox = new QLineEdit(this);
    textBox->move(10, 20);
    textBox->resize(this->width() - 20, 20);
    textBox->setFocus();

    connect(okButton, SIGNAL(pressed()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(pressed()), this, SLOT(reject()));
    connect(textBox, SIGNAL(textChanged(QString)), this, SLOT(OnGroupNameTextChanged(QString)));
}

QString AddGroupDialog::GroupName()
{
    return mGroupName;
}

void AddGroupDialog::OnGroupNameTextChanged(QString newText)
{
    mGroupName = newText;
}
