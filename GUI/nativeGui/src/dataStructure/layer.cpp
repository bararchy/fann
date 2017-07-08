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
#include "layer.h"
#include <fann/fann.h>

namespace DS
{
  Layer::Layer( struct fann_layer *value )
      : m_fann_layer( value )
  {}
  Layer::~Layer()
  {}

  struct fann_layer* Layer::fann_layer() const
  {
    return m_fann_layer;
  }

  void Layer::appendNeuron( Neuron *value )
  {
    m_neurons.append( value );
  }

  void Layer::replaceNeuron( int index, Neuron *value )
  {
    m_neurons.replace( index, value );
  }

  int Layer::numNeurons() const
  {
    return m_neurons.size();
  }

  QList<Neuron *> Layer::neurons() const
  {
    return m_neurons;
  }
}
