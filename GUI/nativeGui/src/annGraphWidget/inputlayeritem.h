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
#ifndef AGWINPUTLAYERITEM_H
#define AGWINPUTLAYERITEM_H

#include "layeritem.h"

namespace AGW
{
  class InputLayerItem : public LayerItem
  {
    public:
      InputLayerItem( unsigned int numInput, QGraphicsItem *parent = 0 );
      ~InputLayerItem();

      void connectLayerTo( const LayerItem *layer );
      void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    protected:
      QList<Node *> m_nodes;

    private:
      void connectNodes( const QList<Node *> &fromNodes, const QList<Node *> &toNodes );


    private: /* Forbid users to use these methods from the superclass */
      QList<MIMONeuronItem *> neurons() const;
      void addNeuron( MIMONeuronItem * ) {};
  };
}

#endif
