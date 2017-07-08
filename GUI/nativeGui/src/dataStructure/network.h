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
#ifndef DSNETWORK_H
#define DSNETWORK_H

#include "definitions.h"
#include "layer.h"
#include "trainingparameters.h"
#include "traindata.h"

#include <QList>
#include <QStringList>

struct fann;

namespace DS /* Data Structure */
{
  class NetworkDescriptor;

  class Network
  {
    public:
      Network();
      ~Network();

      inline struct fann* fann() const;

      inline int numLayers() const;
      inline QList<Layer *> layers() const;
      inline TrainingParameters* trainParam() const;
      inline TrainData* trainData() const;
      void setTrainData( TrainData *data );

      void appendLayer( Layer *value );
      void createNetFromDescriptor( NetworkDescriptor *descriptor );

      struct TrainEpochValues
      {
        TrainEpochValues() { epoch = 0; bitFail = 0; mse = 0; }
        unsigned int epoch;
        unsigned int bitFail;
        float mse;
      };

      struct TestNetworkValues
      {
        TestNetworkValues() { totalMSE = 0; totalMSEIncrement = 0; }
        QList<fannType *> outputs;
        float totalMSE;
        float totalMSEIncrement;
      };

      Network::TrainEpochValues trainEpoch();
      Network::TestNetworkValues testNetwork();

    protected:
      struct fann *m_fann;
      QList<Layer *> m_layers;
      TrainingParameters *m_trainParam;
      TrainData *m_trainData;
      TrainEpochValues m_trainEpochValues;
      TestNetworkValues m_testNetworkValues;

    /* Static Methods */
    public:
      static QStringList activationFunctions();
      static QStringList errorFunctions();
      static QStringList stopFunctions();
      static QStringList trainingAlgorithms();

      static QStringList blasActivationFunctions();
      static QStringList blasTrainingAlgorithms();

    protected:
      static QStringList ms_activationFunctions;
      static QStringList ms_errorFunctions;
      static QStringList ms_stopFunctions;
      static QStringList ms_trainingAlgorithms;

      static QStringList ms_blasActivationFunctions;
      static QStringList ms_blasTrainingAlgorithms;

    private:
      static void populateBlasNames();
      static QString capitalizeString( const QString &string );
  };


  struct fann* Network::fann() const
  {
    return m_fann;
  }
  int Network::numLayers() const
  {
    return m_layers.size();
  }
  QList<Layer *> Network::layers() const
  {
    return m_layers;
  }
  TrainingParameters* Network::trainParam() const
  {
    return m_trainParam;
  }
  TrainData* Network::trainData() const
  {
    return m_trainData;
  }
}
#endif
