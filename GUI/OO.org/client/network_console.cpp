#include <QVBoxLayout>

#include <iostream>
#include <cassert>
#include <stdexcept>

#include <doublefann.h>
#include <fann_cpp.h>

#include "activation_function.h"
#include "layers_settings.h"
#include "network_console.h"
#include "resilient_propagation.h"

NetworkConsole::NetworkConsole(QWidget *parent, bool createNew, FANN::neural_net *net, Qt::WindowFlags flags)
    : QDialog(parent, flags), Ui::NetworkConsole(), net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    
    if (createNew)
    {
        newNetworkLayout();
    }
    else
    {
        openNetworkLayout();
    }
}

void NetworkConsole::newNetworkLayout()
{
    try 
    {
        createTabs(true);
    }
    catch (std::bad_alloc &e)
    {
        throw std::runtime_error("memory allocation failure in NetworkConsole");
    }

    connect(layersSettings->hiddenLayersSpin, SIGNAL(valueChanged(int)),
            activationFunction, SLOT(layersNumberChanged(const int)));

    connect(layersSettings->inputNeuronsEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(inputNeuronNumberChanged(const QString&)));

    connect(layersSettings->outputNeuronsEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(outputNeuronNumberChanged(const QString&)));

    connect(layersSettings->neuronCountEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(hiddenLayersNeuronNumberChanged(const QString&)));

    connect(okButton, SIGNAL(clicked()), this, SLOT(saveData()));
}

void NetworkConsole::openNetworkLayout()
{
    createTabs(false);
    loadData();
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void NetworkConsole::createTabs(bool isNewNet)
{
    std::auto_ptr<ActivationFunction> af(new ActivationFunction(this, net, isNewNet));
    std::auto_ptr<ResilientPropagation> rp(new ResilientPropagation(this, net, isNewNet));
    std::auto_ptr<LayersSettings> ls;
    std::auto_ptr<QVBoxLayout> 
        afL(new QVBoxLayout()), rpL(new QVBoxLayout()), lsL;
    if (isNewNet)
    {
        ls.reset(new LayersSettings(this, net, isNewNet));
        lsL.reset(new QVBoxLayout());
    }

    activationFunction = af.release();
    QVBoxLayout *activationFunctionLayout = afL.release();
    activationFunctionLayout->addWidget(activationFunction);
    activationFunctionLayout->addStretch();
    activationFunctionTab->setLayout(activationFunctionLayout);

    if (isNewNet)
    {
        layersSettings = ls.release();
        QVBoxLayout *settingsLayout = lsL.release();
        settingsLayout->addWidget(layersSettings);
        settingsLayout->addStretch();
        neuronsTab->setLayout(settingsLayout);
    }
    else
    {
        tabs->removeTab(0);
    }

    resilientPropagation = rp.release();
    QVBoxLayout *resilientPropagationLayout = rpL.release();
    resilientPropagationLayout->addWidget(resilientPropagation);
    resilientPropagationLayout->addStretch();
    resilientPropagationTab->setLayout(resilientPropagationLayout);
}

void NetworkConsole::hiddenLayersNeuronNumberChanged(const QString &numberString)
{
    const int number = numberString.toInt();
    activationFunction->neuronsNumberChanged(
            number, layersSettings->layerNumberCombo->currentIndex() + 1);
    assert(number >= 0 && "Illegal program state, negative neuron number");
}

void NetworkConsole::inputNeuronNumberChanged(const QString &numberString)
{
    const int number = numberString.toInt();
    activationFunction->neuronsNumberChanged(number, 0);
    assert(number >= 0 && "Illegal program state, negative neuron number");
}

void NetworkConsole::outputNeuronNumberChanged(const QString &numberString)
{
    const int number = numberString.toInt();
    activationFunction->neuronsNumberChanged(
            number, layersSettings->layerNumberCombo->count() + 1);
    assert(number >= 0 && "Illegal program state, negative neuron number");
}

void NetworkConsole::saveData()
{
    assert(activationFunction && layersSettings && resilientPropagation && "Illegal program state, frames uninitialized");
    activationFunction->saveData();
    layersSettings->saveData();
    resilientPropagation->saveData();
    hide();
    setResult(QDialog::Accepted);
}

void NetworkConsole::loadData()
{
    activationFunction->loadData();
    resilientPropagation->loadData();
}

void NetworkConsole::setNet(FANN::neural_net *const net)
{
    this->net = net;
}

FANN::neural_net* NetworkConsole::getNet() const
{
    return net;
}
