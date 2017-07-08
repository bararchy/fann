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
#ifndef DSTRAINDATA_H
#define DSTRAINDATA_H

#include "definitions.h"
#include <QString>

struct fann_train_data;

namespace DS
{
  class TrainData
  {
    public:
      TrainData();
      TrainData( const QString &fileName );
      ~TrainData();

      struct fann_train_data* fann_train_data() const;
      fannType* inputData( int index ) const;
      fannType* outputData( int index ) const;

      unsigned int numData() const;
      unsigned int numInputs() const;
      unsigned int numOutputs() const;

      void setTrainDataFile( const QString &fileName );

    protected:
      struct fann_train_data* m_fann_train_data;
  };
}

#endif
