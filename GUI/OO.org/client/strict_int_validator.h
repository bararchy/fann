#ifndef STRICT_INT_VALIDATOR_H
#define STRICT_INT_VALIDATOR_H

#include <QIntValidator>
#include <QValidator>

class StrictIntValidator : public QIntValidator
{
public:
    StrictIntValidator(int bottom, int top, QObject *parent = 0);
    QValidator::State validate(QString &input, int &pos) const;
};

#endif // STRICT_INT_VALIDATOR_H
