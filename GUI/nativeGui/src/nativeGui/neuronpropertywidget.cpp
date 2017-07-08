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
#include "neuronpropertywidget.h"
#include "network.h"

/* FIXME this includes should not be here */
#include "batchsigmoidoptimizedneuron.h"
#include "batchsigmoidsymmetricoptimizedneuron.h"
#include "rpropsigmoidoptimizedneuron.h"
#include "rpropsigmoidsymmetricoptimizedneuron.h"

NeuronPropertyWidget::NeuronPropertyWidget( QWidget *parent, Qt::WFlags fl )
    : QWidget( parent, fl ), Ui::NeuronProperty(),
    m_lastNeuron( NULL )
{
  setupUi( this );
  stackedWidget->setCurrentIndex( 0 );
  cmbActivationFunction->addItems( DS::Network::blasActivationFunctions() );
  cmbTrainAlgorithm->addItems( DS::Network::blasTrainingAlgorithms() );
  stackedTrainParam->setCurrentIndex( 0 );
}
NeuronPropertyWidget::~NeuronPropertyWidget()
{}

void NeuronPropertyWidget::updateValues( DS::Neuron *neuron )
{
  m_lastNeuron = neuron;

  if ( neuron )
  {
    stackedWidget->setCurrentIndex( 1 );
    lblNumNeurons->setText( QString().setNum( neuron->numOutputs() ) );
    lblNumInputs->setText( QString().setNum( neuron->numInputs() ) );
    lblNumWeights->setText( QString().setNum( neuron->numWeights() ) );
    spinSteepness->setValue( neuron->activationSteepness() );
    cmbActivationFunction->setCurrentIndex( cmbActivationFunction->findText( neuron->activationFunction() ) );
    cmbTrainAlgorithm->setCurrentIndex( cmbTrainAlgorithm->findText( neuron->trainingAlgorithm() ) );
    stackedTrainParam->setCurrentIndex( cmbTrainAlgorithm->currentIndex() );

    if( stackedTrainParam->currentIndex() == 0 )
    {
      if( neuron->activationFunction() == "Sigmoid" )
      {
        DS::BatchSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidOptimizedNeuron *>( neuron );
        spinLearningMomentum->setValue( tmp->learningMomentum() );
        spinLearningRate->setValue( tmp->learningRate() );
      }
      else
      {
        DS::BatchSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidSymmetricOptimizedNeuron *>( neuron );;
        spinLearningMomentum->setValue( tmp->learningMomentum() );
        spinLearningRate->setValue( tmp->learningRate() );
      }
    }
    else
    {
      if( neuron->activationFunction() == "Sigmoid" )
      {
        DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( neuron );
        spinIncreaseFactor->setValue( tmp->increaseFactor() );
        spinDecreaseFactor->setValue( tmp->decreaseFactor() );
        spinDeltaMin->setValue( tmp->deltaMin() );
        spinDeltaMax->setValue( tmp->deltaMax() );
        spinDeltaZero->setValue( tmp->deltaZero() );
      }
      else
      {
        DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( neuron );;
        spinIncreaseFactor->setValue( tmp->increaseFactor() );
        spinDecreaseFactor->setValue( tmp->decreaseFactor() );
        spinDeltaMin->setValue( tmp->deltaMin() );
        spinDeltaMax->setValue( tmp->deltaMax() );
        spinDeltaZero->setValue( tmp->deltaZero() );
      }
    }
  }
  else
  {
    stackedWidget->setCurrentIndex( 0 );
  }
}

void NeuronPropertyWidget::on_spinSteepness_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    m_lastNeuron->setActivationSteepness( static_cast<DS::fannType>( value ) );
    emit neuronParametersChanged( m_lastNeuron );
  }
}

void NeuronPropertyWidget::on_cmbActivationFunction_currentIndexChanged( int index )
{
  if ( m_lastNeuron )
    emit changeNeuronTypeRequested( m_lastNeuron, cmbActivationFunction->itemText( index ) );
}

void NeuronPropertyWidget::on_cmbTrainAlgorithm_currentIndexChanged( int index )
{
  if ( m_lastNeuron )
    emit changeNeuronTypeRequested( m_lastNeuron, cmbTrainAlgorithm->itemText( index ) );
}

void NeuronPropertyWidget::on_spinLearningMomentum_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::BatchSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setLearningMomentum( static_cast<float>( value ) );
    }
    else
    {
      DS::BatchSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setLearningMomentum( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
void NeuronPropertyWidget::on_spinLearningRate_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::BatchSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setLearningRate( static_cast<float>( value ) );
    }
    else
    {
      DS::BatchSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::BatchSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setLearningRate( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}

void NeuronPropertyWidget::on_spinIncreaseFactor_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setIncreaseFactor( static_cast<float>( value ) );
    }
    else
    {
      DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setIncreaseFactor( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
void NeuronPropertyWidget::on_spinDecreaseFactor_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setDecreaseFactor( static_cast<float>( value ) );
    }
    else
    {
      DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setDecreaseFactor( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
void NeuronPropertyWidget::on_spinDeltaMin_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setDeltaMin( static_cast<float>( value ) );
    }
    else
    {
      DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setDeltaMin( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
void NeuronPropertyWidget::on_spinDeltaMax_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setDeltaMax( static_cast<float>( value ) );
    }
    else
    {
      DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setDeltaMax( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
void NeuronPropertyWidget::on_spinDeltaZero_valueChanged( double value )
{
  if ( m_lastNeuron )
  {
    if( m_lastNeuron->activationFunction() == "Sigmoid" )
    {
      DS::RPropSigmoidOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidOptimizedNeuron *>( m_lastNeuron );
      tmp->setDeltaZero( static_cast<float>( value ) );
    }
    else
    {
      DS::RPropSigmoidSymmetricOptimizedNeuron *tmp = dynamic_cast<DS::RPropSigmoidSymmetricOptimizedNeuron *>( m_lastNeuron );;
      tmp->setDeltaZero( static_cast<float>( value ) );
    }
    emit neuronParametersChanged( m_lastNeuron );
  }
}
