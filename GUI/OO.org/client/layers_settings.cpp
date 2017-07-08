#include <cassert>
#include <iostream>

#include <doublefann.h>
#include <fann_cpp.h>

#include "fann_translation.h"
#include "layers_settings.h"
#include "strict_int_validator.h"
#include "util.h"

LayersSettings::LayersSettings(QWidget *parent, FANN::neural_net *net, bool isNewNet, Qt::WindowFlags flags) :
    QWidget(parent, flags), Ui::LayersSettings(), net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    populateComboBox(networkTypeCombo, FANN::NET_TYPES, "NetworkTypes",  2);
    if (isNewNet == 0)
    {
        loadData();
        disableControls();
    }

    std::auto_ptr<StrictIntValidator> 
       i1(new StrictIntValidator(1, 999, neuronCountEdit)),
       i2(new StrictIntValidator(1, 999, inputNeuronsEdit)),
       i3(new StrictIntValidator(1, 999, outputNeuronsEdit));
    connect(hiddenLayersSpin, SIGNAL(valueChanged(int)),
            this, SLOT(layersNumberChanged(const int)));
    connect(networkTypeCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(networkTypeChanged(int)));
    connect(neuronCountEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(neuronsNumberChanged(const QString&)));
    connect(layerNumberCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(layersChoiceChanged(const int)));
    
    neuronCountEdit->setValidator(i1.release());
    inputNeuronsEdit->setValidator(i2.release());
    outputNeuronsEdit->setValidator(i3.release());
}

void LayersSettings::layersNumberChanged(const int number)
{
    hiddenNeuronsCount.resize(number, 1);
    setIntSequenceComboBox(layerNumberCombo, number);
    
    bool enable = number > 0 ? true : false;
    if (enable != layerNumberCombo->isEnabled() || enable != layerNumberLabel->isEnabled())
    {
        layerNumberCombo->setEnabled(enable);
        layerNumberLabel->setEnabled(enable);
    }
}

void LayersSettings::neuronsNumberChanged(const QString &numberString)
{
    const int number = numberString.toInt();
    if (hiddenNeuronsCount.size() > static_cast<size_t>(layerNumberCombo->currentIndex()))
    {
        hiddenNeuronsCount[layerNumberCombo->currentIndex()] = number;
    }
    assert(number >= 0 && "Illegal program state, negative neuron number");
}

void LayersSettings::layersChoiceChanged(const int number)
{
    assert(number >= 0 && "Illegal program state, negative layer number");
    bool enable = false;
    if (layerNumberCombo->currentIndex() >= 0)
    {
        neuronCountEdit->setText(QString::number(hiddenNeuronsCount[number]));
        enable = true;
    }
    if (enable != neuronCountEdit->isEnabled() || enable != neuronCountLabel->isEnabled())
    {
        neuronCountEdit->setEnabled(enable);
        neuronCountLabel->setEnabled(enable);
    }
}

int LayersSettings::getHiddenLayerNeuronsNum(const int layer)
{
    assert(layer >= 0 && "Illegal program state, negative layer number");
    return hiddenNeuronsCount[layer];
}

void LayersSettings::networkTypeChanged(const int index)
{
    connectionRateSpin->setDisabled(index == 1);
}

// throws std::bad_alloc
void LayersSettings::saveData()
{
    const unsigned int numHidden = hiddenLayersSpin->value();
    const unsigned int numLayers = numHidden + 2;
    const unsigned int numInput  = inputNeuronsEdit->text().toInt();
    const unsigned int numOutput = outputNeuronsEdit->text().toInt();
    unsigned int *layers = new unsigned int[numLayers];
    layers[0] = numInput;
    for (unsigned int i = 0; i < numHidden; ++i)
    {
        layers[1+i] = getHiddenLayerNeuronsNum(i);
    }
    layers[numLayers-1] = numOutput;

    if (networkTypeCombo->currentIndex() == 0)
    {
        net->create_sparse_array(connectionRateSpin->value(), numLayers, layers);
    }
    else if (networkTypeCombo->currentIndex() == 1)
    {
        net->create_shortcut_array(numLayers, layers);
    }
    else
    {
        assert(false && "Unknown network type!");
    }
    delete [] layers;
}

void LayersSettings::disableControls()
{
    networkTypeCombo->setDisabled(true);
    connectionRateSpin->setDisabled(true);
    inputNeuronsEdit->setDisabled(true);
    outputNeuronsEdit->setDisabled(true);
    hiddenLayersSpin->setDisabled(true);
    layerNumberCombo->setDisabled(true);
}

// throws bad_alloc
void LayersSettings::loadData()
{
    assert(net && "Illegal program state, net uninitialized");
    const unsigned int totalLayers = net->get_num_layers();
    unsigned int *neuronsCount = new unsigned int[totalLayers];
    net->get_layer_array(neuronsCount);

    const unsigned int hiddenLayers = totalLayers - 2;
    if (hiddenLayers > 0)
    {
        hiddenNeuronsCount.resize(hiddenLayers);
        for (unsigned int i = 0; i < hiddenLayers; ++i)
        {
            hiddenNeuronsCount[i] = neuronsCount[i+1];
        }
    }
    delete [] neuronsCount;
    hiddenLayersSpin->setValue(hiddenLayers);
    inputNeuronsEdit->setText(QString::number(net->get_num_input()));
    outputNeuronsEdit->setText(QString::number(net->get_num_output()));
}
