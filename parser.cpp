#include "parser.h"

bool isQuote(QString s) {
    if (s == '\'' || s == '\"') return true;
    else return false;
}

bool is_keyword(QString id)
{
    //static Lexicon keyword("BasicKeywords.txt");
    //stop use lexicon because it is not case-sensitive
    if (id == "QUIT") return true;
    if (id == "LIST") return true;
    if (id == "RUN") return true;
    if (id == "CLEAR") return true;
    if (id == "HELP") return true;
    if (id == "IF") return true;
    if (id == "THEN") return true;
    if (id == "GOTO") return true;
    if (id == "REM") return true;
    if (id == "LET") return true;
    if (id == "PRINT") return true;
    if (id == "INPUT") return true;
    if (id == "END") return true;
    if (id == "INPUTS") return true;
    if (id == "PRINTF") return true;
    return false;
}

int precedence(QString &op){
    if (op == "+" || op == "-") return 2;
    if (op == "*" || op == "/") return 3;
    if (op == "**") return 4;
    return 0;//if op = '=' | '>' | '<' | 'THEN' return 0
}

expression *readT(Tokenscanner & scanner){
    bool is_num, is_neg = false;
    QString token = scanner.nextToken();
    //处理数字前面的符号
    int const_num;
    if (token == '+') {
        is_neg = false;
        token = scanner.nextToken();
    }
    else if (token == '-') {
        is_neg = true;
        token = scanner.nextToken();
    }

    const_num = token.toInt(&is_num);
    expression *exp;
    if (is_num) {
        if (is_neg) const_num = -const_num;
        exp = new constantexp(const_num);
    }
    else if (token == '(') {
        exp = readE(scanner);
        if (scanner.nextToken() != ")") throw BasicError("BRACKETS NOT CLOSED");
    }
    else if (token == ')') throw BasicError("BRACKETS NOT CLOSED");
    else {
        if (is_keyword(token)) throw BasicError("NAME OF VARIABLE CANNOT BE A KEY WORD");
        exp = new identifierexp(token);
    }
    return exp;
}

expression *readE(Tokenscanner & scanner, int prec){
    expression *exp = readT(scanner);
    while (scanner.hasMoreTokens()) {
        QString op = scanner.nextToken();
        int new_prec = precedence(op);
        if (new_prec <= prec && !(new_prec == 4 && prec == 4)) break;
        expression *rhc = readE(scanner, new_prec);
        exp = new compoundexp(op, exp, rhc);
    }
    if (scanner.hasMoreTokens()) scanner.saveToken();
    return exp;
}

QString parseString(Tokenscanner & scanner) {
    QString firstQuote = scanner.nextToken();
    QStringList result;
    QString tmp = scanner.nextToken();
    while(!isQuote(tmp)) {
        result.append(tmp);
        tmp = scanner.nextToken();
    }
    if (tmp != firstQuote) throw BasicError("THE STRING SHOULD BE CLOSED IN THE SAME QUOTE");
    return result.join(' ');
}

expression *parseEXP(Tokenscanner & scanner){
    expression *exp;
    if (isQuote(scanner.nextToken())) {
        scanner.saveToken();
        exp = new stringexp(parseString(scanner));
    }
    else {
        scanner.saveToken();
        exp = readE(scanner);
    }
    return exp;
}


statement *parsedirect(Tokenscanner &scanner) {
    QString fun = scanner.nextToken();
    if (fun == "LET") return parseLET(scanner);
    if (fun == "INPUT") return parseINPUT(scanner);
    if (fun == "PRINT") return parsePRINT(scanner);
    if (fun == "INPUTS") return parseINPUTS(scanner);
    if (fun == "PRINTF") return parsePRINTF(scanner);
    throw BasicError("INVALID DIRECTIVE INSTRUCTION");
}

statement *parsestatement(Tokenscanner &scanner){
    QString fun = scanner.nextToken();
    if (fun == "LET") return parseLET(scanner);
    if (fun == "INPUT") return parseINPUT(scanner);
    if (fun == "PRINT") return parsePRINT(scanner);
    if (fun == "GOTO") return parseGOTO(scanner);
    if (fun == "IF") return parseIF(scanner);
    if (fun == "REM") return parseREM(scanner);
    if (fun == "END") return parseEND(scanner);
    if (fun == "INPUTS") return parseINPUTS(scanner);
    if (fun == "PRINTF") return parsePRINTF(scanner);
    throw BasicError("INVALID INSTRUCTION");
}

