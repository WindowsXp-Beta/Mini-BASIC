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
    QString command = ui -> cmdLineEdit -> text();
    QString number = command.split(' ').at(0);
    QTextCursor cursor = ui -> codedisplay -> textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    do {
        cursor.select(QTextCursor::WordUnderCursor);
        if (cursor.selectedText() == number){
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.selectedText().clear();
            cursor.insertText(command);
            ui -> cmdLineEdit -> clear();
            return;
        }
    } while(cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1));
    ui -> codedisplay -> appendPlainText(command);
    ui -> cmdLineEdit -> clear();
}

void GuiBasic::on_btnClearCode_clicked()
{
    ui -> codedisplay -> clear();
    ui -> codebrowser -> clear();
    ui -> plainTextEdit_3 -> clear();
}
