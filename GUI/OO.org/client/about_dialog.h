#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QWidget>

#include "ui_about_dialog.h"

class AboutDialog : public QDialog, public Ui::AboutDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);

};

#endif  // ABOUT_DIALOG_H
