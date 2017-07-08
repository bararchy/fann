#ifndef DATA_CHOOSER_H
#define DATA_CHOOSER_H

#include <QWidget>

#include "ui_data_chooser.h"
#include "office_client.h"

class DataChooser : public QDialog, private Ui::DataChooser
{
    Q_OBJECT

public:
    DataChooser(QWidget *parent, Selection &sel, OfficeClient *client, Qt::WindowFlags flags=Qt::Dialog|Qt::WindowStaysOnTopHint);

public slots:
    void assignSpreadsheet();
    void unassignSpreadsheet();

private:
    Selection &sel;
    OfficeClient *client;
};

#endif  // DATA_CHOOSER_H
