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
#include "neuron.h"
#include <fann/fann.h>

namespace DS
{
  Neuron::Neuron( struct fann_neuron *value )
      : m_fann_neuron( value )
  {}
  Neuron::~Neuron()
  {}

  struct fann_neuron* Neuron::fann_neuron() const
  {
    return m_fann_neuron;
  }

  unsigned int Neuron::numInputs() const
  {
    return m_fann_neuron->num_inputs;
  }
  unsigned int Neuron::numOutputs() const
  {
    return m_fann_neuron->num_outputs;
  }
  unsigned int Neuron::numSimpleNeurons() const
  {
    return numOutputs();
  }
  unsigned int Neuron::numWeights() const
  {
    return m_fann_neuron->num_weights;
  }

  /* FIXME valid only for non optimized neuron */
//   int Neuron::activationFunction() const
//   {
//     return m_fann_neuron->activation_function;
//   }
//   void Neuron::setActivationFunction( int index )
//   {
//     m_fann_neuron->activation_function = static_cast<enum fann_activationfunc_enum>( index );
//   }

  fannType Neuron::activationSteepness() const
  {
    return m_fann_neuron->activation_steepness;
  }
  void Neuron::setActivationSteepness( fannType steepness )
  {
    m_fann_neuron->activation_steepness = steepness;
  }

  void Neuron::callDestructor()
  {
    m_fann_neuron->destructor( m_fann_neuron );
  }
}
