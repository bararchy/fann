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
#include "node.h"

#include <QPainter>

namespace AGW
{
  Node::Node( QGraphicsItem *parent )
      : QGraphicsItem( parent ),
      m_pen( Qt::black, 0 ),
      m_boundingRect( -12, -12, 25, 25 ),
      m_gradient( -3, -3, 10 )
  {
    m_gradient.setColorAt( 0, Qt::cyan );
    m_gradient.setColorAt( 1, Qt::blue );

    setAcceptedMouseButtons( Qt::NoButton );
  }
  Node::~Node()
  {}

  QRectF Node::boundingRect() const
  {
    return m_boundingRect;
  }

  void Node::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
  {
    painter->setPen( m_pen );
    painter->setBrush( m_gradient );
    painter->drawEllipse( -10, -10, 20, 20 );
  }
}
