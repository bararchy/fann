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
#ifndef DSNEURON_H
#define DSNEURON_H

#include "definitions.h"
#include <QString>

struct fann_neuron;

namespace DS
{
  class Neuron
  {
    public:
      Neuron( struct fann_neuron *value );
      virtual ~Neuron();

    public:
      struct fann_neuron* fann_neuron() const;

      unsigned int numInputs() const;
      unsigned int numOutputs() const;
      unsigned int numSimpleNeurons() const;
      unsigned int numWeights() const;

      /* FIXME valid only for non optimized neuron */
//       int activationFunction() const;
//       void setActivationFunction( int index );

      fannType activationSteepness() const;
      void setActivationSteepness( fannType steepness );

      virtual QString activationFunction() = 0;
      virtual QString trainingAlgorithm() = 0;

      void callDestructor();

    protected:
      struct fann_neuron *m_fann_neuron;
  };
}

#endif
