#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <QMap>
#include <QString>
#include "error.h"

class EvalState //保存程序运行时的状态
{
private:
    QMap<QString, int> symboltable;//保存程序运行时的变量
    int program_counter;//PC

public:
    static const int SEQUENTIAL = -1;  //Special Line Number for sequential executive;
    static const int HALT = -2;  //Special Line Number for halting;
    EvalState();
    ~EvalState();
    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void setPC(int line_number);
    int getPC();
    void clear();
};

#endif // EVALSTATE_H
