#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class ReplaceDialog : public QDialog {
    Q_OBJECT

public:
    ReplaceDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle(tr("查找和替换"));

        // 创建输入框
        findLineEdit = new QLineEdit(this);
        replaceLineEdit = new QLineEdit(this);

        // 设置输入框的提示文本
        findLineEdit->setPlaceholderText(tr("查找内容"));
        replaceLineEdit->setPlaceholderText(tr("替换为"));

        // 创建按钮
        QPushButton *okButton = new QPushButton(tr("确定"), this);
        QPushButton *cancelButton = new QPushButton(tr("取消"), this);

        // 连接信号和槽
        connect(okButton, &QPushButton::clicked, this, &ReplaceDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &ReplaceDialog::reject);

        // 布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(findLineEdit);
        layout->addWidget(replaceLineEdit);
        layout->addWidget(okButton);
        layout->addWidget(cancelButton);
        setLayout(layout);
    }

    // 获取输入文本的函数
    QString getFindText() const { return findLineEdit->text(); }
    QString getReplaceText() const { return replaceLineEdit->text(); }

private:
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
};


#endif // REPLACEDIALOG_H
