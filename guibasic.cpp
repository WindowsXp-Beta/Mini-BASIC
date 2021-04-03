#include "guibasic.h"
#include "ui_guibasic.h"
#include "statement.h"
#include "parser.h"

GuiBasic* GuiBasic::ui_handle = nullptr;

GuiBasic::GuiBasic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GuiBasic)
{
    ui->setupUi(this);
    ui_handle = this;
}

GuiBasic::~GuiBasic()
{
    delete ui;
}

void GuiBasic::LoadFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(filename), file.errorString()));
        return;
    }

    QTextStream in(&file);
    ui -> codedisplay -> setPlainText(in.readAll());
}

void GuiBasic::on_btnLoadCode_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        LoadFile(fileName);
    }
}

void GuiBasic::on_btnClearCode_clicked()
{
    ui -> codedisplay -> clear();
    ui -> codebrowser -> clear();
    ui -> syntax_tree -> clear();
}

void GuiBasic::on_cmdLineEdit_returnPressed()//命令行中输入回车
{
    QString command = ui -> cmdLineEdit -> text();
    QStringList cmd_list = command.split(' ', Qt::SkipEmptyParts);

    if (cmd_list.at(0) == '?') { //处理INPUT的情况
        int var = cmd_list.at(1).toInt();
        emit input_num(var);
        ui -> cmdLineEdit -> clear();
    }

    else if (isHaveLineNumber(cmd_list)) { // 如果有行号，则执行插入操作
        QString number = cmd_list.at(0);
        int line_number = number.toInt();
        if (cmd_list.size() == 1) { //仅含有行号，删除该行
            pro.removeSourceLine(line_number);
        }
        else {
            statement *stmt = parsestatement(cmd_list);
            pro.addSourceLine(line_number, command, stmt);
        }
        ui -> codedisplay -> clear();
        pro.list();
        ui -> cmdLineEdit -> clear();
    }
    else { //如果没有行号
        statement *stmt = parsedirect(cmd_list);
        stmt->execute(s);
        ui -> cmdLineEdit -> clear();
    }
}

bool GuiBasic::isHaveLineNumber(QStringList &cmd_list)
{
    QString number = cmd_list.at(0);
    bool is_num = false;
    number.toInt(&is_num, 10);
    return is_num;
}

bool GuiBasic::isOnlyNumber(QString &command)
{
    if (command.split(' ', Qt::SkipEmptyParts).size() == 1)//skipemptyparts 可以忽略多余的空格
        return true;
    else return false;
}

/**提供给其他文件的接口**/
//提供给PRINT
void GuiBasic::print(QString &content) {
    ui -> codebrowser -> appendPlainText(content);
}

//提供给INPUT
void GuiBasic::set_ques_mark(){
    ui -> cmdLineEdit -> setText("? ");
    emit stop_prog_input();
}

//提供给program.cpp
void GuiBasic::show_line(const QString &line){
    ui -> codedisplay -> appendPlainText(line);
}

//void GuiBasic::on_btnRunCode_clicked()
//{

//}

