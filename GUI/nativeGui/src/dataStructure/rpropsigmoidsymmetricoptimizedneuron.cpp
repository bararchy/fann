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
#include "rpropsigmoidsymmetricoptimizedneuron.h"
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>

namespace DS
{
  RPropSigmoidSymmetricOptimizedNeuron::RPropSigmoidSymmetricOptimizedNeuron( struct fann_neuron *value )
      : Neuron( value )
  {
    m_fann_rprop_params = ( static_cast<struct fann_neuron_private_data_blas_rprop_sigmoid_symmetric *>( value->private_data ) )->rprop_params;
  }
  RPropSigmoidSymmetricOptimizedNeuron::~RPropSigmoidSymmetricOptimizedNeuron()
  {}

  float RPropSigmoidSymmetricOptimizedNeuron::increaseFactor() const
  {
    return m_fann_rprop_params->rprop_increase_factor;
  }
  void RPropSigmoidSymmetricOptimizedNeuron::setIncreaseFactor( float value )
  {
    m_fann_rprop_params->rprop_increase_factor = value;
  }

  float RPropSigmoidSymmetricOptimizedNeuron::decreaseFactor() const
  {
    return m_fann_rprop_params->rprop_decrease_factor;
  }
  void RPropSigmoidSymmetricOptimizedNeuron::setDecreaseFactor( float value )
  {
    m_fann_rprop_params->rprop_decrease_factor = value;
  }

  float RPropSigmoidSymmetricOptimizedNeuron::deltaMin() const
  {
    return m_fann_rprop_params->rprop_delta_min;
  }
  void RPropSigmoidSymmetricOptimizedNeuron::setDeltaMin( float value )
  {
    m_fann_rprop_params->rprop_delta_min = value;
  }

  float RPropSigmoidSymmetricOptimizedNeuron::deltaMax() const
  {
    return m_fann_rprop_params->rprop_delta_max;
  }
  void RPropSigmoidSymmetricOptimizedNeuron::setDeltaMax( float value )
  {
    m_fann_rprop_params->rprop_delta_max = value;
  }

  float RPropSigmoidSymmetricOptimizedNeuron::deltaZero() const
  {
    return m_fann_rprop_params->rprop_delta_zero;
  }
  void RPropSigmoidSymmetricOptimizedNeuron::setDeltaZero( float value )
  {
    m_fann_rprop_params->rprop_delta_zero = value;
  }

  QString RPropSigmoidSymmetricOptimizedNeuron::activationFunction()
  {
    return QString( "Sigmoid Symmetric" );
  }
  QString RPropSigmoidSymmetricOptimizedNeuron::trainingAlgorithm()
  {
    return QString( "Rprop" );
  }
}
