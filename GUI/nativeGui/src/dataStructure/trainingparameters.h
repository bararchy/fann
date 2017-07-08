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
#ifndef DSTRAININGPARAMETERS_H
#define DSTRAININGPARAMETERS_H

#include "definitions.h"

struct fann;
struct fann_training_params;

namespace DS
{
  class TrainingParameters
  {
    public:
      TrainingParameters( struct fann *value );
      ~TrainingParameters();

      /* FIXME Not valid here in optimized version */
      /* Training algorithm used when calling fann_train_on_... */
//       int algorithm() const;
//       void setAlgorithm( int value );

      /* The maximum difference between the actual output and the expected output
       * which is accepted when counting the bit fails.
       * This difference is multiplied by two when dealing with symmetric activation functions,
       * so that symmetric and not symmetric activation functions can use the same limit.
       */
      fannType bitFailLimit() const;
      void setBitFailLimit( fannType value );

      /* The number of outputs which would fail (only valid for classification problems) */
      unsigned int numBitFail() const;
      void setNumBitFail( unsigned int value );

      /* Desired output error */
      float desiredError() const;
      void setDesiredError( float value );

      /* Max epochs to train the net */
      unsigned int maxEpochs() const;
      void setMaxEpochs( unsigned int value );

      /* The error function used during training. (default FANN_ERRORFUNC_TANH) */
      int errorFunction() const;
      void setErrorFunction( int value );

      /* The stop function used during training. (default FANN_STOPFUNC_MSE) */
      int stopFunction() const;
      void setStopFunction( int value );

      /* The total error value. The real mean square error is MSE_value/num_MSE */
      float totalMSEValue() const;

    protected:
      struct fann_training_params *m_fann_training_params;
      float m_desiredError;
      unsigned int m_maxEpochs;
  };
}

#endif
