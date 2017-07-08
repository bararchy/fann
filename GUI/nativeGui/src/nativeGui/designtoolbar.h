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
#ifndef DESIGNTOOLBAR_H
#define DESIGNTOOLBAR_H

#include <QToolBar>
#include <QList>

/**
 * @author Yusef Maali <contact@yusefmaali.net>
 */
class DesignToolBar : public QToolBar
{
  public:
    DesignToolBar( QWidget *parent = 0 );
    ~DesignToolBar();

  public:
    void hide();
    void show();

  private:
    /* Store the visibility status of all actions added into DesignToolBar */
    QList<bool> actionStatus;
};

#endif
