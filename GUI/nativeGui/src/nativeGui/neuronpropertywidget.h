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
#ifndef NEURONPROPERTYWIDGET_H
#define NEURONPROPERTYWIDGET_H

#include <QWidget>
#include "ui_neuronproperty_widget.h"

namespace DS
{
  class Neuron;
}

class NeuronPropertyWidget : public QWidget, private Ui::NeuronProperty
{
    Q_OBJECT

  public:
    NeuronPropertyWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~NeuronPropertyWidget();

  public slots:
    void updateValues( DS::Neuron *neuron );

    void on_spinSteepness_valueChanged( double value );
    void on_cmbActivationFunction_currentIndexChanged( int index );
    void on_cmbTrainAlgorithm_currentIndexChanged( int index );

    void on_spinLearningMomentum_valueChanged( double value );
    void on_spinLearningRate_valueChanged( double value );

    void on_spinIncreaseFactor_valueChanged( double value );
    void on_spinDecreaseFactor_valueChanged( double value );
    void on_spinDeltaMin_valueChanged( double value );
    void on_spinDeltaMax_valueChanged( double value );
    void on_spinDeltaZero_valueChanged( double value );

  signals:
    /* Emitted if some parameter of the current neuron is changed. Not emitted if is changed the activation function or the training algorithm */
    void neuronParametersChanged( DS::Neuron *neuron );
    void changeNeuronTypeRequested( DS::Neuron *neuron, const QString &value );

  private:
    DS::Neuron *m_lastNeuron;
};

#endif
