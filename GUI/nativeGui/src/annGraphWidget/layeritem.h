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
#ifndef AGWLAYERITEM_H
#define AGWLAYERITEM_H

#include "connection.h"
#include "mimoneuronitem.h"

#include <QGraphicsItem>

namespace AGW
{
  class LayerItem : public QObject, public QGraphicsItem
  {
    Q_OBJECT

    public:
      LayerItem( QGraphicsItem *parent = 0 );
      ~LayerItem();

      QList<MIMONeuronItem *> neurons() const;
      QList<Connection *> connections() const;

      virtual void addNeuron( MIMONeuronItem *neuron );
      virtual void connectLayerTo ( const LayerItem *layer ) = 0;

      /* QGraphicsItem event reimplementation */
      virtual QRectF boundingRect() const;
      virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    protected:
      QList<MIMONeuronItem *> m_neurons;
      QList<Connection *> m_connections;

      qreal m_width; /* Width of this Layer QGraphicsItem with left and right padding */
      qreal m_height; /* Height of this Layer QGraphicsItem without bottom padding */
      int m_hPadding; /* Horizontal Padding */
      int m_vPadding; /* Vertical Padding */
      int m_vSpace; /* Vertical space between neurons */

      /* Objects used into paint() method. Declared here to speedup things */
      QPen m_pen;
      QRectF m_boundingRect;
      QLinearGradient m_gradient;
  };
}

#endif
