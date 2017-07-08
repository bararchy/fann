#include <QString>

#include <cassert>
#include <vector>
#include <iostream>
#include <utility>

#include <doublefann.h>
#include <fann_cpp.h>

#include "activation_function.h"
#include "fann_translation.h"
#include "util.h"

const int DEFAULT_NEURON_NUM = 1;

ActivationFunction::ActivationFunction(QWidget* parent, FANN::neural_net *net, bool isNewNet, Qt::WindowFlags flags) 
    :   QWidget(parent, flags), Ui::ActivationFunction(),
        neuronNumber(2, DEFAULT_NEURON_NUM),
        globalFunction(0), globalSteepness(0.5), perLayerFunction(2), perLayerSteepness(2, 0.5),
        individualFunction(2, std::vector<int>(DEFAULT_NEURON_NUM)),
        individualSteepness(2, std::vector<double>(DEFAULT_NEURON_NUM, 0.5)),
        net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    if (isNewNet == 0)
    {
        disableControls();
        loadData();
    }
    connect(selectedLayerCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(selectionChanged()));
    connect(selectedNeuronCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(selectionChanged()));
    connect(globalButton, SIGNAL(clicked(bool)),
            this, SLOT(selectionChanged()));
    connect(perLayerButton, SIGNAL(clicked(bool)),
            this, SLOT(selectionChanged()));
    connect(individualButton, SIGNAL(clicked(bool)),
            this, SLOT(selectionChanged()));

    connect(activationFunctionCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(valueChanged()));
    connect(steepnessSpin, SIGNAL(valueChanged(double)),
            this, SLOT(valueChanged()));

    fillLayersComboBox(0);
    populateComboBox(activationFunctionCombo, FANN::ACTIVATION_FUNCTIONS,
        "ActivationFunctions", 18);
    setIntSequenceComboBox(selectedNeuronCombo, DEFAULT_NEURON_NUM);
}

void ActivationFunction::disableControls()
{
    individualButton->setChecked(true);
    globalButton->hide();
    perLayerButton->hide();
    individualButton->hide();
    settingsLabel->hide();
    activationFunctionCombo->setDisabled(true);
    steepnessSpin->setDisabled(true);
}

void ActivationFunction::layersNumberChanged(const int hiddenLayerNumber)
{
    neuronNumber.resize(hiddenLayerNumber + 2, DEFAULT_NEURON_NUM);
    perLayerFunction.resize(hiddenLayerNumber + 2);
    perLayerSteepness.resize(hiddenLayerNumber + 2, 0.5);
    individualFunction.resize(hiddenLayerNumber + 2, std::vector<int>(DEFAULT_NEURON_NUM));
    individualSteepness.resize(hiddenLayerNumber + 2, std::vector<double>(DEFAULT_NEURON_NUM, 0.5));
    fillLayersComboBox(hiddenLayerNumber);
}

void ActivationFunction::neuronsNumberChanged(const int number, const int layer)
{
    assert(static_cast<size_t>(layer) < neuronNumber.size() && "Layer number too big!");
    neuronNumber[layer] = number;
    if (selectedLayerCombo->currentIndex() == layer)
    {
        setIntSequenceComboBox(selectedNeuronCombo, number);
    }
    individualFunction[layer].resize(number);
    individualSteepness[layer].resize(number);
}

void ActivationFunction::selectionChanged()
{
    const int layer = getCurrentLayer();
    
    assert(static_cast<size_t>(layer) < neuronNumber.size() && "Layer number too big!");
    setIntSequenceComboBox(selectedNeuronCombo, neuronNumber[layer]);

    if (globalButton->isChecked())
    {
        activationFunctionCombo->setCurrentIndex(globalFunction);
        steepnessSpin->setValue(globalSteepness);
    }
    else if (perLayerButton->isChecked())
    {
        assert(static_cast<size_t>(layer) < perLayerFunction.size());
        assert(static_cast<size_t>(layer) < perLayerSteepness.size());

        activationFunctionCombo->setCurrentIndex(perLayerFunction[layer]);
        steepnessSpin->setValue(perLayerSteepness[layer]);
    }
    else
    {
        const int neuron = getCurrentNeuron();

        assert(individualButton->isChecked() && "Unknown selection!");
        assert(static_cast<size_t>(layer) < individualFunction.size());
        assert(static_cast<size_t>(neuron) < individualFunction[layer].size());
        assert(static_cast<size_t>(layer) < individualSteepness.size());
        assert(static_cast<size_t>(neuron) < individualSteepness[layer].size());

        activationFunctionCombo->setCurrentIndex(individualFunction[layer][neuron]);
        steepnessSpin->setValue(individualSteepness[layer][neuron]);
    }
}

