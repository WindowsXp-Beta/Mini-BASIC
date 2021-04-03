#include "statement.h"
#include "guibasic.h"
#include <QEventLoop>
#include <cstdio>
#include <QDebug>

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

void INPUTstatement::begin_loop()
{
    //qDebug()<< "begin loop\n";
    QEventLoop loop;
    connect(GuiBasic::ui_handle, SIGNAL(input_num(int)), &loop, SLOT(quit()));
    loop.exec();
    //qDebug()<< "loop ended\n";
}

void INPUTstatement::execute(EvalState & state)
{
    //qDebug() << "begin input\n";
    connect(GuiBasic::ui_handle, SIGNAL(input_num(int)), this, SLOT(get_input(int)));
    connect(GuiBasic::ui_handle, SIGNAL(stop_prog_input()), this, SLOT(begin_loop()));
    GuiBasic::ui_handle -> set_ques_mark();
    //qDebug() << "Get input num is" << num << '\n';
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
