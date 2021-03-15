#ifndef GUIBASIC_H
#define GUIBASIC_H

#include <QMainWindow>

#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui { class GuiBasic; }
QT_END_NAMESPACE

class GuiBasic : public QMainWindow
{
    Q_OBJECT

public:
    GuiBasic(QWidget *parent = nullptr);
    ~GuiBasic();

private slots:
    void on_btnLoadCode_clicked();

    void on_btnClearCode_clicked();

    void on_cmdLineEdit_returnPressed();

private:
    Ui::GuiBasic *ui;
    void LoadFile(const QString &filename);  //按下载入代码按钮加载文件函数

    bool isHaveLineNumber(QString &command);//判断是否存在行号

    bool isNumberExist(QString &command);
    //检测console中输入命令的行号是否已经出现过。返回行首的Cursor
    //上面是最初想法，后来不知道怎么返回一个空对象
    //改成如果找到就完成插入操作并返回true

    bool isOnlyNumber(QString &command);  //判断是否仅有行号



    /******command execute******/
    void RemStmt(QString &command);

    void LetStmt(QString &command);

    void PrintStmt(QString &command);

    void InputStmt(QString &command);

    void GotoStmt(QString &command);

    void IfStmt(QString &command);

    void EndStmt(QString &command);

    void calcutate(QString &command);

    void error_handler(int type);
};
#endif // GUIBASIC_H
