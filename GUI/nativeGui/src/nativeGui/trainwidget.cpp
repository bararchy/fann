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
#include "trainwidget.h"
#include <QHeaderView>

TrainWidget::TrainWidget( QWidget *parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ), Ui::TrainWidget(),
    m_plotEpochs( 100 ),
    m_plotBFValues( 100 ),
    m_plotMSEValues( 100 )
{
  setupUi( this );
  tblTrainLog->verticalHeader()->hide();
  tblTrainLog->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
  tblTrainLog->horizontalHeader()->setStretchLastSection( true );
  plotBitFail->setXAxisTitle("Epochs");
  plotBitFail->setYAxisTitle("Bit Fail");
  plotMSE->setXAxisTitle("Epochs");
  plotMSE->setYAxisTitle("Mean Square Error");
}
TrainWidget::~TrainWidget()
{}

void TrainWidget::updateTrainEpochValues( DS::Network::TrainEpochValues epochValue )
{
  const QString epoch = QString().setNum( epochValue.epoch );
  const QString bitFail = QString().setNum( epochValue.bitFail );
  const QString mse = QString().setNum( epochValue.mse );
  const int lastRow = tblTrainLog->rowCount();

  lblCurrentEpoch->setText( epoch );
  lblCurrentBitFail->setText( bitFail );
  lblCurrentMSE->setText( mse );

  tblTrainLog->setRowCount( lastRow + 1 );
  tblTrainLog->setItem( lastRow, 0, new QTableWidgetItem( epoch ) );
  tblTrainLog->setItem( lastRow, 1, new QTableWidgetItem( bitFail ) );
  tblTrainLog->setItem( lastRow, 2, new QTableWidgetItem( mse ) );

  m_plotEpochs.append( (double)epochValue.epoch );
  m_plotBFValues.append( (double)epochValue.bitFail );
  m_plotMSEValues.append( (double)epochValue.mse );
  plotBitFail->setRawData( m_plotEpochs.constData(), m_plotBFValues.constData(), m_plotEpochs.size() );
  plotMSE->setRawData( m_plotEpochs.constData(), m_plotMSEValues.constData(), m_plotEpochs.size() );
}
