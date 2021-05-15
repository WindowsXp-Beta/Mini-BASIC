#include "guibasic.h"
#include "ui_guibasic.h"
#include "statement.h"
#include "parser.h"
#include "tokenscanner.h"


bool lineNuminRange(int lineNum);
bool isNum(QString s);

GuiBasic* GuiBasic::ui_handle = nullptr;

GuiBasic::GuiBasic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GuiBasic)
{
    ui -> setupUi(this);
    ui_handle = this;
    frequency = 1;
    ui -> codebrowser -> setReadOnly(true);
    ui -> syntax_tree -> setReadOnly(true);
    ui -> codedisplay -> setReadOnly(true);
    ui -> vardisplay -> setReadOnly(true);
}
GuiBasic::~GuiBasic(){
    delete ui;
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
        Tokenscanner scanner(code_line);
        bool flag;
        if(!scanner.hasMoreTokens()) continue;
        int line_num = scanner.nextToken().toInt(&flag);

        statement *stmt = nullptr;
        if (!flag || !lineNuminRange(line_num)) error_display("LINE NUMBER MUST BE LARGER THAN 0 AND SMALLER THAN 1000000");
        else if(!scanner.hasMoreTokens()) pro.removeSourceLine(line_num);
        else {
            try {
                stmt = parsestatement(scanner);
            }  catch (BasicError err) {
                error_display(err.get_err_meg());
            }
        pro.addSourceLine(line_num, code_line, stmt);
        }
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
    s.clear();
    pro.clear();
    ui -> codedisplay -> clear();
    ui -> codebrowser -> clear();
    ui -> syntax_tree -> clear();
    ui -> vardisplay -> clear();
    frequency = 1;
}

void GuiBasic::on_btnRunCode_clicked()
{
    if (frequency == 1) {
        ui -> btnClearCode -> setEnabled(false);
        ui -> btnLoadCode -> setEnabled(false);
        ui -> codebrowser -> clear();
        ui -> syntax_tree -> clear();
        ui -> vardisplay -> clear();
        s.clear();
        pro.init();
    }
    try {
        pro.run(s);//这里只会catch GOTO非法行的错误，其余错误只会报错不会终止程序运行
    }
    catch(BasicError err) {
        ui -> btnClearCode -> setEnabled(true);
        ui -> btnLoadCode -> setEnabled(true);
        frequency = 1;
        error_display(err.get_err_meg());
    }
}

void GuiBasic::on_btnDebugCode_clicked()
{
    if (frequency == 1) {
        ui -> btnClearCode -> setEnabled(false);
        ui -> btnLoadCode -> setEnabled(false);
        ui -> codebrowser -> clear();
        ui -> syntax_tree -> clear();
        ui -> vardisplay -> clear();
        s.clear();
        pro.init();
    }
    frequency++;
    try {
        pro.debug(s);
    }
    catch(BasicError err) {
        ui -> btnClearCode -> setEnabled(true);
        ui -> btnLoadCode -> setEnabled(true);
        frequency = 1;
        error_display(err.get_err_meg());
    }
}


