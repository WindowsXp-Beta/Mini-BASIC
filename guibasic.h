#ifndef GUIBASIC_H
#define GUIBASIC_H

#include <QMainWindow>

#include <QtWidgets>
#include "program.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GuiBasic; }
QT_END_NAMESPACE

class GuiBasic : public QMainWindow
{
    Q_OBJECT //这是一个宏

public:
    GuiBasic(QWidget *parent = nullptr);
    ~GuiBasic();
    static GuiBasic * ui_handle;
    /**Gui提供给其他文件的接口**/
    void print(QString &);//for PRINT
    void set_ques_mark();//for INPUT
    void show_line(const QString &);//for program.cpp
    QTextCursor get_cursor();//return the cursor point to the end of file
    void syn_tree_display(QString line);//for statement's display_tree
    void error_display(QString err_meg);
signals:
    void stop_prog_input();
    void input_num(int num);
    void input_str(QString value);
    void quit_app();

private slots:
    void on_btnLoadCode_clicked();

    void on_btnClearCode_clicked();

    void on_btnRunCode_clicked();

    void on_btnDebugCode_clicked();

    void get_help();

    void on_cmdLineEdit_returnPressed();

private:
    Ui::GuiBasic *ui;
    QString help_doc;
    EvalState s;
    Program pro;

    void LoadFile(const QString &filename);  //按下载入代码按钮加载文件函数
};
#endif // GUIBASIC_H
