#ifndef EXP_H
#define EXP_H

#include "error.h"
#include "evalstate.h"
#include <QString>

class expression
{
public:
    expression(QString = " ");
    virtual ~expression();
    virtual int eval(EvalState & state) = 0;
    bool isCompound;
    QString tree_id;
};

class constantexp: public expression{
public:
    constantexp(int val);
    virtual int eval(EvalState & state);
private:
    int value;
};

class identifierexp : public expression {
public:
    identifierexp(QString id);
    virtual int eval(EvalState & state);
private:
    QString name;
};

class compoundexp: public expression {
public:
    compoundexp(QString _op, expression * _lhs, expression * _rhs);
    virtual ~compoundexp();
    expression *getLHS();
    expression *getRHS();
    QString getop();
    virtual int eval(EvalState &state);
private:
    QString op;
    expression *lhs, *rhs;
};
#endif // EXP_H