void ActivationFunction::valueChanged()
{
    const int function = activationFunctionCombo->currentIndex();
    const double steepness = steepnessSpin->value();
    if (globalButton->isChecked())
    {
        globalFunction = function;
        globalSteepness = steepness;
    }
    else if (perLayerButton->isChecked())
    {
        const int layer = getCurrentLayer();

        assert(static_cast<size_t>(layer) < perLayerFunction.size());
        assert(static_cast<size_t>(layer) < perLayerSteepness.size());
        perLayerFunction[layer] = function;
        perLayerSteepness[layer] = steepness;
    }
    else
    {
        const int layer = getCurrentLayer();
        const int neuron = getCurrentNeuron();

        assert(individualButton->isChecked() && "Unknown selection!");
        assert(static_cast<size_t>(layer) < individualFunction.size());
        assert(static_cast<size_t>(neuron) < individualFunction[layer].size());
        assert(static_cast<size_t>(layer) < individualSteepness.size());
        assert(static_cast<size_t>(neuron) < individualSteepness[layer].size());
        individualFunction[layer][neuron] = function;
        individualSteepness[layer][neuron] = steepness;
    }
}

int ActivationFunction::getCurrentNeuron()
{
        int neuron = selectedNeuronCombo->currentIndex();
        return (neuron == -1) ? 0 : neuron;
}

int ActivationFunction::getCurrentLayer()
{
        int layer = selectedLayerCombo->currentIndex();
        return (layer == -1) ? 0 : layer;
}

void ActivationFunction::fillLayersComboBox(const int hiddenCount, bool addInput, bool addOutput)
{
    assert(hiddenCount >= 0 && "Invalid program state, negative hiddenCount");
    int selection = 0;
    if (selectedLayerCombo->currentIndex() >= 0)
    {
        selection = selectedLayerCombo->currentIndex();
    }
    selectedLayerCombo->clear();
    if (addInput)
    {
        selectedLayerCombo->addItem(tr("Input layer"));
    }
    for (int i = 0; i < hiddenCount; ++i)
    {
        selectedLayerCombo->addItem(tr(" Hidden layer no. ") + QChar('1' + i));
    }
    if (addOutput)
    {
        selectedLayerCombo->addItem(tr("Output layer"));
    }
    selection = std::min(selection, selectedLayerCombo->count() - 1);
    selectedLayerCombo->setCurrentIndex(selection);
}

int ActivationFunction::getGlobalFunction()
{
    return globalFunction;
}

double ActivationFunction::getGlobalSteepness()
{
    return globalSteepness;
}

std::vector<int> ActivationFunction::getPerLayerFunction()
{
    return perLayerFunction;
}

std::vector<double> ActivationFunction::getPerLayerSteepness()
{
    return perLayerSteepness;
}

std::vector<std::vector<int> > ActivationFunction::getIndividualFunction()
{
    return individualFunction;
}

std::vector<std::vector<double> > ActivationFunction::getIndividualSteepness()
{
    return individualSteepness;
}

void ActivationFunction::saveData()
{
    assert(net && "Invalid program state, net uninitialized");
    if (globalButton->isChecked())
    {
        net->set_activation_function_hidden(FANN::activation_function_enum(globalFunction));
        net->set_activation_function_output(FANN::activation_function_enum(globalFunction));
        net->set_activation_steepness_hidden(globalSteepness);
        net->set_activation_steepness_output(globalSteepness);
    }
    else if (perLayerButton->isChecked())
    {
        assert(perLayerFunction.size() == perLayerSteepness.size());
        for (size_t i = 1; i < perLayerFunction.size(); ++i)
        {
            net->set_activation_function_layer(FANN::activation_function_enum(perLayerFunction[i]), i);
            net->set_activation_steepness_layer(perLayerSteepness[i], i);
        }
    }
    else
    {
        assert(individualButton->isChecked());
        assert(individualFunction.size() == individualSteepness.size());
        for (size_t i = 1; i < individualFunction.size(); ++i)
        {
            assert(individualFunction[i].size() == individualSteepness[i].size());
            for (size_t j = 0; j < individualFunction[i].size(); ++j)
            {
                net->set_activation_function(FANN::activation_function_enum(individualFunction[i][j]), i, j);
                net->set_activation_steepness(individualSteepness[i][j], i, j);
            }
        }
    }
}

// throws bad_alloc
void ActivationFunction::loadData()
{
    assert(net && "Invalid program state, net uninitialized");
    const unsigned int layers = net->get_num_layers();
    assert(layers >= 2);
    layersNumberChanged(layers - 2);
    unsigned int *neuronsCount = new unsigned int[layers];
    net->get_layer_array(neuronsCount);

    for (unsigned int i = 1; i < layers; ++i)
    {
        neuronsNumberChanged(neuronsCount[i], i);
        for (unsigned int j = 0; j < neuronsCount[i]; ++j)
        {
            individualFunction[i][j] = net->get_activation_function(i, j);
            individualSteepness[i][j] = net->get_activation_steepness(i, j);
        }
    }
    selectedLayerCombo->setCurrentIndex(0);
    selectedNeuronCombo->setCurrentIndex(0);
    delete [] neuronsCount;
}
