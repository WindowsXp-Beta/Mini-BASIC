
#include "statement.h"
#include "guibasic.h"
#include "ui_guibasic.h"
#include <cstdio>
statement :: statement(){
    /* Empty */
}

statement :: ~statement(){
    /* Empty */
}

LETstatement :: LETstatement(QString init_var, expression *init_exp):var(init_var), exp(init_exp){}

LETstatement :: ~ LETstatement(){
    delete exp;
}

void LETstatement::execute(EvalState &state){
    int res = exp->eval(state);
    state.setValue(var, res);
}

REMstatement::REMstatement(){
    /* Empty */
}

REMstatement::~REMstatement() {
   /* Empty */
}

void REMstatement::execute(EvalState & state)
{
   /* Empty */
}

PRINTstatement::PRINTstatement(expression * init_exp):exp(init_exp) {}

PRINTstatement::~PRINTstatement()
{
    delete exp;
}

void PRINTstatement::execute(EvalState & state)
{
    QString res = QString::number(exp->eval(state));
    GuiBasic::ui_handle -> print(res);
}

ENDstatement::ENDstatement() {
   /* Empty */
}

ENDstatement::~ENDstatement() {
   /* Empty */
}

void ENDstatement::execute(EvalState & state)
{
    state.setPC(EvalState::HALT);
}

INPUTstatement::INPUTstatement(QString init_name):var(init_name) {}

INPUTstatement::~INPUTstatement()
{
    /* Empty */
}

void INPUTstatement::execute(EvalState & state)
{
    //int res = input_int();
    //state.setValue(var, res);
}

/*
IFstatement::IFstatement(expression * exp, LineNumber * ln):cond(exp), line_number(ln) {}

IFstatement::~IFstatement() {
   delete cond;
   delete line_number;
}

void IFstatement::execute(EvalState & state)
{
    bool res = cond->eval(state);
    if (res){
        state.setPC(line_number->eval(state));
    }
}
*/
