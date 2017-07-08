#ifndef NETWORK_OVERVIEW_H
#define NETWORK_OVERVIEW_H

#include <QWidget>

#include "ui_network_overview.h"

namespace FANN { class neural_net; }

class NetworkOverview : public QWidget, private Ui::NetworkOverview
{
    Q_OBJECT

public:
    NetworkOverview(QWidget *parent=0, FANN::neural_net *net=0, Qt::WindowFlags flags=0);

public slots:
    void showDetails();
    void showWeights();

private:
    FANN::neural_net *net;
};

#endif // NETWORK_OVERVIEW_H
