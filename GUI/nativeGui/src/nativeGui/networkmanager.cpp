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
#include "networkmanager.h"

#include "inputlayeritem.h"
#include "defaultlayeritem.h"

/* FIXME these includes should not be here ...*/
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>
#include "rpropsigmoidoptimizedneuron.h"
#include "rpropsigmoidsymmetricoptimizedneuron.h"
#include "batchsigmoidoptimizedneuron.h"
#include "batchsigmoidsymmetricoptimizedneuron.h"
/* .................................... */

#include <QApplication>
#include <QDebug>

NetworkManager::NetworkManager( AGW::AnnGraphWidget *graph )
    : m_net( NULL ),
    m_graph( graph ),
    m_lastSelectedNeuron( NULL ),
    m_stopTrain( false )
{}
NetworkManager::~NetworkManager()
{}

DS::Network* NetworkManager::net() const
{
  return m_net;
}

void NetworkManager::createNet( DS::NetworkDescriptor *descr )
{
  /* Create the net */
  m_net = new DS::Network();
  descr->setNet( m_net );
  m_net->createNetFromDescriptor( descr );

  /* Draw the net */
  AGW::LayerItem *layerItem;
  AGW::MIMONeuronItem *neuronItem;

  m_graph->clear();
  m_graph->addLayerItem( new AGW::InputLayerItem( descr->numInputs() ) );

  m_neurons.clear();
  m_neuronItems.clear();

  foreach( DS::Layer *layer, m_net->layers() )
  {
    layerItem = new AGW::DefaultLayerItem();
    foreach( DS::Neuron *neuron, layer->neurons() )
    {
      neuronItem = new AGW::MIMONeuronItem( neuron->numOutputs() );
      layerItem->addNeuron( neuronItem );

      /* Cache informations about all neurons and its corresponding neuronItem */
      m_neurons << neuron;
      m_neuronItems << neuronItem;
      connect( neuronItem, SIGNAL( neuronClicked(AGW::MIMONeuronItem *) ), this, SLOT( selectNeuron(AGW::MIMONeuronItem *) ) );
    }
    m_graph->addLayerItem( layerItem );
  }

  emit trainingParametersChanged( m_net->trainParam() );
}

/* This is an overloaded member function, provided for convenience. */
void NetworkManager::createNet( DS::TrainData *trainData )
{
  DS::NetworkDescriptor *netDescr;
  DS::LayerDescriptor *layerDescr;

  netDescr = new DS::NetworkDescriptor( trainData->numInputs() );

  layerDescr = new DS::LayerDescriptor();
  layerDescr->appendNeuronDescriptor( new DS::NeuronDescriptor( 3 ) );
  netDescr->appendLayerDescriptor( layerDescr );

  layerDescr = new DS::LayerDescriptor();
  layerDescr->appendNeuronDescriptor( new DS::NeuronDescriptor( trainData->numOutputs() ) );
  netDescr->appendLayerDescriptor( layerDescr );

  createNet( netDescr );
  m_net->setTrainData( trainData );
  emit trainDataChanged( trainData );
}

void NetworkManager::selectNeuron( AGW::MIMONeuronItem *neuronItem )
{
  /* Select twice the same neuron */
  if ( m_lastSelectedNeuron == neuronItem )
  {
    neuronItem->setHighlight( false );
    m_lastSelectedNeuron = NULL;
    emit neuronSelectionChanged( NULL );
  }
  else
  {
    neuronItem->setHighlight( true );
    if ( m_lastSelectedNeuron )
      m_lastSelectedNeuron->setHighlight( false );
    m_lastSelectedNeuron = neuronItem;
    emit neuronSelectionChanged( m_neurons[ m_neuronItems.indexOf( neuronItem ) ] );
  }
}

