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
#ifndef DSNEURONDESCRIPTOR_H
#define DSNEURONDESCRIPTOR_H

struct fann_neuron_descr;

namespace DS /* Data Structure */
{
  class NeuronDescriptor
  {
    public:
      NeuronDescriptor( int num_outputs );
      ~NeuronDescriptor();

      struct fann_neuron_descr* fann_neuron_descr() const;
      void set_fann_neuron_descr( struct fann_neuron_descr* value );

    protected:
      struct fann_neuron_descr* m_fann_neuron_descr;
      unsigned int m_num_outputs;
      void create_neuron_descriptor();
  };
}

#endif
