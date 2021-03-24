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

