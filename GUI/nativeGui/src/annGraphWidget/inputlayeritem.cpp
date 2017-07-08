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
#include "inputlayeritem.h"
#include "node.h"
#include "mimoneuronitem.h"

#include <QPainter>

namespace AGW
{
  InputLayerItem::InputLayerItem( unsigned int numInput, QGraphicsItem *parent )
      : LayerItem( parent )
  {
    if( numInput == 0 )
      return;

    m_vSpace = 40;
    setAcceptedMouseButtons( Qt::NoButton );

    /* Anticipate one cycle to calculate the width of the nodes */
    Node *node = new Node();

    m_width = node->boundingRect().width() + m_hPadding;
    QPointF position( m_width / 2, m_vPadding );
    const QPointF offSet( 0, m_vSpace );

    node->setParentItem( this );
    node->setPos( position );
    m_nodes << node;
    numInput--;
    position += offSet;

    for ( ; numInput; numInput--, position += offSet )
    {
      node = new Node();
      node->setParentItem( this );
      node->setPos( position );
      m_nodes << node;
    }
    position -= offSet;
    m_height = position.y();

    m_pen = QPen( Qt::black, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin );

    m_boundingRect = QRectF( 0, 0, m_width, m_height + m_vPadding );
    m_gradient = QLinearGradient( m_boundingRect.topLeft(), m_boundingRect.bottomRight() );
    m_gradient.setColorAt( 0, Qt::white );
    m_gradient.setColorAt( 1, Qt::lightGray );
  }
  InputLayerItem::~InputLayerItem()
  {}

  void InputLayerItem::connectLayerTo( const LayerItem *layer )
  {
    foreach( MIMONeuronItem *neuron, layer->neurons() )
      connectNodes( m_nodes, neuron->nodes() );
  }

  void InputLayerItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
  {
    painter->setPen( m_pen );
//     painter->setBrush( m_gradient );
    painter->drawRect( m_boundingRect );
  }

  void InputLayerItem::connectNodes( const QList<Node *> &fromNodes, const QList<Node *> &toNodes )
  {
    foreach( Node *fromNode, fromNodes )
      foreach( Node *toNode, toNodes )
        m_connections << new Connection( fromNode, toNode );
  }
}
