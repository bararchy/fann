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
#ifndef AGWNODE_H
#define AGWNODE_H

#include <QGraphicsItem>
#include <QPen>
#include <QRadialGradient>

namespace AGW
{
  class Node : public QGraphicsItem
  {
    public:
      Node( QGraphicsItem *parent = 0 );
      ~Node();

      QRectF boundingRect() const;
      void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    private:
      /* Objects used into paint() method. Declared here to speedup things */
      const QPen m_pen;
      QRectF m_boundingRect;
      QRadialGradient m_gradient;
  };
}

#endif
