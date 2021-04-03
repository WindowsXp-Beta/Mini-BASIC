#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <QObject>
#include "guibasic.h"
//#include "evalstate.h"
#include "exp.h"

class statement : public QObject
{
    Q_OBJECT
public:
    statement();
    virtual ~statement();
    virtual void execute(EvalState &state) = 0;
};


class LETstatement : public statement
{
    Q_OBJECT
public:
    LETstatement(QString init_var, expression *init_exp);
    virtual ~LETstatement();
    virtual void execute(EvalState &state);
private:
    // LET var = exp
        QString var;
        expression *exp;
};

class REMstatement : public statement
{
    Q_OBJECT
public:
    REMstatement();
    virtual ~ REMstatement();
    virtual void execute(EvalState &state);
};

class INPUTstatement : public statement
{
    Q_OBJECT
public:
    INPUTstatement(QString init_name, int num = 0);
    virtual ~INPUTstatement();
    virtual void execute(EvalState & state);
public slots:
    void begin_loop();
    void get_input(int var);//handle the input_num signal
private:
    // INPUT var
        QString var;
        int num;
};

class PRINTstatement : public statement
{
    Q_OBJECT

public:
    PRINTstatement(expression * init_exp);
    virtual ~PRINTstatement();
    virtual void execute(EvalState & state);
private:
    // PRINT exp
        expression * exp;
};

class ENDstatement : public statement
{
    Q_OBJECT

public:
    ENDstatement();
    virtual ~ENDstatement();
    virtual void execute(EvalState & state);

};
/*
class Gotostatement : public statement
{
    Q_OBJECT
public:
    Gotostatement(LineNumber * ln);
    virtual ~Gotostatement();
    virtual void execute(EvalState & state);

private:
    //GOTO n
        LineNumber * line_number;
};

class IFstatement : public statement
{
    public:
    IFstatement(expression * exp, LineNumber * ln);
    virtual ~IFstatement();
    virtual void execute(EvalState & state);

    private:
    //IF exp1 op exp2 THEN n
        LineNumber * line_number;
        expression * cond;
};
*/
#endif // STATEMENT_H
