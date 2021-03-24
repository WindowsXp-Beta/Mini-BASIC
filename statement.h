#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include "guibasic.h"
//#include "evalstate.h"
#include "exp.h"

class statement
{
    public:
    statement();
    virtual ~statement();
    virtual void execute(EvalState &state) = 0;
};


class LETstatement : public statement
{
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
public:
    REMstatement();
    virtual ~ REMstatement();
    virtual void execute(EvalState &state);
};

class INPUTstatement : public statement
{
    public:
    INPUTstatement(QString init_name);
    virtual ~INPUTstatement();
    virtual void execute(EvalState & state);
    private:
    // INPUT var
        QString var;
};

class PRINTstatement : public statement
{
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
    public:
    ENDstatement();
    virtual ~ENDstatement();
    virtual void execute(EvalState & state);

};
/*
class Gotostatement : public statement
{
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
