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
#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <QFrame>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"

class DataPlot : public QwtPlot
{
    Q_OBJECT

  public:
    DataPlot( QWidget *parent = 0 );
    ~DataPlot();

    void setRawData( const double *xData, const double *yData, int size );
    void setXAxisTitle( QString title );
    void setYAxisTitle( QString title );

  private:
    QwtPlotCurve m_plotCurve;
};

#endif
