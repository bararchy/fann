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
#ifndef AGWMIMONEURONITEM_H
#define AGWMIMONEURONITEM_H

#include "node.h"
#include "connection.h"

#include <QGraphicsItem>
#include <QList>
#include <QPen>
#include <QRectF>
#include <QLinearGradient>

namespace AGW
{
  class MIMONeuronItem : public QObject, public QGraphicsItem
  {
      Q_OBJECT

    public:
      MIMONeuronItem( int numNodes, QGraphicsItem *parent = 0 );
      ~MIMONeuronItem();

      QList<Node *> nodes() const;
      QList<Connection *> connections() const;

      bool isHighlighted() const;
      void setHighlight( bool highlighted );

      void connectNeuronFrom ( const MIMONeuronItem *neuron );
      void connectNeuronTo ( const MIMONeuronItem *neuron );

      /* QGraphicsItem event reimplementation */
      QRectF boundingRect() const;
      void mousePressEvent( QGraphicsSceneMouseEvent *event );
      void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    signals:
      void neuronClicked( AGW::MIMONeuronItem *neuronItem );

    protected:
      QList<Node *> m_nodes;
      QList<Connection *> m_connections; /* Connections created after having called connectNodes() */

    private:
      qreal m_width; /* Width of this Layer QGraphicsItem with left and right padding */
      qreal m_height; /* Height of this Layer QGraphicsItem without bottom padding */
      const int m_hPadding; /* Horizontal Padding */
      const int m_vPadding; /* Vertical Padding */
      const int m_vSpace; /* Vertical space between neurons */
      bool m_highlightStatus; /* Highlight status */

      /* Objects used into paint() method. Declared here to speedup things */
      const QPen m_pen;
      const QPen m_penHighlight;
      QRectF m_boundingRect;
      QLinearGradient m_gradient;
      QLinearGradient m_gradientHighlight;

    private:
      void connectNodes ( const QList<Node *> &fromNodes, const QList<Node *> &toNodes );
  };
}

#endif
