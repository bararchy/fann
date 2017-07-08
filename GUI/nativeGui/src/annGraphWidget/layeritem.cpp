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
#include "layeritem.h"
#include <QPainter>

namespace AGW
{
  LayerItem::LayerItem( QGraphicsItem *parent )
      : QGraphicsItem( parent ),
      m_width( 0 ),
      m_height( 0 ),
      m_hPadding( 60 ),
      m_vPadding( 40 ),
      m_vSpace( 30 ),
      m_pen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
  {}
  LayerItem::~LayerItem()
  {}

  QList<Connection *> LayerItem::connections() const
  {
    return m_connections;
  }
  QList<MIMONeuronItem *> LayerItem::neurons() const
  {
    return m_neurons;
  }

  void LayerItem::addNeuron( MIMONeuronItem *neuron )
  {
    if ( !m_neurons.size() )
      m_height += m_vPadding;
    else
      m_height += m_vSpace;

    const QRectF &boundRect = neuron->boundingRect();
    m_width = boundRect.width() + m_hPadding;

    neuron->setParentItem( this );
    neuron->setPos( m_width / 2, m_height );

    m_height += boundRect.height();

    m_neurons << neuron;

    m_boundingRect = QRectF( 0, 0, m_width, m_height + m_vPadding );
    m_gradient = QLinearGradient( m_boundingRect.topLeft(), m_boundingRect.bottomRight() );
    m_gradient.setColorAt( 0, Qt::white );
    m_gradient.setColorAt( 1, Qt::lightGray );
  }

  QRectF LayerItem::boundingRect() const
  {
    return m_boundingRect;
  }
  void LayerItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
  {
    painter->setPen( m_pen );
    painter->setBrush( m_gradient );
    //FIXME the angle of the rounded rect are not properly computed...
    painter->drawRoundRect( m_boundingRect, static_cast<int>( 4 / ( m_boundingRect.width() / m_boundingRect.height() ) ), 4 );
  }

}
