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
#ifndef INPUTDATAGROUP_H
#define INPUTDATAGROUP_H

#include <QLabel>

class InputDataGroup : public QLabel
{
  public:
    InputDataGroup( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    InputDataGroup( QString groupName, QString source, int dataCount, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~InputDataGroup();

  public:
    void setDatacount( int value );
    int datacount() const;

    void setSource( const QString& value );
    QString source() const;

    void setGroupname( const QString& value );
    QString groupname() const;

  private:
    QString m_groupname;
    QString m_source;
    int m_datacount;

  private:
    void writeLabelText();
};

#endif
