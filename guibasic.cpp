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
    if (cmd_list.at(0) == '?') {
        int var = cmd_list.at(1).toInt();
        printf("%d\n", var);
        emit input_num(var);
        ui -> cmdLineEdit -> clear();
    }
    else if (isHaveLineNumber(command)) { // 如果有行号，则执行插入操作
        if (isNumberExist(command)) return;//判断是否行号出现过，如果出现过，直接在函数中处理
        else {
            ui -> codedisplay -> appendPlainText(command);//直接在最后插入
            ui -> cmdLineEdit -> clear();
        }
    }
    else { //如果没有行号
        statement *stmt = parsedirect(command);
        stmt->execute(s);
        ui -> cmdLineEdit -> clear();
    }
}

bool GuiBasic::isHaveLineNumber(QString &command)
{
    QString number = command.split(' ').at(0);
    QByteArray number_b = number.toLatin1();//Latin1是扩充了0x7F ~ 0xFF的兼容ASCII的编码
    for (int i = 0 ; i < number_b.size() ; i++) {
        if (number_b.at(i) < '0' || number_b.at(i) > '9') return false;
    }
    return true;
}

bool GuiBasic::isOnlyNumber(QString &command)
{
    if (command.split(' ', Qt::SkipEmptyParts).size() == 1)//skipemptyparts 可以忽略多余的空格
        return true;
    else return false;
}

bool GuiBasic::isNumberExist(QString &command)
{
    QString number = command.split(' ').at(0);
    QTextCursor cursor = ui -> codedisplay -> textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    do {
        cursor.select(QTextCursor::WordUnderCursor);
        if (cursor.selectedText() == number) {
            if (isOnlyNumber(command)) //if there is only line number delete this line
            {
                cursor.select(QTextCursor::LineUnderCursor);
                //cursor.selectedText().clear();
                cursor.removeSelectedText();
                ui -> cmdLineEdit -> clear();
                return true;
            }
            else
            {
                cursor.select(QTextCursor::LineUnderCursor);
                //cursor.selectedText().clear();
                //这有一个问题，selectedText返回一个新的QString，你删了它其实并没有什么用。
                //但为什么之前的测试了替换内容都是可以的呢，因为我们之前选中了一行，而insertText会替换这行选中的。
                cursor.removeSelectedText();//所以我们要用这个
                cursor.insertText(command);
                ui -> cmdLineEdit -> clear();
                return true;
            }
        }
    } while(cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1));

    return false;
}

/**提供给其他文件的接口**/
//提供给PRINT
void GuiBasic::print(QString &content) {
    ui -> codebrowser -> appendPlainText(content);
}

//提供给INPUT
void GuiBasic::set_ques_mark(){
    ui -> cmdLineEdit -> setText("?");
    emit stop_prog_input();
}
//void GuiBasic::on_btnRunCode_clicked()
//{

//}

