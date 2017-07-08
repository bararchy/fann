#include "strict_int_validator.h"

#include <iostream>

StrictIntValidator::StrictIntValidator(int bottom, int top, QObject *parent)
    : QIntValidator(bottom, top, parent)
{
}

QValidator::State StrictIntValidator::validate (QString &input, int &pos) const
{
    QValidator::State state = QIntValidator::validate(input, pos);
    if (state == Intermediate)
    {
       const int value = input.toInt();
       if ((value == 0) && (bottom() > 0))
       {
           state = Invalid;
       }
    }
    return state;
}
