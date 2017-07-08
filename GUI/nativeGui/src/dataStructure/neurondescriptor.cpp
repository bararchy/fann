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
#include "neurondescriptor.h"
#include <fann/fann.h>
extern "C"
{
  int nativeGUI_neuron_constructor( struct fann *fann_ann, struct fann_layer *fann_layer, struct fann_neuron *fann_neuron, struct fann_neuron_descr *fann_descr );
}

namespace DS
{
  NeuronDescriptor::NeuronDescriptor( int num_outputs )
      : m_fann_neuron_descr( NULL ),
      m_num_outputs( num_outputs )
  {}

  NeuronDescriptor::~NeuronDescriptor()
  {
    delete m_fann_neuron_descr;
  }

  struct fann_neuron_descr* NeuronDescriptor::fann_neuron_descr() const
  {
    if (!m_fann_neuron_descr)
      const_cast<NeuronDescriptor*>(this)->create_neuron_descriptor();
    return m_fann_neuron_descr;
  }

  void NeuronDescriptor::set_fann_neuron_descr( struct fann_neuron_descr* value )
  {
    if (m_fann_neuron_descr)
      delete m_fann_neuron_descr;
    m_fann_neuron_descr = value;
  }

  void NeuronDescriptor::create_neuron_descriptor()
  {
    if (m_fann_neuron_descr)
      return;
    m_fann_neuron_descr = new struct fann_neuron_descr();
    m_fann_neuron_descr->constructor = nativeGUI_neuron_constructor;
    m_fann_neuron_descr->private_data = NULL;
    m_fann_neuron_descr->num_outputs = m_num_outputs;
    m_fann_neuron_descr->run = NULL;
    m_fann_neuron_descr->backpropagate = NULL;
    m_fann_neuron_descr->update_weights = NULL;
  }
}
