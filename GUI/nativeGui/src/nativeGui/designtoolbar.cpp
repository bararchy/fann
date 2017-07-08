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
#include "designtoolbar.h"

#include <QListIterator>
#include <QAction>

DesignToolBar::DesignToolBar( QWidget *parent )
    : QToolBar( parent )
{}

DesignToolBar::~DesignToolBar()
{}

void DesignToolBar::hide()
{
  QListIterator<QAction*> it( this->actions() );
  QAction *tmp;

  actionStatus.clear();
  while ( it.hasNext() ) {
    tmp = it.next();
    actionStatus.append( tmp->isChecked() );

    /* Uncheck the action */
    if ( tmp->isChecked() )
      tmp->activate( QAction::Trigger );
  }

  QToolBar::hide();
}

void DesignToolBar::show()
{
  QListIterator<QAction*> it( this->actions() );
  QListIterator<bool> it2( actionStatus );
  QAction *tmp;

  while ( it.hasNext() ) {
    tmp = it.next();
    if ( it2.next() )
      tmp->activate( QAction::Trigger );
  }

  QToolBar::show();
}
