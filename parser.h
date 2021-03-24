#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>
#include <string>
#include "statement.h"

//直接执行的语句
statement *parsedirect(QString &line);

statement *parsestatement(QString &line);

LETstatement *parseLET(QString &line);

PRINTstatement *parsePRINT(QString &line);

//INPUTstatement *parseINPUT(QString &line);

//REMstatement *parseREM(QString &line);

expression *parseEXP(QString &line);

#endif // PARSER_H
