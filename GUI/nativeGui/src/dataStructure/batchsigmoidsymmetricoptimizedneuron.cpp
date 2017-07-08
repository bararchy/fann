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
#include "batchsigmoidsymmetricoptimizedneuron.h"
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>

namespace DS
{
  BatchSigmoidSymmetricOptimizedNeuron::BatchSigmoidSymmetricOptimizedNeuron( struct fann_neuron *value )
      : Neuron( value )
  {
    m_fann_backprop_params = ( static_cast<struct fann_neuron_private_data_blas_batch_sigmoid_symmetric *>( value->private_data ) )->backprop_params;
  }
  BatchSigmoidSymmetricOptimizedNeuron::~BatchSigmoidSymmetricOptimizedNeuron()
  {}

  float BatchSigmoidSymmetricOptimizedNeuron::learningRate() const
  {
    return m_fann_backprop_params->learning_rate;
  }
  void BatchSigmoidSymmetricOptimizedNeuron::setLearningRate( float value )
  {
    m_fann_backprop_params->learning_rate = value;
  }

  float BatchSigmoidSymmetricOptimizedNeuron::learningMomentum() const
  {
    return m_fann_backprop_params->learning_momentum;
  }
  void BatchSigmoidSymmetricOptimizedNeuron::setLearningMomentum( float value )
  {
    m_fann_backprop_params->learning_momentum = value;
  }

  QString BatchSigmoidSymmetricOptimizedNeuron::activationFunction()
  {
    return QString( "Sigmoid Symmetric" );
  }
  QString BatchSigmoidSymmetricOptimizedNeuron::trainingAlgorithm()
  {
    return QString( "Batch" );
  }
}
