/***************************************************************************
 *   Copyright (C) 2007 by Yusef Maali                                     *
 *   contact@yusefmaali.net                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QList>

#include "network.h"
#include "networkdescriptor.h"
#include "anngraphwidget.h"

class NetworkManager : public QObject
{
    Q_OBJECT

  public:
    NetworkManager( AGW::AnnGraphWidget *graph );
    ~NetworkManager();

    DS::Network* net() const;
    void createNet( DS::NetworkDescriptor *descr );
    void createNet( DS::TrainData *trainData );

  public slots:
    /* Make neuronItem highlighted */
    void selectNeuron( AGW::MIMONeuronItem *neuronItem );
    void changeNeuronType( DS::Neuron *neuron, const QString &value );
    void startTraining();
    void stopTraining();
    void testNetwork();

  signals:
    void neuronSelectionChanged( DS::Neuron *neuron ); /* Emitted when selectNeuron() is called */
    void neuronTypeChanged( DS::Neuron *neuron );
    void trainingParametersChanged( DS::TrainingParameters *trainParam );
    void trainDataChanged( DS::TrainData *trainData );
    void epochTrained( DS::Network::TrainEpochValues epochValue );
    void trainFinished();
    void trainStopped();
    void trainStarted();
    void networkTested( DS::Network::TestNetworkValues );

  protected:
    DS::Network *m_net;
    AGW::AnnGraphWidget *m_graph;

    /* Used to cache information about DS::Neuron and AGW::MIMONeuronItem
     * They are useful to look for correspondence between the two classes */
    QList<DS::Neuron *> m_neurons;
    QList<AGW::MIMONeuronItem *> m_neuronItems;

  private:
    AGW::MIMONeuronItem *m_lastSelectedNeuron;
    bool m_stopTrain;
};

#endif
