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
#include "traindata.h"
#include <fann/fann.h>

namespace DS
{
  TrainData::TrainData()
      : m_fann_train_data( NULL )
  {}
  TrainData::TrainData( const QString &fileName )
  {
    m_fann_train_data = fann_read_train_from_file( fileName.toAscii().data() );
  }
  TrainData::~TrainData()
  {}

  struct fann_train_data* TrainData::fann_train_data() const
  {
    return m_fann_train_data;
  }

  fannType* TrainData::inputData( int index ) const
  {
    return m_fann_train_data->input[index];
  }

  fannType* TrainData::outputData( int index ) const
  {
    return m_fann_train_data->output[index];
  }

  unsigned int TrainData::numData() const
  {
    return m_fann_train_data->num_data;
  }

  unsigned int TrainData::numInputs() const
  {
    return m_fann_train_data->num_input;
  }

  unsigned int TrainData::numOutputs() const
  {
    return m_fann_train_data->num_output;
  }

  void TrainData::setTrainDataFile( const QString &fileName )
  {
    m_fann_train_data = fann_read_train_from_file( fileName.toAscii().data() );
  }
}
