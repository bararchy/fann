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
//  ***************************************************************************/
#include "testwidget.h"
#include <QHeaderView>
#include <QDebug>

TestWidget::TestWidget( QWidget* parent, Qt::WFlags fl )
    : QWidget( parent, fl ), Ui::TestWidget()
{
  setupUi( this );
  plotError->setXAxisTitle("Input Pattern");
  plotError->setYAxisTitle("Error");
  tblInput->horizontalHeader()->setStretchLastSection( true );
  tblTarget->horizontalHeader()->setStretchLastSection( true );
  tblOutput->horizontalHeader()->setStretchLastSection( true );
}
TestWidget::~TestWidget()
{}

void TestWidget::changeTrainData( DS::TrainData *trainData )
{
  unsigned int i, j;
  unsigned int numRows;
  const unsigned int numData = trainData->numData();
  const unsigned int numInput = trainData->numInputs();
  const unsigned int numOutput = trainData->numOutputs();
  DS::fannType *data;

#if 0
  //FIXME clear table --> numRows == 0
  numRows = tblInput->rowCount();
  tblInput->setColumnCount( numInput );
  for( i = 0; i < numData; i++, numRows++ )
  {
    tblInput->setRowCount( numRows + 1 );
    data = trainData->inputData( i );
    for( j = 0; j < numInput; j++ )
      tblInput->setItem( numRows, j, new QTableWidgetItem( QString().setNum( data[j] ) ) );
  }

  //FIXME clear table --> numRows == 0
  numRows = tblTarget->rowCount();
  tblTarget->setColumnCount( numOutput );
  for( i = 0; i < numData; i++, numRows++ )
  {
    tblTarget->setRowCount( numRows + 1 );
    data = trainData->outputData( i );
    for( j = 0; j < numOutput; j++ )
      tblTarget->setItem( numRows, j, new QTableWidgetItem( QString().setNum( data[j] ) ) );
  }
#endif

  m_plotPattern.clear();
  for( i = 1; i <= numData; i++ )
    m_plotPattern << (double)i;
}

void TestWidget::showNetworkTest( DS::Network::TestNetworkValues values )
{
  //FIXME clear table --> numRows == 0
  int i;
  int size = tblTarget->columnCount();
  int numRows = tblOutput->rowCount();
  tblOutput->setColumnCount( size );
  foreach( DS::fannType *value, values.outputs )
  {
    tblOutput->setRowCount( numRows + 1 );
    for( i = 0; i < size; i++ )
    {
      tblOutput->setItem( numRows, i, new QTableWidgetItem( QString().setNum( value[i] ) ) );
    }

    m_plotErrorValues.append( (double)values.totalMSEIncrement );
    plotError->setRawData( m_plotPattern.constData(), m_plotErrorValues.constData(), m_plotPattern.size() );

    numRows++;
  }
}
