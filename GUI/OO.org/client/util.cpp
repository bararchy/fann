#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QWidget>

#include <cassert>

#include "data_chooser.h"
#include "office_client.h"
#include "util.h"

void populateComboBox(QComboBox *combo, char const* const tab[], char const* const context, const int tabSize)
{
    assert(combo && context && tabSize >= 0 && "Illegal program state");
    for (int i = 0; i < tabSize; i++)
    {
        combo->addItem(qApp->translate(context, tab[i]));
    }
}

void setIntSequenceComboBox(QComboBox *combo, const int limit)
{
    assert(combo && limit >= 0 && "Illegal program state");
    if (combo->count() > limit)
    {
        do
        {
            combo->removeItem(combo->count() - 1);
        } while (combo->count() > limit);
    }
    else
    {
        while (combo->count() < limit)
        {
            combo->addItem(QString::number(combo->count() + 1));
        }
    }
}
