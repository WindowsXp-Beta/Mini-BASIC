#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H

#include <QString>
#include <QStringList>
#include "error.h"
class Tokenscanner {
public:
    Tokenscanner(QString);
    QString nextToken();
    bool hasMoreTokens();
    void saveToken();
private:
    int index;
    QStringList line_list;
};
#endif // TOKENSCANNER_H
