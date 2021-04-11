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
    return 0;//if op = '=' | '>' | '<' | 'THEN' return 0
}

//line_list is QStringList;index is the begin index of expression
expression *readT(QStringList &line_list, int &index){
    if (index == line_list.size()) throw BasicError("THE COMPOUND EXPRESSION IS WRONG");
    bool is_num;
    QString token = line_list.at(index);
    int const_num = token.toInt(&is_num, 10);
    expression *exp;
    if (is_num) {
         exp = new constantexp(const_num);
         index++;
    }
    else if (token == '(') {
        index++;//skip the '('
        exp = readE(line_list, index);
        index++;//skip the ')'
    }
    else {
        if (is_keyword(token)) throw BasicError("NAME OF VARIABLE CANNOT BE A KEY WORD");
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

expression *parseEXP(QStringList &line_list, int &index){
    expression *exp = readE(line_list, index);
    return exp;
}


statement *parsedirect(QStringList &cmd_list){
    QString fun = cmd_list.at(0);
    if (fun == "LET") return parseLET(cmd_list);
    if (fun == "INPUT") return parseINPUT(cmd_list);
    if (fun == "PRINT") return parsePRINT(cmd_list);
    throw BasicError("INVALID DIRECTIVE INSTRUCTION");
}

statement *parsestatement(QStringList &cmd_list){
    QString fun = cmd_list.at(1);
    if (fun == "LET") return parseLET(cmd_list);
    if (fun == "INPUT") return parseINPUT(cmd_list);
    if (fun == "PRINT") return parsePRINT(cmd_list);
    if (fun == "GOTO") return parseGOTO(cmd_list);
    if (fun == "IF") return parseIF(cmd_list);
    if (fun == "REM") return parseREM(cmd_list);
    if (fun == "END") return parseEND();
    throw BasicError("INVALID INSTRUCTION");
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


//(num) REM ...
REMstatement *parseREM(QStringList &line_list){
    QString init_anno;
    for (int i = 2; i < line_list.size(); i++) {
        init_anno += (line_list[i] + " ");
    }
    REMstatement *new_rem_stat = new REMstatement(init_anno);
    return new_rem_stat;
}

//num GOTO n
GOTOstatement *parseGOTO(QStringList &cmd_list){
    int line_num = cmd_list.at(2).toInt();
    GOTOstatement *new_goto_stat = new GOTOstatement(line_num);
    return new_goto_stat;
}

//num END
ENDstatement *parseEND(){
    ENDstatement *new_end_stat = new ENDstatement();
    return new_end_stat;
}

//num IF exp1 op exp2 THEN n
IFstatement *parseIF(QStringList &cmd_list) {
    int index_exp1 = 2;
    int index_exp2;
    int ln;
    expression *exp1 = parseEXP(cmd_list, index_exp1);//此时index_exp1应为op的index
    QString op = cmd_list.at(index_exp1);
    index_exp2 = index_exp1 + 1;
    expression *exp2 = parseEXP(cmd_list, index_exp2);//此时index_exp2应为THEN
    ln = cmd_list.at(index_exp2 + 1).toInt();
    IFstatement *new_if_stat = new IFstatement(ln, op, exp1, exp2);
    return new_if_stat;
}
