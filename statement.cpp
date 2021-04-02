#include "statement.h"
#include "ui_guibasic.h"
#include <QEventLoop>
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

INPUTstatement::INPUTstatement(QString init_name, int num):var(init_name), num(num) {}

INPUTstatement::~INPUTstatement()
{
    /* Empty */
}

void INPUTstatement::get_input(int var)
{
    num = var;
}

void INPUTstatement::execute(EvalState & state)
{
    QEventLoop loop;
    connect(GuiBasic::ui_handle, SIGNAL(input_num(int)), this, SLOT(get_input(int)));
    connect(GuiBasic::ui_handle, SIGNAL(stop_prog_input()), &loop, SLOT(exec()));
    connect(GuiBasic::ui_handle, SIGNAL(input_num()), &loop, SLOT(quit()));
    GuiBasic::ui_handle -> set_ques_mark();
    state.setValue(var, num);
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