void GuiBasic::on_cmdLineEdit_returnPressed()//命令行中输入回车
{
    QString command = ui -> cmdLineEdit -> text();
    Tokenscanner scanner(command);
    if (!scanner.hasMoreTokens()) return; //empty line

    QString dirCmd = scanner.nextToken();

    if (dirCmd == "RUN") {
        on_btnRunCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "LOAD") {
        on_btnLoadCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "CLEAR") {
        on_btnClearCode_clicked();
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "HELP") {
        get_help();
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "QUIT") {
        emit quit_app();
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "INT?") { //处理INPUT的情况
        bool isNum = false, isNeg = false;
        if(!scanner.hasMoreTokens()) {
            error_display("YOU SHOULD INPUT AN INTEGER");
            ui -> cmdLineEdit -> setText("INT? ");
            return;
        }
        QString token = scanner.nextToken();
        if (token == '+') {
            isNeg = false;
            try {
                token = scanner.nextToken();
            }  catch (BasicError err) {
                error_display("YOU SHOULD INPUT AN INTEGER");
                ui -> cmdLineEdit -> setText("INT? ");
                return;
            }
        }
        else if (token == '-') {
            isNeg = true;
            try {
                token = scanner.nextToken();
            }  catch (BasicError err) {
                error_display("YOU SHOULD INPUT AN INTEGER");
                ui -> cmdLineEdit -> setText("INT? ");
                return;
            }
        }
        int var = token.toInt(&isNum);
        if (!isNum) {
            error_display("YOU SHOULD INPUT AN INTEGER");
            ui -> cmdLineEdit -> setText("INT? ");
            return;
        }
        if (isNeg) {
            var = -var;
        }
        emit input_num(var);
        ui -> btnDebugCode -> setEnabled(true);
        ui -> btnRunCode -> setEnabled(true);
        ui -> cmdLineEdit -> clear();
    }
    else if (dirCmd == "STR?") {
        if(!scanner.hasMoreTokens()) {
            error_display("YOU SHOULD INPUT AN STRING CLOSED BY QUOTES");
            ui -> cmdLineEdit -> setText("STR? ");
            return;
        }
        QString firstQuote = scanner.nextToken();
        if (!isQuote(firstQuote)) {
            error_display("YOU SHOULD INPUT AN STRING CLOSED BY QUOTES");
            ui -> cmdLineEdit -> setText("STR? ");
            return;
        }
        try {
            QStringList result;
            QString tmp = scanner.nextToken();
            while(!isQuote(tmp)) {
                result.append(tmp);
                tmp = scanner.nextToken();
            }
            if (tmp != firstQuote) throw BasicError("THE STRING SHOULD BE CLOSED IN THE SAME QUOTE");
            if (scanner.hasMoreTokens()) throw BasicError("EXTRA INPUT");
            emit input_str(result.join(' '));
        }  catch (BasicError err) {
           error_display(err.get_err_meg());
           ui -> cmdLineEdit -> setText("STR? ");
           return;
        }
        ui -> btnDebugCode -> setEnabled(true);
        ui -> btnRunCode -> setEnabled(true);
        ui -> cmdLineEdit -> clear();
    }
    else if (isNum(dirCmd)) { // 如果有行号，则执行插入操作
        int line_number = dirCmd.toInt();
        if (!scanner.hasMoreTokens()) { //仅含有行号，删除该行
            pro.removeSourceLine(line_number);
        }
        else {
            statement *stmt = nullptr;
            if (!lineNuminRange(line_number))
                error_display("LINE NUMBER MUST BE LARGER THAN 0 AND SMALLER THAN 1000000");
            else {
                try {
                    stmt = parsestatement(scanner);
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
        scanner.saveToken();
        try {
            stmt = parsedirect(scanner);
        }  catch (BasicError err) {
            error_display(err.get_err_meg());
        }
        if (stmt) {
            switch (stmt -> type) {
                case LET:{
                    syn_tree_display("LET =");
                    break;
                }
                case PRINT:{
                    syn_tree_display("PRINT");
                    break;
                }
                case INPUT:{
                    syn_tree_display("INPUT");
                    break;
                }
                case INPUTS:{
                    syn_tree_display("INPUTS");
                    break;
                }
                case PRINTF:{
                    syn_tree_display("PRINTF");
                    break;
                }
            }
            try {
                stmt -> execute(s);
            }  catch (BasicError err) {
                error_display(err.get_err_meg());
            }
        }
        else {
            syn_tree_display("ERROR");
        }
        s.list();
        ui -> cmdLineEdit -> clear();
        delete stmt;
    }
}

/**提供给其他文件的接口**/
//提供给PRINT
void GuiBasic::print(QString &content) {
    ui -> codebrowser -> appendPlainText(content);
}

//提供给INPUT
void GuiBasic::set_ques_mark_int() {
    ui -> cmdLineEdit -> setText("INT? ");
    ui -> btnDebugCode -> setEnabled(false);
    ui -> btnRunCode -> setEnabled(false);
    emit stop_prog_input();
}
//提供给INPUTS
void GuiBasic::set_ques_mark_str() {
    ui -> cmdLineEdit -> setText("STR? ");
    ui -> btnDebugCode -> setEnabled(false);
    ui -> btnRunCode -> setEnabled(false);
    emit stop_prog_input();
}
//提供给program.cpp
void GuiBasic::show_line(const QString &line) {
    ui -> codedisplay -> appendPlainText(line);
}

//提供给evalstate.cpp
void GuiBasic::show_var(const QString &var) {
    ui -> vardisplay -> appendPlainText(var);
}
void GuiBasic::clear_var() {
    ui -> vardisplay -> clear();
}

QTextCursor GuiBasic::get_cursor() {
    QTextCursor cursor(ui -> codedisplay -> document());
    return cursor;
}

//提供给语法树显示
void GuiBasic::syn_tree_display(QString line) {
    ui -> syntax_tree -> appendPlainText(line);
}

//错误信息打印
void GuiBasic::error_display(QString err_meg) {
    QMessageBox msgBox;
    msgBox.setText(err_meg);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void GuiBasic::run_finished() {
    ui -> btnClearCode -> setEnabled(true);
    ui -> btnLoadCode -> setEnabled(true);
    frequency = 1;
    QMessageBox msgBox;
    msgBox.setText("Program finished");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

/**一些判断函数**/
bool lineNuminRange(int lineNum)
{
    if (lineNum > 1000000 || lineNum < 0) return false;
    return true;
}

bool isNum(QString s)
{
    bool isNum = false;
    s.toInt(&isNum);
    return isNum;
}
