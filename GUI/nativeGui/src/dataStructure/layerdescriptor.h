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
#ifndef DSLAYERDESCRIPTOR_H
#define DSLAYERDESCRIPTOR_H

#include <QList>
#include "neurondescriptor.h"
#include "layer.h"

struct fann_layer_descr;

namespace DS /* Data Structure */
{
  class LayerDescriptor
  {
    public:
      LayerDescriptor();
      ~LayerDescriptor();

    public:
      struct fann_layer_descr* fann_layer_descr() const;
      void set_fann_layer_descr( struct fann_layer_descr* value );

      Layer* layer() const;
      void setLayer( Layer* value );

      void appendNeuronDescriptor( NeuronDescriptor *value );

    protected:
      struct fann_layer_descr *m_fann_layer_descr;
      Layer *m_layer;
      QList<NeuronDescriptor*> m_neurons;

      void create_fann_layer_descr( );
  };
}

#endif
