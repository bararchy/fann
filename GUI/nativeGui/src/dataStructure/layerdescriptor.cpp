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
#include "layerdescriptor.h"
#include "rpropsigmoidsymmetricoptimizedneuron.h"
#include <fann/fann.h>
#include <fann/optimized/blas/fann.h>

extern "C"
{
  int nativeGUI_neuron_constructor( struct fann *fann_ann, struct fann_layer *fann_layer, struct fann_neuron *fann_neuron, struct fann_neuron_descr *fann_descr )
  {
    DS::LayerDescriptor *layerDescr;
    layerDescr = static_cast<DS::LayerDescriptor*>( fann_descr->private_data );

//     fann_neuron_constructor_connected_any_any( fann_ann, fann_layer, fann_neuron, fann_descr );
    fann_neuron_constructor_blas_rprop_sigmoid_symmetric( fann_ann, fann_layer, fann_neuron, fann_descr );

    DS::RPropSigmoidSymmetricOptimizedNeuron *neuron = new DS::RPropSigmoidSymmetricOptimizedNeuron( fann_neuron );
    layerDescr->layer()->appendNeuron( neuron );

    return 0;
  }

  int nativeGUI_layer_constructor( struct fann *fann_ann, struct fann_layer *fann_layer, struct fann_layer_descr *fann_descr );
}

namespace DS
{
  LayerDescriptor::LayerDescriptor()
      : m_fann_layer_descr( NULL ),
      m_layer( NULL )
  {}
  LayerDescriptor::~LayerDescriptor()
  {
    delete m_fann_layer_descr;
  }

  struct fann_layer_descr* LayerDescriptor::fann_layer_descr() const
  {
    if (! m_fann_layer_descr)
      const_cast<LayerDescriptor*>(this)->create_fann_layer_descr();
    return m_fann_layer_descr;
  }

  Layer* LayerDescriptor::layer() const
  {
    return m_layer;
  }
  void LayerDescriptor::setLayer( Layer* value )
  {
    m_layer = value;
  }

  void LayerDescriptor::appendNeuronDescriptor( NeuronDescriptor *value )
  {
    m_neurons.append( value );
  }

  void LayerDescriptor::set_fann_layer_descr( struct fann_layer_descr* value )
  {
    if (m_fann_layer_descr)
      delete m_fann_layer_descr;
    m_fann_layer_descr = value;
  }

  void LayerDescriptor::create_fann_layer_descr( )
  {
    if (m_fann_layer_descr)
    {
      return;
    }

    m_fann_layer_descr = new struct fann_layer_descr();
    m_fann_layer_descr->constructor = nativeGUI_layer_constructor;
    m_fann_layer_descr->private_data = NULL;

    int num_neurons = m_neurons.size();
    m_fann_layer_descr->num_neurons = num_neurons;
    struct fann_neuron_descr *it = m_fann_layer_descr->neurons_descr = new struct fann_neuron_descr[num_neurons];
    foreach (NeuronDescriptor * value, m_neurons)
    {
      value->fann_neuron_descr()->private_data = this;
      *it=*(value->fann_neuron_descr());
      value->set_fann_neuron_descr(it);
      it++;
    }
  }
}
