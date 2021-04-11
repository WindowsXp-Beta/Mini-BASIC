#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>
#include <string>
#include "statement.h"
#include "error.h"

/* overall parse */
statement *parsedirect(QStringList &cmd_list);

statement *parsestatement(QStringList &cmd_list);
/* overall parse end */


/* parse instruction */
LETstatement *parseLET(QStringList &line_list);

PRINTstatement *parsePRINT(QStringList &line_list);

INPUTstatement *parseINPUT(QStringList &line_list);

REMstatement *parseREM(QStringList &line_list);

GOTOstatement *parseGOTO(QStringList &cmd_list);

ENDstatement *parseEND();

IFstatement *parseIF(QStringList &cmd_list);

/* parse direction end */


expression *parseEXP(QString &line);

expression *readE(QStringList &line_list, int &index, int prec = 0);

expression *readT(QStringList &line_list, int &index);

#endif // PARSER_H
