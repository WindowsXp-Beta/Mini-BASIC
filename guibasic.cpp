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
    QString filename = QFileDialog::getOpenFileName(this);
    if (!filename.isEmpty())
        LoadFile(filename);
}
