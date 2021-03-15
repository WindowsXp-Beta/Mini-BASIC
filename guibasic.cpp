#include "guibasic.h"
#include "ui_guibasic.h"

GuiBasic::GuiBasic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GuiBasic)
{
    ui->setupUi(this);
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

void GuiBasic::on_cmdLineEdit_returnPressed()
{
    QString command = ui -> cmdLineEdit -> text();
    if (isHaveLineNumber(command)) { // 如果有行号，则执行插入操作
        if (isNumberExist(command)) return;//判断是否行号出现过
        else {
            ui -> codedisplay -> appendPlainText(command);//直接在最后插入
            ui -> cmdLineEdit -> clear();
            return;
        }
    }

    else { //如果没有行号
        QString fun = command.split(' ').at(0);
/*
        if (fun == "LET") LetStmt(command);//==会发生类型转换，将const char* 转换为 QString
        else if (fun == "PRINT") PrintStmt(command);
        else if (fun == "INPUT") InputStmt(command);
        else */
        error_handler(1);
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
    if (command.split(' ' , Qt::SkipEmptyParts).size() == 1)//skipemptyparts 可以忽略多余的空格
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
/*
void GuiBasic::PrintStmt(QString &command)
{

}
*/

void GuiBasic::error_handler(int type)
{
    ui -> codebrowser -> appendPlainText("Unvalid syntax !");
}
/*
int GuiBasic::calcutate(QString &command)
{

}
*/
