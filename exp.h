#ifndef EXP_H
#define EXP_H

#include "error.h"
#include "evalstate.h"
#include <QString>

enum expType {constexp, identexp, comexp, strexp};
class expression
{
public:
    expression();
    virtual ~expression();
    virtual int eval(EvalState & state) = 0;
    virtual void display(QString space) = 0;
    expType type;
};

class constantexp: public expression{
public:
    constantexp(int val);
    virtual int eval(EvalState & state);
    virtual void display(QString space);
private:
    int value;
};

class identifierexp : public expression {
public:
    identifierexp(QString id);
    virtual int eval(EvalState & state);
    virtual void display(QString space);
    QString getName();
private:
    QString name;
};

class compoundexp: public expression {
public:
    compoundexp(QString _op, expression * _lhs, expression * _rhs);
    virtual ~compoundexp();
    virtual int eval(EvalState &state);
    virtual void display(QString space);
private:
    QString op;
    expression *lhs, *rhs;
};

class stringexp: public expression {
public:
    stringexp(QString value);
    virtual int eval(EvalState & state);
    virtual void display(QString space);
    QString getValue();
private:
    QString value;
};
#endif // EXP_H
