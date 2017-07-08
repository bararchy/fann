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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QDockWidget>
#include <QList>
#include <QMainWindow>

#include "networkmanager.h"

#include "mainstackedwidget.h"
#include "neuronpropertywidget.h"
#include "outputpropertywidget.h"
#include "trainpropertywidget.h"

#include "ui_main_window.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

  public:
    MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~MainWindow();

  private slots:
    void on_actionDesign_View_triggered();
    void on_actionTrain_View_triggered();
    void on_actionTest_View_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionStart_Train_triggered();
    void on_actionStop_Train_triggered();
    void on_actionOpen_Network_triggered();

  private:
    NetworkManager *m_netManager;

    MainStackedWidget *m_stackedWidget;
    NeuronPropertyWidget *m_neuronPropertyWidget;
    OutputPropertyWidget *m_outputPropertyWidget;
    TrainPropertyWidget *m_trainPropertyWidget;

    QActionGroup *m_modalityGroup;
    QDockWidget *m_inputDock;
    QDockWidget *m_trainDock;
    QDockWidget *m_outputDock;
    QDockWidget *m_propertyDock;
    QToolBar *m_trainToolBar;
    QToolBar *m_testToolBar;

    QVBoxLayout *m_inputDockLayout;

    /* Used to store which actions are triggered when the user shows another view */
    QList<QAction *> designViewStatus;
    QList<QAction *> trainViewStatus;
    QList<QAction *> testViewStatus;

  private:
    void restoreDockStatus( const QList<QAction *> &list );
    void saveDockStatus( QList<QAction *> &list );
};

#endif
