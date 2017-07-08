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
#include "network.h"
#include "networkdescriptor.h"
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>

#include <QStringList>
#include <QDebug>

namespace DS
{
  Network::Network()
      : m_fann( NULL ),
      m_trainParam( NULL )
  {}
  Network::~Network()
  {}

  void Network::appendLayer( Layer *value )
  {
    m_layers.append( value );
  }

  void Network::createNetFromDescriptor( NetworkDescriptor *descriptor )
  {
    m_fann = fann_create_from_descr( descriptor->fann_descr() );
    m_trainParam = new TrainingParameters( m_fann );
  }

  void Network::setTrainData( TrainData *data )
  {
    m_trainData = data;
    fann_init_weights( m_fann, m_trainData->fann_train_data() );
  }

  Network::TrainEpochValues Network::trainEpoch()
  {
    if (m_trainData)
    {
      fann_train_epoch( m_fann, m_trainData->fann_train_data() );

      m_trainEpochValues.epoch++;
      m_trainEpochValues.bitFail = m_trainParam->numBitFail();
      m_trainEpochValues.mse = fann_get_MSE( m_fann );
    }

    return m_trainEpochValues;
  }

  Network::TestNetworkValues Network::testNetwork()
  {
    const unsigned int numData = trainData()->numData();
    const unsigned int numOutputs = trainData()->numOutputs();
    fannType *value;

    fann_reset_MSE( m_fann );

    for( uint i = 0; i < numData; i++ )
    {
      value = fann_test( m_fann, trainData()->fann_train_data()->input[i], trainData()->fann_train_data()->output[i] );

      //FIXME
      fannType *outputs = new fannType[numOutputs];
      for( uint j = 0; j < numOutputs; j++ )
        outputs[j] = value[j];

      m_testNetworkValues.outputs << outputs;
      m_testNetworkValues.totalMSEIncrement = m_trainParam->totalMSEValue() - m_testNetworkValues.totalMSE;
      m_testNetworkValues.totalMSE = m_trainParam->totalMSEValue();
    }

    return m_testNetworkValues;
  }


  /*
   * Static Methods
   */
  QStringList Network::ms_activationFunctions;
  QStringList Network::ms_errorFunctions;
  QStringList Network::ms_stopFunctions;
  QStringList Network::ms_trainingAlgorithms;
  QStringList Network::ms_blasActivationFunctions;
  QStringList Network::ms_blasTrainingAlgorithms;

  QStringList Network::activationFunctions()
  {
    /* If this is the first call, populate the QStringList */
    if ( ms_activationFunctions.size() == 0 )
    {
      const int size = sizeof( FANN_ACTIVATIONFUNC_NAMES ) / sizeof( *FANN_ACTIVATIONFUNC_NAMES );
      for ( int i = 0; i < size; i++ )
        ms_activationFunctions << capitalizeString( QString( FANN_ACTIVATIONFUNC_NAMES[i] ).remove( "FANN_" ).replace( "_" , " " ).toLower() );
    }
    return ms_activationFunctions;
  }

  QStringList Network::errorFunctions()
  {
    /* If this is the first call, populate the QStringList */
    if ( ms_errorFunctions.size() == 0 )
    {
      const int size = sizeof( FANN_ERRORFUNC_NAMES ) / sizeof( *FANN_ERRORFUNC_NAMES );
      for ( int i = 0; i < size; i++ )
        ms_errorFunctions << capitalizeString( QString( FANN_ERRORFUNC_NAMES[i] ).remove( "FANN_ERRORFUNC_" ).replace( "_" , " " ).toLower() );
    }
    return ms_errorFunctions;
  }

  QStringList Network::stopFunctions()
  {
    /* If this is the first call, populate the QStringList */
    if ( ms_stopFunctions.size() == 0 )
    {
      const int size = sizeof( FANN_STOPFUNC_NAMES ) / sizeof( *FANN_STOPFUNC_NAMES );
      for ( int i = 0; i < size; i++ )
        ms_stopFunctions << capitalizeString( QString( FANN_STOPFUNC_NAMES[i] ).remove( "FANN_STOPFUNC_" ).replace( "_" , " " ).toLower() );
    }
    return ms_stopFunctions;
  }

  QStringList Network::trainingAlgorithms()
  {
    /* If this is the first call, populate the QStringList */
    if ( ms_trainingAlgorithms.size() == 0 )
    {
      const int size = sizeof( FANN_TRAIN_NAMES ) / sizeof( *FANN_TRAIN_NAMES );
      for ( int i = 0; i < size; i++ )
        ms_trainingAlgorithms << capitalizeString( QString( FANN_TRAIN_NAMES[i] ).remove( "FANN_TRAIN_" ).replace( "_" , " " ).toLower() );
    }
    return ms_trainingAlgorithms;
  }

  QStringList Network::blasActivationFunctions()
  {
    /*
     * FIXME I have to manage associations between activation functions and training algorithm...
     * As I'm doing now, I forget any associations and I can select an activation function non implemented with the selected training algorithm.
     */

    /* If this is the first call, populate the QStringList */
    if ( ms_blasActivationFunctions.size() == 0 )
      populateBlasNames();

    return ms_blasActivationFunctions;
  }

  QStringList Network::blasTrainingAlgorithms()
  {
    /*
    * FIXME I have to manage associations between activation functions and training algorithm...
    * As I'm doing now, I forget any associations and I can select an activation function non implemented with the selected training algorithm.
    */

    /* If this is the first call, populate the QStringList */
    if ( ms_blasTrainingAlgorithms.size() == 0 )
      populateBlasNames();

    return ms_blasTrainingAlgorithms;
  }

  void Network::populateBlasNames()
  {
    const int size = sizeof( FANN_OPTIMIZED_BLAS_IMPLEMENTED_ALGORITHM_NAMES ) / sizeof( *FANN_OPTIMIZED_BLAS_IMPLEMENTED_ALGORITHM_NAMES );
    QString tmp;
    for ( int i = 0; i < size; i++ )
    {
      tmp = capitalizeString( QString( FANN_OPTIMIZED_BLAS_IMPLEMENTED_ALGORITHM_NAMES[i] ).section( " ", 0, 0 ).replace( "_" , " " ).toLower() );
      if( !ms_blasTrainingAlgorithms.contains( tmp ) )
        ms_blasTrainingAlgorithms << tmp;

      tmp = capitalizeString( QString( FANN_OPTIMIZED_BLAS_IMPLEMENTED_ALGORITHM_NAMES[i] ).section( " ", 1, 1 ).replace( "_" , " " ).toLower() );
      if( !ms_blasActivationFunctions.contains( tmp ) )
        ms_blasActivationFunctions << tmp;
    }
  }

  QString Network::capitalizeString( const QString &string )
  {
    QStringList list = string.split( " " );
    QString retValue;
    foreach( QString str, list )
    {
      str[0] = str[0].toUpper();
      retValue += str + " ";
    }
    return retValue.trimmed();
  }
}
