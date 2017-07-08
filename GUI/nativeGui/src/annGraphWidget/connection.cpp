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
#include "connection.h"

#include <QPainter>
#include "math.h"

namespace AGW
{
  Connection::Connection( Node* fromNode, Node* toNode, QGraphicsItem *parent )
      : QGraphicsItem( parent ),
      m_fromNode( fromNode ),
      m_toNode( toNode ),
      m_arrowSize( 10 ),
      m_pen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
  {
    setAcceptedMouseButtons( Qt::NoButton );
    resizeLine();

    const double Pi = 3.14159265358979323846264338327950288419717;
    const double TwoPi = 2.0 * Pi;

    m_line = QLineF( m_sourcePoint, m_destPoint );
    double angle = ::acos( m_line.dx() / m_line.length() );
    if ( m_line.dy() >= 0 )
      angle = TwoPi - angle;

    QPointF arrowP1 = m_destPoint + QPointF( sin( angle - Pi / 3 ) * m_arrowSize, cos( angle - Pi / 3 ) * m_arrowSize );
    QPointF arrowP2 = m_destPoint + QPointF( sin( angle - Pi + Pi / 3 ) * m_arrowSize, cos( angle - Pi + Pi / 3 ) * m_arrowSize );

    m_arrow = QPolygonF() << m_line.p2() << arrowP1 << arrowP2;
  }
  Connection::~Connection()
  {}

  Node* Connection::fromNode() const
  {
    return m_fromNode;
  }
  Node* Connection::toNode() const
  {
    return m_toNode;
  }

  QRectF Connection::boundingRect() const
  {
    const qreal penWidth = 1;
    const qreal extra = ( penWidth + m_arrowSize ) / 2.0;

    return QRectF( m_sourcePoint, QSizeF( m_destPoint.x() - m_sourcePoint.x(), m_destPoint.y() - m_sourcePoint.y() ) )
           .normalized()
           .adjusted( -extra, -extra, extra, extra );
  }

  void Connection::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
  {
    painter->setPen( m_pen );
    painter->drawLine( m_line );

    painter->setBrush( Qt::black );
    painter->drawPolygon( m_arrow );
  }

  void Connection::resizeLine()
  {
    QLineF line( mapFromItem( m_fromNode, 0, 0 ), mapFromItem( m_toNode, 0, 0 ) );
    const qreal length = line.length();

    QPointF edgeOffset(( line.dx() * 10 ) / length, ( line.dy() * 10 ) / length );

    prepareGeometryChange(); //prepareGeometryChange() will call update() if this is necessary
    m_sourcePoint = line.p1() + edgeOffset;
    m_destPoint = line.p2() - edgeOffset;
  }
}
