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
void GuiBasic::get_help()
{
    ui -> codebrowser -> clear();
    ui -> codebrowser -> appendPlainText("# A minimal BASIC interpreter");
    ui -> codebrowser -> appendPlainText("## We support two ways to input code");
    ui -> codebrowser -> appendPlainText("1. Just type Instructions in the bottom box and press return and you'll see your code being displayed on the top-left box.");
    ui -> codebrowser -> appendPlainText("2. Press Load Button and choose a file to load.You will also see the file being displayed on the top-left box.");
    ui -> codebrowser -> appendPlainText("## Instructions we support");
    ui -> codebrowser -> appendPlainText("- REM:comment your program");
    ui -> codebrowser -> appendPlainText("- GOTO n:goto line n and execute");
    ui -> codebrowser -> appendPlainText("- IF exp1 op exp2 THEN n:if exp1 op exp2 is right then goto line n");
    ui -> codebrowser -> appendPlainText("- INPUT var:input a variable");
    ui -> codebrowser -> appendPlainText("- LET variablename = exp:set the value of a variable to the expression");
    ui -> codebrowser -> appendPlainText("- PRINT exp:print the value of expression");
    ui -> codebrowser -> appendPlainText("- END:marks the end of your program");
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
    QString code_line;
    while ( !(code_line = in.readLine()).isNull() ) {
        QStringList cmd_list = code_line.split(' ', Qt::SkipEmptyParts);
        int line_num = cmd_list.at(0).toInt();

        statement *stmt = nullptr;
        if (!lineNuminRange(line_num)) error_display("LINE NUMBER MUST BE LARGER THAN 0 AND SMALLER THAN 1000000");
        else {
            try {
                stmt = parsestatement(cmd_list);
            }  catch (BasicError err) {
                error_display(err.get_err_meg());
            }
        }
        pro.addSourceLine(line_num, code_line, stmt);
    }
    pro.list();
}

void GuiBasic::on_btnLoadCode_clicked()
{
    on_btnClearCode_clicked();
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        LoadFile(fileName);
    }
}

void GuiBasic::on_btnClearCode_clicked()
{
    pro.clear();
    s.clear();
    ui -> codedisplay -> clear();
    ui -> codebrowser -> clear();
    ui -> syntax_tree -> clear();
}

void GuiBasic::on_btnRunCode_clicked()
{
    pro.run(s);
}

void GuiBasic::on_cmdLineEdit_returnPressed()//命令行中输入回车
{
    QString command = ui -> cmdLineEdit -> text();
    QStringList cmd_list = command.split(' ', Qt::SkipEmptyParts);

    if (cmd_list.at(0) == "RUN") {
        on_btnRunCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (cmd_list.at(0) == "LOAD") {
        on_btnLoadCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (cmd_list.at(0) == "CLEAR") {
        on_btnClearCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (cmd_list.at(0) == "HELP") {
        get_help();
        ui -> cmdLineEdit -> clear();
    }
    else if (cmd_list.at(0) == '?') { //处理INPUT的情况
        bool isNum = false;
        int var = cmd_list.at(1).toInt(&isNum);
        if (!isNum) error_display("YOU CAN ONLY INPUT INTEGER");
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
            statement *stmt = nullptr;
            if (!lineNuminRange(line_number))
                error_display("LINE NUMBER MUST BE LARGER THAN 0 AND SMALLER THAN 1000000");
            else {
                try {
                    stmt = parsestatement(cmd_list);
                }  catch (BasicError err) {
                    error_display(err.get_err_meg());
                }
            }
            pro.addSourceLine(line_number, command, stmt);
        }
        ui -> codedisplay -> clear();
        pro.list();
        ui -> cmdLineEdit -> clear();
    }
    else { //如果没有行号
        statement *stmt = nullptr;
        try {
            stmt = parsedirect(cmd_list);
        }  catch (BasicError err) {
            error_display(err.get_err_meg());
        }
        if (stmt) {
            try {
                stmt->execute(s);
            }  catch (BasicError err) {
                error_display(err.get_err_meg());
            }
        }
        else {
            QString tree_meg = "ERROR";
            syn_tree_display(tree_meg);
        }
        ui -> cmdLineEdit -> clear();
        delete stmt;
    }
}

bool GuiBasic::isHaveLineNumber(QStringList &cmd_list)
{
    QString number = cmd_list.at(0);
    bool is_num = false;
    number.toInt(&is_num, 10);
    return is_num;
}

bool GuiBasic::lineNuminRange(int lineNum)
{
    if (lineNum > 1000000 || lineNum < 0) return false;
    return true;
}
/**提供给其他文件的接口**/
//提供给PRINT
void GuiBasic::print(QString &content) {
    ui -> codebrowser -> appendPlainText(content);
}

//提供给INPUT
void GuiBasic::set_ques_mark() {
    ui -> cmdLineEdit -> setText("? ");
    emit stop_prog_input();
}

//提供给program.cpp
void GuiBasic::show_line(const QString &line) {
    ui -> codedisplay -> appendPlainText(line);
}

//提供给语法树显示
void GuiBasic::syn_tree_display(QString line) {
    ui -> syntax_tree -> appendPlainText(line);
}

//错误信息打印
void GuiBasic::error_display(QString err_meg) {
    QString err_meg_dis = "ERROR!!! " + err_meg;
    ui -> codebrowser -> appendPlainText(err_meg_dis);
}
