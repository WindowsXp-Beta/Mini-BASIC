#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <QObject>
#include "exp.h"

enum stateType { LET, REM, INPUT, PRINT, END, GOTO, IF};

class statement : public QObject
//因为statement要使用slot signal机制，所以需继承QObject
{
    Q_OBJECT
public:
    statement();
    virtual ~statement();
    virtual void execute(EvalState &state) = 0;
    virtual void display_tree() = 0;
    stateType type;
};


class LETstatement : public statement
{
    Q_OBJECT
public:
    LETstatement(QString init_var, expression *init_exp);
    virtual ~LETstatement();
    virtual void execute(EvalState &state);
    virtual void display_tree();
private:
    // LET var = exp
        QString var;
        expression *exp;
};

class REMstatement : public statement
{
    Q_OBJECT
public:
    REMstatement(QString );
    virtual ~ REMstatement();
    virtual void execute(EvalState &state);
    virtual void display_tree();
private:
    QString annotation;
};

class INPUTstatement : public statement
{
    Q_OBJECT

public:
    INPUTstatement(QString init_name, int num = 0);
    virtual ~INPUTstatement();
    virtual void execute(EvalState & state);
    virtual void display_tree();

public slots:
    void begin_loop();//解决了QeventLoop的exec不是slot的问题，将exec包装
    void get_input(int var);//设置INPUT的num
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
    virtual void display_tree();
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
    virtual void display_tree();
};

class GOTOstatement : public statement
{
    Q_OBJECT
public:
    GOTOstatement(int ln);
    virtual ~GOTOstatement();
    virtual void execute(EvalState & state);
    virtual void display_tree();

private:
    //GOTO n
        int line_number;
};

class IFstatement : public statement
{
    public:
    IFstatement(int ln, QString op, expression *exp1, expression *exp2);
    virtual ~IFstatement();
    virtual void execute(EvalState & state);
    virtual void display_tree();
    private:
    //IF exp1 op exp2 THEN n
        int line_number;
        QString op;
        expression * exp1;
        expression * exp2;
};

#endif // STATEMENT_H
