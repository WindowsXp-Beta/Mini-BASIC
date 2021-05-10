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

bool ProgramLine::isError() {
    return stmt == nullptr;
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
        case INPUTS:{
            ins = QString::number(line) + " INPUTS";
            break;
        }
        case PRINTF:{
            ins = QString::number(line) + " PRINTF";
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
    p = code.begin();
}

Program::~Program()
{
    clear();
    linePosition.clear();
}

void Program::clear()
{
    code.clear();
    linePosition.clear();
}

void Program::addSourceLine(int lineNumber, QString line, statement *parsed_line)
{
    code[lineNumber] = ProgramLine(parsed_line, line);//若行号重复，直接覆盖
}

void Program::removeSourceLine(int lineNumber)
{
    QMap<int, ProgramLine>::iterator pDel = code.find(lineNumber);
    if (pDel != code.end()) {
        code.erase(pDel);
    }
}

void Program::init(){
    p = code.begin();
    cursor = GuiBasic::ui_handle -> get_cursor();
}

void Program::run(EvalState &state)
{
    if (p == code.end()) throw BasicError("THERE ARE NO CODES YET");
    /* 如果cursor不在首行，先褪色*/
    if (cursor.position() > 0) {
        if (!p.value().isError()) {
            QTextBlockFormat format = cursor.blockFormat();
            QColor color(0,0,0,0);
            format.setBackground(color);
            cursor.setBlockFormat(format);
        }
    }
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
            if (p == code.end()) throw BasicError("GOTO AN INVALID LINE");
        }
        state.list();
    }
    GuiBasic::ui_handle -> run_finished();
}

void Program::debug(EvalState & state) {
    if (cursor.position() == 0) {//仅在调用init后cursor位于0处
        if (p == code.end()) throw BasicError("THERE ARE NO CODES YET");
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        if (!p.value().isError()) {
            QTextBlockFormat format;
            format.setProperty(QTextFormat::FullWidthSelection, true);
            QColor color(100,255,100);
            format.setBackground(color);
            cursor.setBlockFormat(format);
        }
    }
    else if(p != code.end()) {
        bool isWrong = p.value().isError();//为判断是否褪色使用
        /* 先执行一行语句 */
        state.setPC(EvalState::SEQUENTIAL);
        try {
            p.value().execute(state, p.key());
        }  catch (BasicError err) {
            GuiBasic::ui_handle -> error_display(err.get_err_meg());
        }
        int PC = state.getPC();
        if ( PC == EvalState::HALT) {
            p = code.end();
        }
        else if (PC == EvalState::SEQUENTIAL) {
            p++;
        }
        else {
            p = code.find(PC);
            if (p == code.end()) throw BasicError("GOTO AN INVALID LINE");
        }
        state.list();//更新变量显示
        /* 再褪色 */
        if (!isWrong) {
            QTextBlockFormat format = cursor.blockFormat();
            QColor color(0,0,0,0);
            format.setBackground(color);
            cursor.setBlockFormat(format);
        }
        if (p == code.end()) {
            GuiBasic::ui_handle -> run_finished();
            return;
        }
        /* 再染下一行 */
        cursor.setPosition(linePosition[p.key()]);
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
        if (!p.value().isError()) {
            QTextBlockFormat format;
            format.setProperty(QTextFormat::FullWidthSelection, true);
            QColor color(100,255,100);
            format.setBackground(color);
            cursor.setBlockFormat(format);
        }
    }
}

void Program::list()
{
    linePosition.clear();
    QMap<int, ProgramLine>::iterator p;
    QTextCursor cursor = GuiBasic::ui_handle -> get_cursor();
    for (p = code.begin(); p != code.end(); p++) {
        p.value().show();
        cursor.movePosition(QTextCursor::End);
        cursor.movePosition(QTextCursor::StartOfLine);
        linePosition[p.key()] = cursor.position();
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        QTextBlockFormat format;
        format.setProperty(QTextFormat::FullWidthSelection, true);
        QColor color(0,0,0,0);
        if (p.value().isError()) {
            color.setRgb(255, 100, 100);
            color.setAlpha(255);
        }
        format.setBackground(color);
        cursor.setBlockFormat(format);
    }
}
