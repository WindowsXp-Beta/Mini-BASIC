#include "exp.h"

expression::expression(QString tree_id):tree_id(tree_id) {

}

expression::~expression() {
   /* Empty */
}

constantexp::constantexp(int val) : expression(QString::number(val)), value(val) {
    isCompound = false;
}

int constantexp::eval(EvalState & state) {
    return value;
}

identifierexp::identifierexp(QString id) : expression(id), name(id) {
    isCompound = false;
}

int identifierexp::eval(EvalState & state) {
    if (!state.isDefined(name)) throw BasicError("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

compoundexp::compoundexp(QString _op, expression * _lhs, expression * _rhs) : expression(_op), op(_op), lhs(_lhs), rhs(_rhs) {
    isCompound = true;
}

compoundexp:: ~ compoundexp(){
    delete lhs;
    delete rhs;
}
expression* compoundexp::getLHS()
{
    return lhs;
}
expression* compoundexp::getRHS()
{
    return rhs;
}

int compoundexp::eval(EvalState &state) {
    int lcr = lhs->eval(state);
    int rcr = rhs->eval(state);
    int result = 0;
    if (op == '+'){
        result = lcr + rcr;
    }
    else if (op == '-'){
        result = lcr - rcr;
    }
    else if (op == '*'){
        result = lcr * rcr;
    }
    else if (op == '/'){
        if (rcr == 0) throw BasicError("DIVIDE BY ZERO");
        result = lcr / rcr;
    }
    else throw BasicError("UNSUPPORTED OPERATOR");
    return result;
}
