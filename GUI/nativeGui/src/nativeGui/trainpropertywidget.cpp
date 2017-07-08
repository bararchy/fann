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
#include "trainpropertywidget.h"
#include "network.h"

TrainPropertyWidget::TrainPropertyWidget( QWidget* parent, Qt::WFlags fl )
    : QWidget( parent, fl ), Ui::TrainProperty(),
    m_lastTrainParam( NULL )
{
  setupUi( this );
  stackedWidget->setCurrentIndex( 0 );
  cmbErrorFunction->addItems( DS::Network::errorFunctions() );
  cmbStopFunction->addItems( DS::Network::stopFunctions() );
}
TrainPropertyWidget::~TrainPropertyWidget()
{}

void TrainPropertyWidget::updateValues( DS::TrainingParameters *trainParam )
{
  m_lastTrainParam = trainParam;

  if ( trainParam )
  {
    stackedWidget->setCurrentIndex( 1 );
    cmbErrorFunction->setCurrentIndex( trainParam->errorFunction() );
    cmbStopFunction->setCurrentIndex( trainParam->stopFunction() );
    spinBitFailLimit->setValue( trainParam->bitFailLimit() );
    spinNumBitFail->setValue( trainParam->numBitFail() );
    spinDesiredError->setValue( trainParam->desiredError() );
    spinMaxEpochs->setValue( trainParam->maxEpochs() );
  }
  else
  {
    stackedWidget->setCurrentIndex( 0 );
  }
}

void TrainPropertyWidget::on_cmbErrorFunction_currentIndexChanged( int index )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setErrorFunction( index );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}

void TrainPropertyWidget::on_cmbStopFunction_currentIndexChanged( int index )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setStopFunction( index );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}

void TrainPropertyWidget::on_spinBitFailLimit_valueChanged( double value )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setBitFailLimit( static_cast<float>( value ) );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}

void TrainPropertyWidget::on_spinNumBitFail_valueChanged( int value )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setNumBitFail( value );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}

void TrainPropertyWidget::on_spinMaxEpochs_valueChanged( int value )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setMaxEpochs( value );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}

void TrainPropertyWidget::on_spinDesiredError_valueChanged( double value )
{
  if ( m_lastTrainParam )
  {
    m_lastTrainParam->setDesiredError( static_cast<float>( value ) );
    emit trainingParametersChanged( m_lastTrainParam );
  }
}
