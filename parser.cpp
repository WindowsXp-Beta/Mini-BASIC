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

//目前只能处理常量和变量的exp
expression *parseEXP(QString &exp){
    bool is_num;
    int num = exp.toInt(&is_num, 10);
    if (is_num) {
        constantexp *new_exp = new constantexp(num);
        return new_exp;
    }
    else {
        identifierexp *new_exp = new identifierexp(exp);
        return new_exp;
    }
}


statement *parsedirect(QString &line){
    QString fun = line.split(' ').at(0);
    if (fun == "LET") return parseLET(line);
    //if (fun == "INPUT") return parseINPUT(line);
    if (fun == "PRINT") return parsePRINT(line);
}


//(num) LET var = exp
LETstatement *parseLET(QString &line){
    int index_var = 2;//index of var
    int index_exp = 4;//index of exp
    bool isNumber;
    QStringList line_list = line.split(' ');
    line_list.at(0).toInt(&isNumber, 10);
    if (!isNumber) {//if dont have a line number
        index_var = index_var - 1;
        index_exp = index_exp - 1;
    }
    QString var_str = line_list.at(index_var);
    QString exp_str = line_list.at(index_exp);
    expression *exp = parseEXP(exp_str);
    LETstatement * new_let_stat = new LETstatement(var_str, exp);
    return new_let_stat;
}

//(num) PRINT exp
PRINTstatement *parsePRINT(QString &line){
    int index_exp = 2;
    bool isNumber;
    QStringList line_list = line.split(' ');
    line_list.at(0).toInt(&isNumber, 10);
    if (!isNumber) {//if dont have a line number
        index_exp = index_exp - 1;
    }
    QString exp_str = line_list.at(index_exp);
    expression *exp = parseEXP(exp_str);
    PRINTstatement *new_print_stat = new PRINTstatement(exp);
    return new_print_stat;
}
