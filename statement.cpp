#include "statement.h"
#include "guibasic.h"
#include <QEventLoop>
#include <QDebug>
#include <QQueue>

statement :: statement(){
    /* Empty */
}

statement :: ~statement(){
    /* Empty */
}
/**** LET begin ****/
LETstatement :: LETstatement(QString init_var, expression *init_exp):var(init_var), exp(init_exp){
    type = LET;
}

LETstatement :: ~ LETstatement(){
    delete exp;
}

void LETstatement::execute(EvalState &state){
    display_tree();
    int res = exp->eval(state);
    state.setValue(var, res);
}
void LETstatement::display_tree() {
    QString space = "    ";
    QString tree_var = space + var;
    GuiBasic::ui_handle -> syn_tree_display(tree_var);
    //开始处理表达式
    expression *cur_exp = exp;
    int next_len = 1;//表示下一行有几个元素，初始根节点有一个元素
    QQueue<expression *> exp_queue;
    exp_queue.enqueue(cur_exp);
    while (true) {
        if (next_len == 0) break;
        int len = next_len;
        next_len = 0;
        while (len--) { //处理一行
            cur_exp = exp_queue.dequeue();
            if (cur_exp -> isCompound) {
                next_len += 2;
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getLHS() );
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getRHS() );
            }
            QString tree_exp = space + cur_exp -> tree_id;
            GuiBasic::ui_handle -> syn_tree_display(tree_exp);
        }
        space += "    ";
    }
}
/**** LET end ****/


/**** REM begin ****/
REMstatement::REMstatement(QString init_anno): annotation(init_anno) {
    type = REM;
}

REMstatement::~REMstatement() {
   /* Empty */
}

void REMstatement::execute(EvalState & state)
{
   display_tree();
}
void REMstatement::display_tree()
{
    QString space = "    ";
    QString tree_anno = space + annotation;
    GuiBasic::ui_handle -> syn_tree_display(tree_anno);
}
/**** REM end ****/

/**** PRINT begin ****/
PRINTstatement::PRINTstatement(expression * init_exp):exp(init_exp) {
    type = PRINT;
}

PRINTstatement::~PRINTstatement()
{
    delete exp;
}

void PRINTstatement::execute(EvalState & state)
{
    display_tree();
    QString res = QString::number(exp->eval(state));
    GuiBasic::ui_handle -> print(res);
}

void PRINTstatement::display_tree()
{
    QString space = "    ";
    //开始处理表达式
    expression *cur_exp = exp;
    int next_len = 1;//表示下一行有几个元素，初始根节点有一个元素
    QQueue<expression *> exp_queue;
    exp_queue.enqueue(cur_exp);
    while (true) {
        if (next_len == 0) break;
        int len = next_len;
        next_len = 0;
        while (len--) { //处理一行
            cur_exp = exp_queue.dequeue();
            if (cur_exp -> isCompound) {
                next_len += 2;
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getLHS());
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getRHS());
            }
            QString tree_exp = space + cur_exp -> tree_id;
            GuiBasic::ui_handle -> syn_tree_display(tree_exp);
        }
        space += "    ";
    }
}
/**** PRINT end ****/


/**** END begin ****/
ENDstatement::ENDstatement() {
   type = END;
}

ENDstatement::~ENDstatement() {
   /* Empty */
}

void ENDstatement::execute(EvalState & state)
{
    display_tree();
    state.setPC(EvalState::HALT);
}

void ENDstatement::display_tree(){
    /*** Empty ***/
}
/**** END end ****/

/**** INPUT begin ****/
INPUTstatement::INPUTstatement(QString init_name, int num):var(init_name), num(num) {
    type = INPUT;
}

INPUTstatement::~INPUTstatement()
{
    /* Empty */
}
void INPUTstatement::display_tree()
{
    QString syn_var = "    " + var;
    GuiBasic::ui_handle -> syn_tree_display(syn_var);
}

void INPUTstatement::get_input(int var)
{
    num = var;
}

