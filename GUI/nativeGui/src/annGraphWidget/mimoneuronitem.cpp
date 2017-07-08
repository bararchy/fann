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
#include "mimoneuronitem.h"

#include <QPainter>

namespace AGW
{
  MIMONeuronItem::MIMONeuronItem( int numNodes , QGraphicsItem *parent )
      : QGraphicsItem( parent ),
      m_width( 0 ),
      m_height( 0 ),
      m_hPadding( 60 ),
      m_vPadding( 40 ),
      m_vSpace( 40 ),
      m_highlightStatus( false ),
      m_pen( Qt::darkMagenta, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ),
      m_penHighlight( Qt::darkMagenta, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
  {
    setAcceptedMouseButtons( Qt::LeftButton );

    if ( numNodes > 0 )
    {
      Node *node;
      QPointF position( 0, m_vPadding );
      const QPointF offSet( 0, m_vSpace );

      for ( ; numNodes; numNodes--, position += offSet )
      {
        node = new Node();
        node->setParentItem( this );
        node->setPos( position );

        m_nodes << node;
      }
      position -= offSet;

      m_width = node->boundingRect().width() + m_hPadding;
      m_height = position.y();

      m_boundingRect = QRectF( -m_width / 2, 0, m_width, m_height + m_vPadding );
      m_gradient = QLinearGradient( m_boundingRect.topLeft(), m_boundingRect.bottomRight() );
      m_gradient.setColorAt( 0, QColor( 255, 220, 255, 75 ) );
      m_gradient.setColorAt( 1, QColor( 255, 0, 255, 75 ) );

      m_gradientHighlight = QLinearGradient( m_boundingRect.topLeft(), m_boundingRect.bottomRight() );
      m_gradientHighlight.setColorAt( 0, QColor( 255, 220, 255, 255 ) );
      m_gradientHighlight.setColorAt( 1, QColor( 255, 0, 255, 255 ) );
    }
  }
  MIMONeuronItem::~MIMONeuronItem()
  {}

  QList<Node *> MIMONeuronItem::nodes() const
  {
    return m_nodes;
  }
  QList<Connection *> MIMONeuronItem::connections() const
  {
    return m_connections;
  }

  bool MIMONeuronItem::isHighlighted() const
  {
    return m_highlightStatus;
  }
  void MIMONeuronItem::setHighlight( bool highlight )
  {
    m_highlightStatus = highlight;
    update();
  }

  void MIMONeuronItem::connectNeuronFrom( const MIMONeuronItem *neuron )
  {
    connectNodes( neuron->nodes(), m_nodes );
  }
  void MIMONeuronItem::connectNeuronTo( const MIMONeuronItem *neuron )
  {
    connectNodes( m_nodes, neuron->nodes() );
  }

  QRectF MIMONeuronItem::boundingRect() const
  {
    return m_boundingRect;
  }
  void MIMONeuronItem::mousePressEvent( QGraphicsSceneMouseEvent * )
  {
    emit neuronClicked( this );
  }
  void MIMONeuronItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
  {
    if( m_highlightStatus )
    {
      painter->setPen( m_penHighlight );
      painter->setBrush( m_gradientHighlight );
    }
    else
    {
      painter->setPen( m_pen );
      painter->setBrush( m_gradient );
    }
    //FIXME the angle of the rounded rect are not properly computed...
    painter->drawRoundRect( m_boundingRect, static_cast<int>( 10 / ( m_boundingRect.width() / m_boundingRect.height() ) ), 10 );
  }

  void MIMONeuronItem::connectNodes( const QList<Node *> &fromNodes, const QList<Node *> &toNodes )
  {
    foreach( Node *fromNode, fromNodes )
      foreach( Node *toNode, toNodes )
        m_connections << new Connection( fromNode, toNode );
  }
}
