#include <cassert>
#include <exception>
#include <memory>
#include <vector>

#include <QMessageBox>

#include <doublefann.h>
#include <fann_cpp.h>

#include "fann_translation.h"
#include "network_overview.h"
#include "network_console.h"
#include "weights.h"

NetworkOverview::NetworkOverview(QWidget *parent, FANN::neural_net *net, Qt::WindowFlags flags)
      : QWidget(parent, flags), Ui::NetworkOverview(), net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    connect(detailsButton, SIGNAL(clicked(bool)), this, SLOT(showDetails()));
    connect(weightsButton, SIGNAL(clicked(bool)), this, SLOT(showWeights()));

    const unsigned int numLayers = net->get_num_layers();
    networkTypeLabel->setText(FANN::NET_TYPES[net->get_network_type()]);
    connectionRateLabel->setText(QString::number(net->get_connection_rate()));
    layersNumberLabel->setText(QString::number(numLayers));
    totalNeuronsLabel->setText(QString::number(net->get_total_neurons()));
    totalConnectionsLabel->setText(QString::number(net->get_total_connections()));
    inputNeuronsLabel->setText(QString::number(net->get_num_input()));
    outputNeuronsLabel->setText(QString::number(net->get_num_output()));

    unsigned int *neurons = new(std::nothrow) unsigned int[numLayers];
    unsigned int *bias = new(std::nothrow) unsigned int[numLayers];
    if (bias == 0 || neurons == 0) 
    {
        delete [] bias;
        delete [] neurons;
        throw std::bad_alloc();
    }
    net->get_layer_array(neurons);
    net->get_bias_array(bias);
    QString neuronLayers, biasLayers;
    for (unsigned int i = 0; i < numLayers - 1; ++i)
    {
        neuronLayers += QString::number(neurons[i]) + ", ";
        biasLayers += QString::number(bias[i]) + ", ";
    }
    neuronLayers += QString::number(neurons[numLayers-1]);
    biasLayers += QString::number(bias[numLayers-1]);
    delete [] bias;
    delete [] neurons;
    neuronsLabel->setText(neuronLayers);
    biasLabel->setText(biasLayers);
}

void NetworkOverview::showDetails()
{
    assert(!net);
    try 
    {
        NetworkConsole(this, false, net).exec();
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(this, "Critical error", e.what());
    }
}


void NetworkOverview::showWeights()
{
    assert(!net);
    try 
    {
        Weights(this, net).exec();
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(this, "Critical error", e.what());
    }
}
