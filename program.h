#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMap>
#include <QString>
#include <QTextEdit>
#include <QObject>
#include "statement.h"
#include "evalstate.h"

class ProgramLine {
public:
    ProgramLine(statement * parsed_ast = nullptr, QString origin_line = "");
    ~ProgramLine();
    //execute the line
    void execute(EvalState & state, const int);
    //Show the line
    void show();
    bool isError();
private:
    std::shared_ptr<statement> stmt;//智能指针，会自动delete
    QString line;
};

class Program: public QObject {
   Q_OBJECT
public:
   Program();
   ~Program();
   void clear();
   void addSourceLine(int lineNumber, QString line, statement * parsed_line);
   void removeSourceLine(int lineNumber);//若仅有行号，则删除该行
   void run(EvalState & state);
   void list();
public slots:
   void debug(EvalState & state,int frequency);
private:
   //the map to store the code
   QMap<int, ProgramLine> code;
   QMap<int, int> linePosition;
   QTextCursor cursor;
   QMap<int, ProgramLine>::iterator p;
};
#endif // PROGRAM_H
