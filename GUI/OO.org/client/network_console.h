#ifndef NETWORK_DETAILS_H
#define NETWORK_DETAILS_H

#include <QDialog>

#include <doublefann.h>
#include <fann_cpp.h>

#include "ui_network_console.h"

class ActivationFunction;
class LayersSettings;
class ResilientPropagation;

class NetworkConsole : public QDialog, private Ui::NetworkConsole
{
    Q_OBJECT

public:
    NetworkConsole(QWidget *parent=0, bool createNew=true, FANN::neural_net *net=0, Qt::WindowFlags flags=0);
    void setNet(FANN::neural_net* const net);
    FANN::neural_net *getNet() const;

public slots:
    void hiddenLayersNeuronNumberChanged(const QString &numberString);
    void inputNeuronNumberChanged(const QString& numberString);
    void outputNeuronNumberChanged(const QString& numberString);

private slots:
    void loadData();
    void saveData();

private:
    void newNetworkLayout();
    void openNetworkLayout();
    void createTabs(bool newNet);

    ActivationFunction *activationFunction;
    LayersSettings *layersSettings;
    ResilientPropagation *resilientPropagation;

    FANN::neural_net *net;
};

#endif // NETWORK_DETAILS_H
