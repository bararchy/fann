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
#ifndef DSRPROPSIGMOIDSYMMETRICOPTIMIZEDNEURON_H
#define DSRPROPSIGMOIDSYMMETRICOPTIMIZEDNEURON_H

#include <neuron.h>

struct fann_rprop_params;

namespace DS
{
  class RPropSigmoidSymmetricOptimizedNeuron : public Neuron
  {
    public:
      RPropSigmoidSymmetricOptimizedNeuron( struct fann_neuron *value );
      ~RPropSigmoidSymmetricOptimizedNeuron();

      /* Tells how much the stepsize should increase during learning */
      float increaseFactor() const;
      void setIncreaseFactor( float value );

      /* Tells how much the stepsize should decrease during learning */
      float decreaseFactor() const;
      void setDecreaseFactor( float value );

      /* The minimum stepsize */
      float deltaMin() const;
      void setDeltaMin( float value );

      /* The maximum stepsize */
      float deltaMax() const;
      void setDeltaMax( float value );

      /* The initial stepsize */
      float deltaZero() const;
      void setDeltaZero( float value );

      QString activationFunction();
      QString trainingAlgorithm();

    private:
      struct fann_rprop_params *m_fann_rprop_params;
  };
}

#endif
