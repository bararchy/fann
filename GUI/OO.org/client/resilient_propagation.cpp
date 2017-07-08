#include <cassert>

#include <doublefann.h>
#include <fann_cpp.h>

#include "resilient_propagation.h"

ResilientPropagation::ResilientPropagation(QWidget *parent, FANN::neural_net *net, bool isNewNet, Qt::WindowFlags flags) 
    : QWidget(parent, flags), Ui::ResilientPropagation(), net(net)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi(this);
    if (!isNewNet)
    {
        disableControls();
    }
}

void ResilientPropagation::saveData()
{
    assert(net && "Illegal program state, net uninitialized");
    net->set_rprop_increase_factor(increaseFactorSpin->value());
    net->set_rprop_decrease_factor(decreaseFactorSpin->value());
    net->set_rprop_delta_min(minDeltaSpin->value());
    net->set_rprop_delta_max(maxDeltaSpin->value());
}

void ResilientPropagation::loadData()
{
    increaseFactorSpin->setValue(net->get_rprop_increase_factor());
    decreaseFactorSpin->setValue(net->get_rprop_decrease_factor());
    minDeltaSpin->setValue(net->get_rprop_delta_min());
    maxDeltaSpin->setValue(net->get_rprop_delta_max());
}

void ResilientPropagation::disableControls()
{

    increaseFactorSpin->setDisabled(true);
    decreaseFactorSpin->setDisabled(true);
    minDeltaSpin->setDisabled(true);
    maxDeltaSpin->setDisabled(true);
}
