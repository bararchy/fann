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
#include "trainingparameters.h"
#include <fann/fann.h>

namespace DS
{
  TrainingParameters::TrainingParameters( struct fann *value )
      : m_fann_training_params( value->training_params ),
      m_desiredError( 0 ),
      m_maxEpochs( 100 )
  {}
  TrainingParameters::~TrainingParameters()
  {}

//   int TrainingParameters::algorithm() const
//   {
//     return m_fann_training_params->training_algorithm;
//   }
//   void TrainingParameters::setAlgorithm( int value )
//   {
//     m_fann_training_params->training_algorithm = static_cast<enum fann_train_enum>( value );
//   }

  fannType TrainingParameters::bitFailLimit() const
  {
    return m_fann_training_params->bit_fail_limit;
  }
  void TrainingParameters::setBitFailLimit( fannType value )
  {
    m_fann_training_params->bit_fail_limit = value;
  }

  unsigned int TrainingParameters::numBitFail() const
  {
    return m_fann_training_params->num_bit_fail;
  }
  void TrainingParameters::setNumBitFail( unsigned int value )
  {
    m_fann_training_params->num_bit_fail = value;
  }

  float TrainingParameters::desiredError() const
  {
    return m_desiredError;
  }
  void TrainingParameters::setDesiredError( float value )
  {
    m_desiredError = ( value >= 0 ) ? value : 0;
  }

  unsigned int TrainingParameters::maxEpochs() const
  {
    return m_maxEpochs;
  }
  void TrainingParameters::setMaxEpochs( unsigned int value )
  {
    m_maxEpochs = value;
  }

  int TrainingParameters::errorFunction() const
  {
    return m_fann_training_params->train_error_function;
  }
  void TrainingParameters::setErrorFunction( int value )
  {
    m_fann_training_params->train_error_function = static_cast<enum fann_errorfunc_enum>( value );
  }

  int TrainingParameters::stopFunction() const
  {
    return m_fann_training_params->train_stop_function;
  }
  void TrainingParameters::setStopFunction( int value )
  {
    m_fann_training_params->train_stop_function = static_cast<enum fann_stopfunc_enum>( value );
  }

  float TrainingParameters::totalMSEValue() const
  {
    return m_fann_training_params->MSE_value;
  }
}
