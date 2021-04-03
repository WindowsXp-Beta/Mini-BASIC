#include "guibasic.h"

ProgramLine::ProgramLine(statement * parsed_ast, QString origin_line):
    stmt(parsed_ast), line(origin_line){};

ProgramLine::~ProgramLine(){
    /* Empty */
}

void ProgramLine::show()
{
    GuiBasic::ui_handle -> show_line(line);
}

Program::Program(){
    /* Empty */
}

Program::~Program(){
    code.clear();
}

void Program::addSourceLine(int lineNumber, QString line, statement *parsed_line)
{
    code[lineNumber] = ProgramLine(parsed_line, line);;//若行号重复，直接覆盖
}

void Program::list()
{
    QMap<int, ProgramLine>::iterator p;
    for (p = code.begin(); p != code.end(); p++) {
        p.value().show();
    }
}

void Program::removeSourceLine(int lineNumber){
    QMap<int, ProgramLine>::iterator p = code.find(lineNumber);
    if (p != code.end()) {
        code.erase(p);
    }
}
