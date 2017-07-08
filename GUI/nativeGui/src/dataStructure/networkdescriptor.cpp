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
#include "networkdescriptor.h"
#include <fann/fann.h>

extern "C"
{
  int nativeGUI_layer_constructor( struct fann *fann_ann, struct fann_layer *fann_layer, struct fann_layer_descr *fann_descr )
  {
    DS::NetworkDescriptor *netDescr;
    netDescr = static_cast<DS::NetworkDescriptor*>( fann_descr->private_data ); /* take back the NetworkDescriptor */

    DS::Layer* layer = new DS::Layer( fann_layer );
    netDescr->net()->appendLayer( layer ); /* add the layer to the net */
    netDescr->layerDescriptor( netDescr->net()->numLayers() - 1 )->setLayer( layer ); /* add the layer to the appropiate LayerDescriptor for further reference */

    fann_layer_constructor_connected_any_any( fann_ann, fann_layer, fann_descr ); /* call FANN function to properly setup the structs */

    return 0;
  }
}

namespace DS
{
  NetworkDescriptor::NetworkDescriptor( int num_inputs )
      : m_fann_descr( NULL ),
      m_net( NULL ),
      m_num_inputs( num_inputs )
  {}
  NetworkDescriptor::~NetworkDescriptor()
  {
    delete m_fann_descr;
  }

  struct fann_descr* NetworkDescriptor::fann_descr() const
  {
    if (!m_fann_descr)
      const_cast<NetworkDescriptor*>(this)->create_fann_descr();
    return m_fann_descr;
  }

  unsigned int NetworkDescriptor::numInputs() const
  {
    return m_fann_descr->num_inputs;
  }

  Network* NetworkDescriptor::net() const
  {
    return m_net;
  }
  void NetworkDescriptor::setNet( Network* value )
  {
    m_net = value;
  }

  LayerDescriptor* NetworkDescriptor::layerDescriptor( int index ) const
  {
    if ( index >= m_layers.size() )
      return NULL;

    return m_layers[index];
  }

  void NetworkDescriptor::appendLayerDescriptor( LayerDescriptor* value )
  {
    m_layers.append( value );
  }

  void NetworkDescriptor::create_fann_descr( )
  {
    if (m_fann_descr)
    {
      return;
    }

    m_fann_descr=new struct fann_descr();
    m_fann_descr->num_inputs = m_num_inputs;
    m_fann_descr->connection_rate = 0;

    int num_layers = m_layers.size();
    m_fann_descr->num_layers = num_layers;
    struct fann_layer_descr *it = m_fann_descr->layers_descr = new struct fann_layer_descr[num_layers];
    foreach (LayerDescriptor * value, m_layers)
    {
      value->fann_layer_descr()->private_data = this;
      *(it++)=*(value->fann_layer_descr());
      value->set_fann_layer_descr(it);
    }
  }
}
