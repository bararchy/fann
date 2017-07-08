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
#ifndef AGWANNGRAPHWIDGET_H
#define AGWANNGRAPHWIDGET_H

#include "layeritem.h"

#include <QGraphicsView>
#include <QList>

namespace AGW
{
  class AnnGraphWidget : protected QGraphicsView
  {
    Q_OBJECT

    public:
      AnnGraphWidget( QWidget *parent = 0 );
      ~AnnGraphWidget();

      void addLayerItem( LayerItem *layer );
      QList<LayerItem *> layerItems() const;

    public slots:
      void scaleView( qreal scaleFactor );
      void clear();

    protected:
      QList<LayerItem *> m_layers;
  };
}

#endif
