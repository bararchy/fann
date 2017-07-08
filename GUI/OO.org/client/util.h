#ifndef UTIL_H
#define UTIL_H


class QComboBox;
class QWidget;

void populateComboBox(QComboBox *combo, char const *const tab[], char const *const context, const int tabSize);

void setIntSequenceComboBox(QComboBox *combo, const int limit);

#endif // UTIL_H
