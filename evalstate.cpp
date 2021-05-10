#include "evalstate.h"
#include "guibasic.h"

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

void EvalState::list()
{
    GuiBasic::ui_handle -> clear_var();
    QString varLine;
    QMap<QString, int>::iterator intP = symboltableNum.begin();
    while (intP != symboltableNum.end()) {
        varLine = intP.key() + ": INT = " + QString::number(intP.value());
        GuiBasic::ui_handle -> show_var(varLine);
        intP++;
    }
    QMap<QString, QString>::iterator strP = symboltableStr.begin();
    while (strP != symboltableStr.end()) {
        varLine = strP.key() + ": STR = " + "\"" + strP.value() + "\"";
        GuiBasic::ui_handle -> show_var(varLine);
        strP++;
    }
}
