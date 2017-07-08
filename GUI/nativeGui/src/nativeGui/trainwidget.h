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
#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>
#include "ui_train_widget.h"
#include "network.h"
#include "buffervector.h"

class TrainWidget : public QWidget, private Ui::TrainWidget
{
    Q_OBJECT

  public:
    TrainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~TrainWidget();

  public slots:
    void updateTrainEpochValues( DS::Network::TrainEpochValues epochValue );

  private:
    BufferVector<double> m_plotEpochs;
    BufferVector<double> m_plotBFValues;
    BufferVector<double> m_plotMSEValues;
};

#endif
