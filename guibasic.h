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

private:
    Ui::GuiBasic *ui;
    void LoadFile(const QString &filename);

};
#endif // GUIBASIC_H
