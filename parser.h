#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>
#include <string>
#include "statement.h"

//直接执行的语句
statement *parsedirect(QString &line);

statement *parsestatement(QString &line);

LETstatement *parseLET(QStringList &line_list);

PRINTstatement *parsePRINT(QStringList &line_list);

INPUTstatement *parseINPUT(QStringList &line_list);

REMstatement *parseREM(QString &line);

expression *parseEXP(QString &line);

expression *readE(QStringList &line_list, int &index, int prec = 0);

expression *readT(QStringList &line_list, int &index);
#endif // PARSER_H
