#include "parser.h"

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
    return false;
}

int precedence(QString &op){
    if (op == '+' || op == '-') return 2;
    if (op == '*' || op == '/') return 3;
}

//line_list is QStringList;index is the begin index of expression
expression *readT(QStringList &line_list, int &index){
    bool is_num;
    QString token = line_list.at(index);
    int const_num = token.toInt(&is_num, 10);
    expression *exp;
    if (is_num) {
         exp = new constantexp(const_num);
         index++;
    }
    else if (token == '(') {
        index++;//skip the (
        exp = readE(line_list, index);
        index++;//skip the )
    }
    else {
        exp = new identifierexp(token);
        index++;
    }
    return exp;
}

expression *readE(QStringList &line_list, int &index, int prec){
    expression *exp = readT(line_list, index);
    while (index < line_list.size()) {
        QString op = line_list.at(index);
        if (op == ')') break;
        int new_prec = precedence(op);
        if (new_prec <= prec) break;
        index++;
        expression *rhc = readE(line_list, index, new_prec);
        exp = new compoundexp(op, exp, rhc);
    }
    return exp;
}

expression *parseEXP(QStringList &line_list, int index){
    expression *exp = readE(line_list, index);
    return exp;
}


statement *parsedirect(QString &line){
    QStringList cmd_list = line.split(' ', Qt::SkipEmptyParts);
    QString fun = cmd_list.at(0);
    if (fun == "LET") return parseLET(cmd_list);
    if (fun == "INPUT") return parseINPUT(cmd_list);
    if (fun == "PRINT") return parsePRINT(cmd_list);
}


//(num) LET var = exp
LETstatement *parseLET(QStringList &line_list){
    int index_var = 2;//index of var
    int index_exp = 4;//index of exp
    bool isNumber;
    line_list.at(0).toInt(&isNumber, 10);
    if (!isNumber) {//if dont have a line number
        index_var = index_var - 1;
        index_exp = index_exp - 1;
    }
    QString var_str = line_list.at(index_var);
    expression *exp = parseEXP(line_list, index_exp);
    LETstatement * new_let_stat = new LETstatement(var_str, exp);
    return new_let_stat;
}

//(num) PRINT exp
PRINTstatement *parsePRINT(QStringList &line_list){
    int index_exp = 2;
    bool isNumber;
    line_list.at(0).toInt(&isNumber, 10);
    if (!isNumber) {//if dont have a line number
        index_exp = index_exp - 1;
    }
    expression *exp = parseEXP(line_list, index_exp);
    PRINTstatement *new_print_stat = new PRINTstatement(exp);
    return new_print_stat;
}

//(num) INPUT x
INPUTstatement *parseINPUT(QStringList &line_list){
    int index_var = 2;
    bool isNumber;
    line_list.at(0).toInt(&isNumber, 10);
    if (!isNumber) {//if dont have a line number
        index_var = index_var - 1;
    }
    QString var = line_list.at(index_var);
    INPUTstatement *new_input_stat = new INPUTstatement(var);
    return new_input_stat;
}
