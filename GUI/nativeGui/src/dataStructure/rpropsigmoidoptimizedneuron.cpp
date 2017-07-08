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
#include "rpropsigmoidoptimizedneuron.h"
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>

namespace DS
{
  RPropSigmoidOptimizedNeuron::RPropSigmoidOptimizedNeuron( struct fann_neuron *value )
      : Neuron( value )
  {
    m_fann_rprop_params = ( static_cast<struct fann_neuron_private_data_blas_rprop_sigmoid *>( value->private_data ) )->rprop_params;
  }
  RPropSigmoidOptimizedNeuron::~RPropSigmoidOptimizedNeuron()
  {}

  float RPropSigmoidOptimizedNeuron::increaseFactor() const
  {
    return m_fann_rprop_params->rprop_increase_factor;
  }
  void RPropSigmoidOptimizedNeuron::setIncreaseFactor( float value )
  {
    m_fann_rprop_params->rprop_increase_factor = value;
  }

  float RPropSigmoidOptimizedNeuron::decreaseFactor() const
  {
    return m_fann_rprop_params->rprop_decrease_factor;
  }
  void RPropSigmoidOptimizedNeuron::setDecreaseFactor( float value )
  {
    m_fann_rprop_params->rprop_decrease_factor = value;
  }

  float RPropSigmoidOptimizedNeuron::deltaMin() const
  {
    return m_fann_rprop_params->rprop_delta_min;
  }
  void RPropSigmoidOptimizedNeuron::setDeltaMin( float value )
  {
    m_fann_rprop_params->rprop_delta_min = value;
  }

  float RPropSigmoidOptimizedNeuron::deltaMax() const
  {
    return m_fann_rprop_params->rprop_delta_max;
  }
  void RPropSigmoidOptimizedNeuron::setDeltaMax( float value )
  {
    m_fann_rprop_params->rprop_delta_max = value;
  }

  float RPropSigmoidOptimizedNeuron::deltaZero() const
  {
    return m_fann_rprop_params->rprop_delta_zero;
  }
  void RPropSigmoidOptimizedNeuron::setDeltaZero( float value )
  {
    m_fann_rprop_params->rprop_delta_zero = value;
  }

  QString RPropSigmoidOptimizedNeuron::activationFunction()
  {
    return QString( "Sigmoid" );
  }
  QString RPropSigmoidOptimizedNeuron::trainingAlgorithm()
  {
    return QString( "Rprop" );
  }
}
