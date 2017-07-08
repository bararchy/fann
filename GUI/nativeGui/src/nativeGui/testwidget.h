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

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include "ui_test_widget.h"
#include "network.h"

class TestWidget : public QWidget, private Ui::TestWidget
{
    Q_OBJECT

  public:
    TestWidget( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~TestWidget();

  public slots:
    void changeTrainData( DS::TrainData *trainData );
    void showNetworkTest( DS::Network::TestNetworkValues values );

  private:
    QVector<double> m_plotPattern;
    QVector<double> m_plotErrorValues;
};

#endif

