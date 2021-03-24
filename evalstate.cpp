#include "evalstate.h"

EvalState::EvalState() {
    program_counter = SEQUENTIAL;
}

EvalState::~EvalState() {
   /* Empty */
}

void EvalState :: setValue(QString name, int value) {
    symboltable.insert(name, value);
}

int EvalState::getValue(QString var) {
   return symboltable.find(var).value();
}

bool EvalState::isDefined(QString var) {
   if (symboltable.count(var)) {
        return true;
   }
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
    symboltable.clear();
    program_counter = SEQUENTIAL;
}
