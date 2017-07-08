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
#include "anngraphwidget.h"
#include "connection.h"
#include "mimoneuronitem.h"

#include <QGraphicsScene>
#include <QDebug>

#ifdef AGW_OPENGL
#include <QGLFormat>
#include <QGLWidget>
#endif

#include <math.h>

namespace AGW
{
  AnnGraphWidget::AnnGraphWidget( QWidget *parent )
      : QGraphicsView( parent )
  {
    QGraphicsScene *scene = new QGraphicsScene( this );
    QRectF sceneRect( 0, 0, 0, 0 );
    scene->setSceneRect( sceneRect );
    setScene( scene );

#ifdef AGW_OPENGL
    setViewport( new QGLWidget( QGLFormat( QGL::SampleBuffers) ) );
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
#endif

    QLinearGradient gradient( 0, 0, 500, 500 ); //FIXME adjust gradient bounds...when adding item to the scene, the gradient bounds will change...
    gradient.setColorAt( 0, QColor( "#ecf5f8" ) );
    gradient.setColorAt( 1, QColor( "#b9dce7" ) );
    this->setBackgroundBrush( gradient );

#ifndef AGW_OPENGL
    setCacheMode( QGraphicsView::CacheBackground );
#endif
    setRenderHints( QPainter::Antialiasing );
    setResizeAnchor( QGraphicsView::AnchorViewCenter );
    setTransformationAnchor( QGraphicsView::AnchorViewCenter );

    //FIXME
    scale( 0.8, 0.8 );
    scale( 0.8, 0.8 );
    scale( 0.8, 0.8 );
  }
  AnnGraphWidget::~AnnGraphWidget()
  {}

  void AnnGraphWidget::addLayerItem( LayerItem *layer )
  {
    static QPointF offSet( 0, 0 );
    const qreal layerHeight = layer->boundingRect().height();
    const QPointF position( 0, -( layerHeight / 2 ) );

    m_layers << layer;

    QGraphicsScene *tmp = scene();
    layer->setPos( position + offSet );
    offSet.rx() += 300;
    tmp->addItem( layer );

    const int size = m_layers.size();
    if ( size < 2 )
      return;

    m_layers[ size - 2 ]->connectLayerTo( layer );
    foreach( Connection *conn, m_layers[ size - 2 ]->connections() )
    {
      conn->setZValue( 1 );
      tmp->addItem( conn );
    }
  }

  QList<LayerItem *> AnnGraphWidget::layerItems() const
  {
    return m_layers;
  }

  void AnnGraphWidget::scaleView( qreal scaleFactor )
  {
    const qreal factor = matrix().scale( scaleFactor, scaleFactor ).mapRect( QRectF( 0, 0, 1, 1 ) ).width();
    if ( factor < 0.07 || factor > 100 )
      return;

    scale( scaleFactor, scaleFactor );
  }

  void AnnGraphWidget::clear()
  {
    QList<QGraphicsItem *> list = scene()->items();
    foreach( QGraphicsItem *item, list )
    {
      //FIXME missing delete
      scene()->removeItem( item );
    }
  }
}
