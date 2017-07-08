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
#include "defaultlayeritem.h"

namespace AGW
{
  DefaultLayerItem::DefaultLayerItem( QGraphicsItem *parent )
      : LayerItem( parent )
  {
    setAcceptedMouseButtons( Qt::LeftButton );
  }
  DefaultLayerItem::~DefaultLayerItem()
  {}

  void DefaultLayerItem::mousePressEvent( QGraphicsSceneMouseEvent * )
  {
    emit layerClicked( this );
  }

  void DefaultLayerItem::connectLayerTo( const LayerItem *layer )
  {
    connectNeurons( m_neurons, layer->neurons() );
  }

  void DefaultLayerItem::connectNeurons( const QList<MIMONeuronItem *> &fromNeurons, const QList<MIMONeuronItem *> &toNeurons )
  {
    foreach( MIMONeuronItem *fromNeuron, fromNeurons )
    {
      foreach( MIMONeuronItem *toNeuron, toNeurons )
      fromNeuron->connectNeuronTo( toNeuron );

      m_connections << fromNeuron->connections();
    }
  }
}
