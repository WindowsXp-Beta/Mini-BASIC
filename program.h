#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMap>
#include <QString>
#include "statement.h"
#include "evalstate.h"

class ProgramLine {
public:
    //ProgramLine(){}
    ProgramLine(statement * parsed_ast = nullptr, QString origin_line = "");
    ~ProgramLine();
    //excute the line
    void execute(EvalState & state);
    //Show the line
    void show();
private:
    //used smart pointer to handle the AST
    statement* stmt;
    QString line;
};

class Program {
public:
   Program();
   ~Program();
   void clear();
   void addSourceLine(int lineNumber, QString line, statement * parsed_line);
   void removeSourceLine(int lineNumber);//若仅有行号，则删除该行
   void run(EvalState & state);
   void list();

private:
   //the map to store the code
   QMap<int, ProgramLine> code;

};
#endif // PROGRAM_H
