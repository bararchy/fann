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
#ifndef BUFFERVECTOR_H
#define BUFFERVECTOR_H

#include <QVector>

template <class T>
class BufferVector : public QVector<T>
{
  public:
    BufferVector();
    BufferVector( int size );
    ~BufferVector();

    inline void append( const T & value );
    inline void resize( int size );

  protected:
    int m_bufsize; /* If m_bufsize <= 0, BufferVector behave as a default QVector */
};

template <class T>
BufferVector<T>::BufferVector()
    : QVector<T>(),
    m_bufsize( 0 )
{}
template <class T>
BufferVector<T>::BufferVector( int size )
    : QVector<T>( size ),
    m_bufsize( size )
{
  QVector<T>::reserve( size );
}
template <class T>
BufferVector<T>::~BufferVector()
{}

template <class T>
void BufferVector<T>::append( const T & value )
{
  if ( m_bufsize > 0 && QVector<T>::size() == m_bufsize )
    QVector<T>::remove( 0 );

  QVector<T>::append( value );
}

template <class T>
void BufferVector<T>::resize( int size )
{
  m_bufsize = size;
  QVector<T>::reserve( size );
}

#endif
