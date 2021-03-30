#include "exp.h"

expression::expression() {
   /* Empty */
}

expression::~expression() {
   /* Empty */
}

constantexp::constantexp(int val) : value(val) {}

int constantexp::eval(EvalState & state) {
   return value;
}

identifierexp::identifierexp(QString id) : name(id) {}

int identifierexp::eval(EvalState & state) {
   if (state.isDefined(name)) //error("VARIABLE NOT DEFINED");
   return state.getValue(name);
}

compoundexp::compoundexp(QString _op, expression * _lhs, expression * _rhs) : op(_op), lhs(_lhs), rhs(_rhs){}

compoundexp:: ~ compoundexp(){
    delete lhs;
    delete rhs;
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
        result = lcr / rcr;
    }
    return result;
}
