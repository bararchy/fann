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
#include "dataplot.h"
#include <QFont>
#include <QPainter>
#include <QPaintEngine>
#include "qwt_symbol.h"
#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_draw.h"
#include "qwt_text.h"
#include <QDebug>

DataPlot::DataPlot( QWidget *parent )
    : QwtPlot( parent )
{
  // Disable polygon clipping
  QwtPainter::setDeviceClipping( false );

   // Disable cache here. Speedup thinghs.
  canvas()->setPaintAttribute( QwtPlotCanvas::PaintCached, false );
  canvas()->setPaintAttribute( QwtPlotCanvas::PaintPacked, false );

#ifdef Q_WS_X11
  /*
   * Qt::WA_PaintOnScreen is only supported for X11, but leads
   * to substantial bugs with Qt 4.2.x/Windows
   */
  canvas()->setAttribute( Qt::WA_PaintOnScreen, true );
#endif

  canvas()->setFrameStyle( QFrame::NoFrame | QFrame::Plain );
  canvas()->setLineWidth(1);

  for ( int i = 0; i < QwtPlot::axisCnt; i++ )
  {
    QwtScaleWidget *scaleWidget = axisWidget(i);
    if ( scaleWidget )
      scaleWidget->setMargin(0);

    QwtScaleDraw *scaleDraw = axisScaleDraw(i);
    if ( scaleDraw )
      scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
  }

  QwtSymbol symbol;
//   symbol.setStyle(QwtSymbol::Ellipse);
  symbol.setPen(QColor(Qt::black));
  symbol.setSize(5);

  m_plotCurve.attach( this );
  m_plotCurve.setPen(QColor(Qt::blue));
  m_plotCurve.setStyle(QwtPlotCurve::Lines);
  m_plotCurve.setCurveAttribute(QwtPlotCurve::Fitted);
  m_plotCurve.setSymbol( symbol );
}
DataPlot::~DataPlot()
{}

void DataPlot::setRawData( const double *xData, const double *yData, int size )
{
  m_plotCurve.setRawData( xData, yData, size );
  replot();
}

void DataPlot::setXAxisTitle( QString title )
{
  QwtText text = QwtText( title );
  text.setFont( QFont( "Sans Serif", 9, QFont::Bold ) );
  setAxisTitle(QwtPlot::xBottom, text);
}

void DataPlot::setYAxisTitle( QString title )
{
  QwtText text = QwtText( title );
  text.setFont( QFont( "Sans Serif", 9, QFont::Bold ) );
  setAxisTitle(QwtPlot::yLeft, text);
}
