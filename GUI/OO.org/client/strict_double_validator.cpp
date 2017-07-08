#include "strict_double_validator.h"

StrictDoubleValidator::StrictDoubleValidator(double bottom, double top, int decimals, QObject *parent)
    : QDoubleValidator(bottom, top, decimals, parent)
{

}

QValidator::State StrictDoubleValidator::validate (QString &input, int &pos) const
{
    if (input.isEmpty() || input == ".")
    {
        return Intermediate;
    }
    if (QDoubleValidator::validate(input, pos) != Acceptable)
    {
        return Invalid;
    }
    return Acceptable;
}
