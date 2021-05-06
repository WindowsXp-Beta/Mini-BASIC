#include "exp.h"
#include "guibasic.h"

expression::expression(){
    /* Empty */
}

expression::~expression() {
   /* Empty */
}

constantexp::constantexp(int val) : value(val) {
    type = constexp;
}

int constantexp::eval(EvalState & state) {
    return value;
}

void constantexp::display(QString space) {
    GuiBasic::ui_handle -> syn_tree_display(space + QString::number(value));
}

identifierexp::identifierexp(QString id) : name(id) {
    type = identexp;
}

int identifierexp::eval(EvalState & state) {
    if (!state.isDefined(name)) throw BasicError("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

void identifierexp::display(QString space) {
    GuiBasic::ui_handle -> syn_tree_display(space + name);
}
QString identifierexp::getName() {
    return name;
}

compoundexp::compoundexp(QString _op, expression * _lhs, expression * _rhs) : op(_op), lhs(_lhs), rhs(_rhs) {
    type = comexp;
}

compoundexp:: ~ compoundexp(){
    delete lhs;
    delete rhs;
}


int compoundexp::eval(EvalState &state) {
    if (lhs -> type == strexp || rhs -> type == strexp) throw BasicError("String cannot be evaluated");
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
    else if (op == "**"){
        result = pow(lcr, rcr);
    }
    else throw BasicError("UNSUPPORTED OPERATOR");
    return result;
}

void compoundexp::display(QString space) {
    GuiBasic::ui_handle -> syn_tree_display(space + op);
    lhs -> display(space + "    ");
    rhs -> display(space + "    ");
}

stringexp::stringexp(QString value):value(value) {
    type = strexp;
}

int stringexp::eval(EvalState & state) {
    return 0;
}

void stringexp::display(QString space) {
    GuiBasic::ui_handle -> syn_tree_display(space + "\"" + value + "\"");
}

QString stringexp::getValue() {
    return value;
}
