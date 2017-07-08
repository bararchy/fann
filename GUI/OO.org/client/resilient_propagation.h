#ifndef RESILIENT_PROPAGATION_H
#define RESILIENT_PROPAGATION_H

#include <QWidget>

#include "ui_resilient_propagation.h"

namespace FANN { class neural_net; }

class ResilientPropagation : public QWidget, public Ui::ResilientPropagation
{
    Q_OBJECT

public:
    ResilientPropagation(QWidget* parent=0, FANN::neural_net *net=0, bool newNet=true, Qt::WindowFlags flags=0);
    void saveData();
    void loadData();

private:
    void disableControls();

    FANN::neural_net *net;
};

#endif  // RESILIENT_PROPAGATION_H
