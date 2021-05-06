#include "evalstate.h"

EvalState::EvalState() {
    program_counter = SEQUENTIAL;
}

EvalState::~EvalState() {
   clear();
}

void EvalState :: setValue(QString name, int value) {
    symboltableNum.insert(name, value);
    symboltableStr.remove(name);
}

void EvalState::setValue(QString var, QString value) {
    symboltableStr.insert(var, value);
    symboltableNum.remove(var);
}


int EvalState::getValue(QString var) {
    return symboltableNum.find(var).value();
}

QString EvalState::getStr(QString var)
{
    return symboltableStr.find(var).value();
}

bool EvalState::isDefined(QString var) {
   if (symboltableNum.count(var)) return true;
   else return false;
}

bool EvalState::isDefinedStr(QString var) {
    if(symboltableStr.count(var)) return true;
    else return false;
}

void EvalState::setPC(int line_number)
{
    program_counter = line_number;
}

int EvalState::getPC()
{
    return program_counter;
}

void EvalState::clear()
{
    symboltableNum.clear();
    symboltableStr.clear();
    program_counter = SEQUENTIAL;
}
