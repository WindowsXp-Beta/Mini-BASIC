#include "tokenscanner.h"

bool isOp(QChar c) {
    if (c == '(' || c == ')' || c == '=' || c == '+' || c == '-' ||c == '*' || c == '/' )return true;
    return false;
}

Tokenscanner::Tokenscanner(QString aline) {
    for (int i = 0; i < aline.size(); i++) {
        if (isOp(aline[i])) {
            if (i == 0) continue;//忽略行首的符号
            if (i && aline[i - 1] != ' ') {
                aline.insert(i, ' ');
                i++;
            }
            if (aline[i] == '*' && aline[i + 1] == '*'){
                if (aline[i + 2] != ' '){
                    aline.insert(i + 2, ' ');
                    i+=2;
                    continue;
                }
            }
            if (aline[i + 1] != ' ') {
                aline.insert(i + 1, ' ');
                i++;
            }
        }
    }
    line_list = aline.split(' ', Qt::SkipEmptyParts);
    index = 0;
}

QString Tokenscanner::nextToken() {
    if (index >= line_list.size()) throw BasicError("SYNTAX ERROR");
    return line_list[index++];
}

bool Tokenscanner::hasMoreTokens() {
    if (index >= line_list.size()) return false;
    else return true;
}

void Tokenscanner::saveToken() {
    index--;
}