void NetworkManager::changeNeuronType( DS::Neuron *neuron, const QString &value )
{
  if( neuron->activationFunction() == value || neuron->trainingAlgorithm() == value )
    return; /*Do nothing*/

  DS::Neuron *newNeuron; /* newNeuron will replace neuron */
  DS::Layer *layer = 0; /* layer where neuron is */
  int idx = -1; /* index of the position of the layer in the QList */

  //find the layer and the neuron position
  foreach ( layer, m_net->layers() )
  {
    idx = layer->neurons().indexOf( neuron );
    if( idx > -1 )
      break;
  }

  struct fann_neuron_descr *descr = new fann_neuron_descr();
  descr->num_outputs = neuron->numOutputs();

  /* FIXME consider to save the weights before calling the destructor */
  struct fann_neuron *fann_neuron;
  neuron->callDestructor();
  fann_neuron = neuron->fann_neuron();
  if( value == "Sigmoid" )
  {
    if( neuron->trainingAlgorithm() == "Rprop" )
    {
      fann_neuron_constructor_blas_rprop_sigmoid( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::RPropSigmoidOptimizedNeuron( fann_neuron );
    }
    else
    {
      fann_neuron_constructor_blas_batch_sigmoid( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::BatchSigmoidOptimizedNeuron( fann_neuron );
    }
  }
  else if( value == "Sigmoid Symmetric" )
  {
    if( neuron->trainingAlgorithm() == "Rprop" )
    {
      fann_neuron_constructor_blas_rprop_sigmoid_symmetric( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::RPropSigmoidSymmetricOptimizedNeuron( fann_neuron );
    }
    else
    {
      fann_neuron_constructor_blas_batch_sigmoid_symmetric( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::BatchSigmoidSymmetricOptimizedNeuron( fann_neuron );
    }
  }
  else if( value == "Rprop" )
  {
    if( neuron->activationFunction() == "Sigmoid" )
    {
      fann_neuron_constructor_blas_rprop_sigmoid( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::RPropSigmoidOptimizedNeuron( fann_neuron );
    }
    else
    {
      fann_neuron_constructor_blas_rprop_sigmoid_symmetric( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::RPropSigmoidSymmetricOptimizedNeuron( fann_neuron );
    }
  }
  else if( value == "Batch" )
  {
    if( neuron->activationFunction() == "Sigmoid" )
    {
      fann_neuron_constructor_blas_batch_sigmoid( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::BatchSigmoidOptimizedNeuron( fann_neuron );
    }
    else
    {
      fann_neuron_constructor_blas_batch_sigmoid_symmetric( m_net->fann(), layer->fann_layer(), fann_neuron, descr );
      newNeuron = new DS::BatchSigmoidSymmetricOptimizedNeuron( fann_neuron );
    }
  }

  layer->replaceNeuron( idx, newNeuron );
  m_neurons.replace( m_neurons.indexOf( neuron ), newNeuron );

  delete descr;
  delete neuron;

  emit neuronTypeChanged( newNeuron );
}

void NetworkManager::startTraining()
{
  emit trainStarted();

  const unsigned int maxEpochs = m_net->trainParam()->maxEpochs();
  const float desiredError = m_net->trainParam()->desiredError();
  const unsigned int numBitFail = m_net->trainParam()->numBitFail();
  DS::Network::TrainEpochValues currValues;

  /* Anticipate one cycle to fill currValues with proper values */
  currValues = m_net->trainEpoch();
  emit epochTrained( currValues );

  while( currValues.mse > desiredError && currValues.epoch <= maxEpochs && currValues.bitFail > numBitFail && !m_stopTrain )
  {
    currValues = m_net->trainEpoch();
    emit epochTrained( currValues );
    qApp->processEvents();
  }

  if( m_stopTrain )
  {
    m_stopTrain = false;
    emit trainStopped();
  }
  else
    emit trainFinished();
}

void NetworkManager::stopTraining()
{
  m_stopTrain = true;
}

void NetworkManager::testNetwork()
{
  emit networkTested( m_net->testNetwork() );
}
