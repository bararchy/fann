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
#ifndef DSNETWORKDESCRIPTOR_H
#define DSNETWORKDESCRIPTOR_H

#include <QList>
#include "layerdescriptor.h"
#include "network.h"

struct fann_descr;

namespace DS /* Data Structure */
{
  class NetworkDescriptor
  {
    public:
      NetworkDescriptor( int num_inputs );
      ~NetworkDescriptor();

    public:
      struct fann_descr* fann_descr() const;
      unsigned int numInputs() const;

      Network* net() const;
      void setNet( Network* value );

      LayerDescriptor* layerDescriptor( int index ) const;
      void appendLayerDescriptor( LayerDescriptor *value );

      void create_fann_descr();

    protected:
      struct fann_descr *m_fann_descr;
      Network *m_net;
      QList<LayerDescriptor*> m_layers;
      unsigned int m_num_inputs;
  };
}

#endif
