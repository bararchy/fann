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
#include "inputdatagroup.h"

InputDataGroup::InputDataGroup( QWidget *parent, Qt::WindowFlags flags )
    : QLabel( parent, flags ),
    m_groupname( "not assigned" ),
    m_source( "not assigned" ),
    m_datacount( 0 )
{
  setLineWidth( 1 );
  setFrameStyle( QFrame::Box | QFrame::Plain );
  writeLabelText();
}

InputDataGroup::InputDataGroup( QString groupName, QString source, int dataCount, QWidget *parent, Qt::WindowFlags flags )
    : QLabel( parent, flags ),
    m_groupname( groupName ),
    m_source( source ),
    m_datacount( dataCount )
{
  setLineWidth( 1 );
  setFrameStyle( QFrame::Box | QFrame::Plain );
  writeLabelText();
}

InputDataGroup::~InputDataGroup()
{}


int InputDataGroup::datacount() const
{
  return m_datacount;
}
void InputDataGroup::setDatacount( int value )
{
  m_datacount = value;
  writeLabelText();
}

QString InputDataGroup::source() const
{
  return m_source;
}
void InputDataGroup::setSource( const QString& value )
{
  m_source = value;
  writeLabelText();
}

QString InputDataGroup::groupname() const
{
  return m_groupname;
}
void InputDataGroup::setGroupname( const QString& value )
{
  m_groupname = value;
  writeLabelText();
}

void InputDataGroup::writeLabelText()
{
  QString tmp;
  setText( "<b>Group</b>: " + m_groupname + "<br>" +
           "# data: " + ( new QString() )->setNum( m_datacount ) + "<br>" +
           "Source: " + m_source );
}
