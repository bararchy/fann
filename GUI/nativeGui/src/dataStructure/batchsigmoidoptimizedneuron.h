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
#ifndef DSBATCHSIGMOIDOPTIMIZEDNEURON_H
#define DSBATCHSIGMOIDOPTIMIZEDNEURON_H

#include <neuron.h>

struct fann_backprop_params;

namespace DS
{
  class BatchSigmoidOptimizedNeuron : public Neuron
  {
    public:
      BatchSigmoidOptimizedNeuron( struct fann_neuron *value );
      ~BatchSigmoidOptimizedNeuron();

      /* the learning rate of the network */
      float learningRate() const;
      void setLearningRate( float value );

      /* The learning momentum used for backpropagation algorithm. */
      float learningMomentum() const;
      void setLearningMomentum( float value );

      QString activationFunction();
      QString trainingAlgorithm();

    private:
      struct fann_backprop_params *m_fann_backprop_params;
  };
}

#endif
