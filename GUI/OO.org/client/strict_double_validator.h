#ifndef STRICT_DOUBLE_VALIDATOR_H
#define STRICT_DOUBLE_VALIDATOR_H

#include <QDoubleValidator>
#include <QValidator>

class StrictDoubleValidator : public QDoubleValidator
{
public:
    StrictDoubleValidator(double bottom, double top, int decimals, QObject *parent = 0);
    QValidator::State validate(QString &input, int &pos) const;
};

#endif // STRICT_DOUBLE_VALIDATOR_H
