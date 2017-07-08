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
#ifndef TRAINPROPERTYWIDGET_H
#define TRAINPROPERTYWIDGET_H

#include <QWidget>
#include "ui_trainproperty_widget.h"

namespace DS
{
  class TrainingParameters;
}

class TrainPropertyWidget : public QWidget, private Ui::TrainProperty
{
    Q_OBJECT

  public:
    TrainPropertyWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
    ~TrainPropertyWidget();

  public slots:
    void updateValues( DS::TrainingParameters *trainParam );

    void on_cmbErrorFunction_currentIndexChanged( int index );
    void on_cmbStopFunction_currentIndexChanged( int index );
    void on_spinBitFailLimit_valueChanged( double value );
    void on_spinNumBitFail_valueChanged( int value );
    void on_spinMaxEpochs_valueChanged( int value );
    void on_spinDesiredError_valueChanged( double value );

  signals:
    void trainingParametersChanged( DS::TrainingParameters *trainParam );

  private:
    DS::TrainingParameters *m_lastTrainParam;
};

#endif
