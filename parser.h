#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>
#include <string>
#include "statement.h"
#include "error.h"
#include "tokenscanner.h"

/* overall parse */
statement *parsedirect(Tokenscanner & scanner);

statement *parsestatement(Tokenscanner & scanner);
/* overall parse end */

bool isQuote(QString s);
/* parse instruction */
LETstatement *parseLET(Tokenscanner & scanner);

PRINTstatement *parsePRINT(Tokenscanner & scanner);

INPUTstatement *parseINPUT(Tokenscanner & scanner);

REMstatement *parseREM(Tokenscanner & scanner);

GOTOstatement *parseGOTO(Tokenscanner & scanner);

ENDstatement *parseEND(Tokenscanner & scanner);

IFstatement *parseIF(Tokenscanner & scanner);

INPUTSstatement *parseINPUTS(Tokenscanner & scanner);

PRINTFstatement *parsePRINTF(Tokenscanner & scanner);

/* parse direction end */

QString parseString(Tokenscanner & scanner);

expression *parseEXP(Tokenscanner & scanner);

expression *readE(Tokenscanner & scanner, int prec = 0);

expression *readT(Tokenscanner & scanner);

#endif // PARSER_H
