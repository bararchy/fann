#include <QFileDialog>
#include <QMessageBox>

#include <cassert>

#include "data_chooser.h"
#include "office_client.h"


DataChooser::DataChooser(QWidget *parent, Selection& sel, OfficeClient *client, Qt::WindowFlags flags) :
  QDialog(parent, flags), Ui::DataChooser(), sel(sel), client(client)
{
    assert(client && "Illegal program state, client uninitialized");
    setupUi(this);
    connect(assignButton, SIGNAL(clicked(bool)), this, SLOT(assignSpreadsheet()));
    connect(unassignButton, SIGNAL(clicked(bool)), this, SLOT(unassignSpreadsheet()));
}

void DataChooser::assignSpreadsheet()
{
    assert(client && "Illegal program state, client uninitialized");
    if (client->fillSelection(sel) == 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No valid selection found"));
    }
    close();
}

void DataChooser::unassignSpreadsheet()
{
    sel.setValid(false);
    close();
}
