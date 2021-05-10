#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <QMap>
#include <QString>
#include "error.h"

class EvalState //保存程序运行时的状态
{
private:
    QMap<QString, int> symboltableNum;//保存程序运行时的数值变量
    QMap<QString, QString> symboltableStr;//保存程序运行时的字符串变量
    int program_counter;//PC

public:
    static const int SEQUENTIAL = -1;  //Special Line Number for sequential executive;
    static const int HALT = -2;  //Special Line Number for halting;
    EvalState();
    ~EvalState();
    void setValue(QString var, int value);
    void setValue(QString var, QString value);
    int getValue(QString var);
    QString getStr(QString var);
    bool isDefined(QString var);
    bool isDefinedStr(QString var);
    void setPC(int line_number);
    int getPC();
    void clear();
    void list();
};

#endif // EVALSTATE_H
