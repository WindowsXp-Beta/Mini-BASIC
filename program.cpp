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

void ProgramLine::execute(EvalState &state, const int line)
{
    if (stmt) {
        QString ins;
        switch (stmt -> type) {
        case LET:{
            ins = QString::number(line) + " LET =";
            break;
        }
        case REM:{
            ins = QString::number(line) + " REM";
            break;
        }
        case PRINT:{
            ins = QString::number(line) + " PRINT";
            break;
        }
        case END:{
            ins = QString::number(line) + " END";
            break;
        }
        case INPUT:{
            ins = QString::number(line) + " INPUT";
            break;
        }
        case GOTO:{
            ins = QString::number(line) + " GOTO";
            break;
        }
        case IF:{
            ins = QString::number(line) + " IF THEN";
            break;
        }
        }
        GuiBasic::ui_handle -> syn_tree_display(ins);
        stmt -> execute(state);
    }
    else {
        QString err_meg = QString::number(line) + " Error";
        GuiBasic::ui_handle -> syn_tree_display(err_meg);
    }
}

Program::Program()
{
    /* Empty */
}

Program::~Program()
{
    /* Empty */
}

void Program::clear()
{
    code.clear();
}

void Program::addSourceLine(int lineNumber, QString line, statement *parsed_line)
{
    code[lineNumber] = ProgramLine(parsed_line, line);//若行号重复，直接覆盖
}

void Program::removeSourceLine(int lineNumber)
{
    QMap<int, ProgramLine>::iterator p = code.find(lineNumber);
    if (p != code.end()) {
        code.erase(p);
    }
}

void Program::run(EvalState &state)
{
    QMap<int, ProgramLine>::iterator p = code.begin();
    while (p != code.end()) {
        state.setPC(EvalState::SEQUENTIAL);
        try {
            p.value().execute(state, p.key());
        }  catch (BasicError err) {
            GuiBasic::ui_handle -> error_display(err.get_err_meg());
        }

        int PC = state.getPC();
        if ( PC == EvalState::HALT) {
            break;
        }
        else if (PC == EvalState::SEQUENTIAL) {
            p++;
        }
        else {
            p = code.find(PC);
        }
    }
}

void Program::list()
{
    QMap<int, ProgramLine>::iterator p;
    for (p = code.begin(); p != code.end(); p++) {
        p.value().show();
    }
}

