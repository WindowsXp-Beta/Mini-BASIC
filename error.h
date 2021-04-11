#ifndef ERROR_H
#define ERROR_H

#include <QString>

class BasicError {
public:
    BasicError(QString Meg = ""): err_meg(Meg){};
    QString get_err_meg() { return err_meg; }
private:
    QString err_meg;
};
#endif // ERROR_H