void INPUTstatement::begin_loop()
{
    QEventLoop loop;
    connect(GuiBasic::ui_handle, SIGNAL(input_num(int)), &loop, SLOT(quit()));
    loop.exec();
}

void INPUTstatement::execute(EvalState & state)
{
    display_tree();
    connect(GuiBasic::ui_handle, SIGNAL(input_num(int)), this, SLOT(get_input(int)));
    connect(GuiBasic::ui_handle, SIGNAL(stop_prog_input()), this, SLOT(begin_loop()));
    GuiBasic::ui_handle -> set_ques_mark();
    state.setValue(var, num);
    disconnect(GuiBasic::ui_handle, SIGNAL(input_num(int)), this, SLOT(get_input(int)));
    disconnect(GuiBasic::ui_handle, SIGNAL(stop_prog_input()), this, SLOT(begin_loop()));
}
/**** INPUT end ****/


/**** GOTO begin ****/
GOTOstatement::GOTOstatement(int ln):line_number(ln){
    type = GOTO;
}

GOTOstatement::~GOTOstatement(){
    /* Empty */
}

void GOTOstatement::execute(EvalState &state)
{
    display_tree();
    state.setPC(line_number);
}

void GOTOstatement::display_tree()
{
    QString space = "    ";
    QString tree_dis = space + QString::number(line_number);
    GuiBasic::ui_handle -> syn_tree_display(tree_dis);
}
/**** GOTO end ****/

/**** IF begin ****/
IFstatement::IFstatement(int ln, QString op, expression * exp1,expression * exp2):line_number(ln),op(op) ,exp1(exp1), exp2(exp2){
    type = IF;
}

IFstatement::~IFstatement() {
    delete exp1;
    delete exp2;
}

void IFstatement::execute(EvalState & state)
{
    display_tree();
    int val_exp1 = exp1 -> eval(state);
    int val_exp2 = exp2 -> eval(state);
    bool res = false;
    if (op == '>') {
        res = val_exp1 > val_exp2;
    }
    else if (op == '<') {
        res = val_exp1 < val_exp2;
    }
    else if (op == '=') {
        res = val_exp1 == val_exp2;
    }
    else throw BasicError("UNSUPPORTED OPERATOR");
    if (res) {
        state.setPC(line_number);
    }
}

void IFstatement::display_tree()
{
    QString space = "    ";
    expression * cur_exp = exp1;
    QQueue<expression *> exp_queue;
    int next_len = 0;
    if (exp1 -> isCompound) {
        next_len += 2;
        exp_queue.enqueue( ((compoundexp*)cur_exp) -> getLHS());
        exp_queue.enqueue( ((compoundexp*)cur_exp) -> getRHS());
    }
    QString tree_exp = space + cur_exp -> tree_id;
    GuiBasic::ui_handle -> syn_tree_display(tree_exp);
    QString tree_op = space + op;
    GuiBasic::ui_handle -> syn_tree_display(tree_op);
    cur_exp = exp2;
    if (exp2 -> isCompound) {
        next_len += 2;
        exp_queue.enqueue( ((compoundexp*)cur_exp) -> getLHS());
        exp_queue.enqueue( ((compoundexp*)cur_exp) -> getRHS());
    }
    tree_exp = space + cur_exp -> tree_id;
    GuiBasic::ui_handle -> syn_tree_display(tree_exp);
    QString exp_line_num = space + QString::number(line_number);
    GuiBasic::ui_handle -> syn_tree_display(exp_line_num);
    while (true) {
        space += "    ";
        if (next_len == 0) break;
        int len = next_len;
        next_len = 0;
        while (len--) {
            cur_exp = exp_queue.dequeue();
            if (cur_exp -> isCompound) {
                next_len += 2;
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getLHS());
                exp_queue.enqueue( ((compoundexp*)cur_exp) -> getRHS());
            }
            tree_exp = space + cur_exp -> tree_id;
            GuiBasic::ui_handle -> syn_tree_display(tree_exp);
        }
    }
}
/**** IF end ****/
