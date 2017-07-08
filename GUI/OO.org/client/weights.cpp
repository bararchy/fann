#include <cassert>
#include <memory>

#include <QCloseEvent>
#include <QTableWidgetItem>

#include <doublefann.h>
#include <fann_cpp.h>

#include "weights.h"

// throws bad_alloc
Weights::Weights(QWidget *parent, FANN::neural_net *net, Qt::WindowFlags flags) :
  QDialog(parent, flags), Ui::Weights(), net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    long count = net->get_total_connections();
    connections = new FANN::connection[count];
    net->get_connection_array(connections);
    table->setRowCount(count);
    for (int i = 0; i < count; ++i)
    {
        try 
        {
            std::auto_ptr<QTableWidgetItem> 
                i1(new QTableWidgetItem(QString::number(connections[i].from_neuron))),
                i2(new QTableWidgetItem(QString::number(connections[i].to_neuron))),
                i3(new QTableWidgetItem(QString::number(connections[i].weight)));
            
            table->setItem(i, 0, i1.release());
            table->setItem(i, 1, i2.release());
            table->setItem(i, 2, i3.release());
        } 
        catch (std::bad_alloc &e)
        {
            delete [] connections;
            throw;
        }
    }
}

void Weights::closeEvent(QCloseEvent*)
{
    delete [] connections;
}
