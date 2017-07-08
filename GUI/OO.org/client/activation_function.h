#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include <QWidget>

#include <vector>

namespace FANN { class neural_net; }

#include "ui_activation_function.h"

class QString;

class ActivationFunction : public QWidget, public Ui::ActivationFunction
{
    Q_OBJECT

public:
    ActivationFunction(QWidget *parent=0, FANN::neural_net *net=0, bool isNewNet=true, Qt::WindowFlags flags=0);
    int getGlobalFunction();
    double getGlobalSteepness();
    void saveData();
    void loadData();

    std::vector<int> getPerLayerFunction();
    std::vector<double> getPerLayerSteepness();
    std::vector<std::vector<int> > getIndividualFunction();
    std::vector<std::vector<double> > getIndividualSteepness();

public slots:
    void layersNumberChanged(const int number);
    void neuronsNumberChanged(const int number, const int layer);
    void selectionChanged();
    void valueChanged();

private:
    void fillLayersComboBox(const int hiddenCount, bool addInput=true, bool addOutput=true);
    int getCurrentLayer();
    int getCurrentNeuron();
    void disableControls();

    std::vector<int> neuronNumber;
    int globalFunction;
    double globalSteepness;
    std::vector<int> perLayerFunction;
    std::vector<double> perLayerSteepness;
    std::vector<std::vector<int> > individualFunction;
    std::vector<std::vector<double> > individualSteepness;

    FANN::neural_net *net;
};

#endif  // ACTIVATION_FUNCTION_H