//(num) LET var = exp
LETstatement *parseLET(Tokenscanner &scanner){
    QString var_str = scanner.nextToken();
    if (is_keyword(var_str)) throw BasicError("NAME OF VARIABLE CANNOT BE A KEY WORD");
    if (scanner.nextToken() != "=") throw BasicError("INVALID LET STATEMENT");
    expression *exp = parseEXP(scanner);
    if (scanner.hasMoreTokens()) throw BasicError("INVALID LET STATEMENT");
    LETstatement * new_let_stat = new LETstatement(var_str, exp);
    return new_let_stat;
}

//(num) PRINT exp
PRINTstatement *parsePRINT(Tokenscanner & scanner){
    expression *exp = parseEXP(scanner);
    if(exp -> type == strexp) throw BasicError("YOU SHOULD USE PRINTF");
    if (scanner.hasMoreTokens()) throw BasicError("INVALID PRINT STATEMENT");
    PRINTstatement *new_print_stat = new PRINTstatement(exp);
    return new_print_stat;
}

//(num) PRINTF "... N * {}", "...", "...", "...",
PRINTFstatement *parsePRINTF(Tokenscanner & scanner){
    QStringList *result = new QStringList;
    QQueue<int> *mark = new QQueue<int>;
    QQueue<expression*> *expQueue = new QQueue<expression*>;
    /* parse templete string */
    QString firstQuote = scanner.nextToken();
    QString tmp = scanner.nextToken();
    while(!isQuote(tmp)) {
        result -> append(tmp);
        if (tmp == "{}") mark -> enqueue(result -> size() - 1);
        tmp = scanner.nextToken();
    }
    if (tmp != firstQuote) throw BasicError("THE STRING SHOULD BE CLOSED IN THE SAME QUOTE");
    int markSize = mark -> size();
    expression* newExp;
    while(markSize && scanner.nextToken() == ",") {
        newExp = parseEXP(scanner);
        expQueue -> enqueue(newExp);
        markSize--;
    }
    if (markSize) throw BasicError("YOU SHOULD USE COMMA AS DELIMITER");
    if (scanner.hasMoreTokens()) throw BasicError("INVALID PRINTF STATEMENT");
    PRINTFstatement* new_printf_stat = new PRINTFstatement(result, mark, expQueue);
    return new_printf_stat;
}

//(num) INPUT x
INPUTstatement *parseINPUT(Tokenscanner & scanner){
    QString var = scanner.nextToken();
    if (scanner.hasMoreTokens()) throw BasicError("INVALID INPUT STATEMENT");
    INPUTstatement *new_input_stat = new INPUTstatement(var);
    return new_input_stat;
}

//(num) REM ...
REMstatement *parseREM(Tokenscanner & scanner){
    QString init_anno;
    while(scanner.hasMoreTokens()) init_anno += scanner.nextToken();
    REMstatement *new_rem_stat = new REMstatement(init_anno);
    return new_rem_stat;
}

//num GOTO n
GOTOstatement *parseGOTO(Tokenscanner & scanner){
    bool isNum = false;
    int line_num = scanner.nextToken().toInt(&isNum);
    if (!isNum) throw BasicError("LINE NUMBER MUST BE AN INTEGER");
    if (scanner.hasMoreTokens()) throw BasicError("INVALID GOTO STATEMENT");
    GOTOstatement *new_goto_stat = new GOTOstatement(line_num);
    return new_goto_stat;
}

//num END
ENDstatement *parseEND(Tokenscanner & scanner){
    ENDstatement *new_end_stat = new ENDstatement();
    if (scanner.hasMoreTokens()) throw BasicError("INVALID END STATEMENT");
    return new_end_stat;
}

//num IF exp1 op exp2 THEN n
IFstatement *parseIF(Tokenscanner & scanner) {
    int ln;
    expression *exp1 = parseEXP(scanner);
    QString op = scanner.nextToken();
    if (op != ">" && op != "<" && op != "=") throw BasicError("INVALID IF STATEMENT");
    expression *exp2 = parseEXP(scanner);
    if (scanner.nextToken() != "THEN") throw BasicError("INVALID IF STATEMENT");
    bool isNum;
    ln = scanner.nextToken().toInt(&isNum);
    if (!isNum) throw BasicError("LINE NUMBER MUST BE AN INTEGER");
    if (scanner.hasMoreTokens()) throw BasicError("INVALID IF STATEMENT");
    IFstatement *new_if_stat = new IFstatement(ln, op, exp1, exp2);
    return new_if_stat;
}

//(num)INPUTS x
INPUTSstatement *parseINPUTS(Tokenscanner & scanner) {
    QString var = scanner.nextToken();
    if(scanner.hasMoreTokens()) throw BasicError("INVALID INPUTS statement");
    INPUTSstatement *new_inputs_stat = new INPUTSstatement(var);
    return new_inputs_stat;
}
