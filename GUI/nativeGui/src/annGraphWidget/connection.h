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
#ifndef AGWCONNECTION_H
#define AGWCONNECTION_H

#include "node.h"

#include <QGraphicsItem>
#include <QPen>
#include <QLineF>
#include <QPolygonF>

namespace AGW
{
  class Connection : public QGraphicsItem
  {
    public:
      Connection( Node* fromNode, Node* toNode, QGraphicsItem *parent = 0 );
      ~Connection();

      Node* fromNode() const;
      Node* toNode() const;

      QRectF boundingRect() const;
      void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    private:
      void resizeLine();

    private:
      Node *m_fromNode;
      Node *m_toNode;

      QPointF m_sourcePoint;
      QPointF m_destPoint;
      const qreal m_arrowSize;

      /* Objects used into paint() method. Declared here to speedup things */
      const QPen m_pen;
      QLineF m_line;
      QPolygonF m_arrow;
  };
}

#endif
