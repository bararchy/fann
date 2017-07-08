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
#include "mainwindow.h"
#include "inputdatagroup.h"
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags ), Ui::MainWindow()
{
  setupUi( this );

  /* QActionGroup for exclusive QAction */
  m_modalityGroup = new QActionGroup( this );
  m_modalityGroup->addAction( actionDesign_View );
  m_modalityGroup->addAction( actionTrain_View );
  m_modalityGroup->addAction( actionTest_View );

  /* Main QStackedWidget */
  m_stackedWidget = new MainStackedWidget( centralwidget );
  vboxLayout->addWidget( m_stackedWidget );

  /* NetworkManager */
  m_netManager = new NetworkManager( m_stackedWidget->graphWidget() );

  /* ToolBar for train view */
  m_trainToolBar = new QToolBar( this );
  m_trainToolBar->setOrientation( Qt::Horizontal );
  m_trainToolBar->setWindowTitle( tr( "Train Bar" ) );
  m_trainToolBar->addAction( actionStart_Train );
  m_trainToolBar->addAction( actionStop_Train );
  m_trainToolBar->setVisible( false );
  addToolBar( Qt::TopToolBarArea, m_trainToolBar );

  /* ToolBar for test view */
  m_testToolBar = new QToolBar( this );
  m_testToolBar->setOrientation( Qt::Horizontal );
  m_testToolBar->setWindowTitle( tr( "Test Bar" ) );
  m_testToolBar->addAction( actionTest_Network );
  m_testToolBar->setVisible( false );
  addToolBar( Qt::TopToolBarArea, m_testToolBar );

  /* QDockWidget for choosing input data */
  m_inputDock = new QDockWidget( tr( "Input Data" ), this );
  QWidget *tmpWidget = new QWidget( m_inputDock );

  QWidget *tmpDataWidget = new QWidget( tmpWidget );
  m_inputDockLayout = new QVBoxLayout( tmpDataWidget );

  QVBoxLayout *inputDockLayout =  new QVBoxLayout( tmpWidget );
  QPushButton *button = new QPushButton( tr("Edit Data Group"), tmpWidget );
  button->setEnabled( false );
  inputDockLayout->addWidget( button );
  inputDockLayout->addWidget( tmpDataWidget );
  inputDockLayout->addItem( new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding ) );

  m_inputDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  m_inputDock->setWidget( tmpWidget );
  m_inputDock->setVisible( false );
  addDockWidget( Qt::LeftDockWidgetArea, m_inputDock );

  /* QDockWidget for choosing train parameters */
  m_trainDock = new QDockWidget( tr( "Train Parameters" ), this );
  m_trainPropertyWidget = new TrainPropertyWidget( m_trainDock );
  m_trainDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  m_trainDock->setWidget( m_trainPropertyWidget );
  m_trainDock->setVisible( false );
  addDockWidget( Qt::RightDockWidgetArea, m_trainDock );

  /* QDockWidget for choosing output property */
//   m_outputDock = new QDockWidget( tr( "Output Property" ), this );
//   m_outputPropertyWidget = new OutputPropertyWidget( m_outputDock );
//   m_outputDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
//   m_outputDock->setWidget( m_outputPropertyWidget );
//   m_outputDock->setVisible( false );
//   addDockWidget( Qt::RightDockWidgetArea, m_outputDock );

  /* QDockWidget for editing neurons property */
  m_propertyDock = new QDockWidget( tr( "Neuron Parameters" ), this );
  m_neuronPropertyWidget = new NeuronPropertyWidget( m_propertyDock );
  m_propertyDock->setAllowedAreas( Qt::AllDockWidgetAreas );
  m_propertyDock->setVisible( false );
  m_propertyDock->setWidget( m_neuronPropertyWidget );
  addDockWidget( Qt::RightDockWidgetArea, m_propertyDock );

  /* Populate View Menu */
  menuView->addAction( m_inputDock->toggleViewAction() );
  menuView->addAction( m_trainDock->toggleViewAction() );
//   menuView->addAction( m_outputDock->toggleViewAction() );
  menuView->addAction( m_propertyDock->toggleViewAction() );

  /* Pre-check some actions */
  m_propertyDock->toggleViewAction()->activate( QAction::Trigger );
  trainViewStatus << m_trainDock->toggleViewAction();

  /* Set connections */
  connect( m_netManager, SIGNAL( neuronSelectionChanged( DS::Neuron * ) ), m_neuronPropertyWidget, SLOT( updateValues( DS::Neuron * ) ) );
  connect( m_netManager, SIGNAL( neuronTypeChanged( DS::Neuron * ) ), m_neuronPropertyWidget, SLOT( updateValues( DS::Neuron * ) ) );
  connect( m_netManager, SIGNAL( trainingParametersChanged( DS::TrainingParameters * ) ), m_trainPropertyWidget, SLOT( updateValues( DS::TrainingParameters * ) ) );
  connect( m_netManager, SIGNAL( epochTrained( DS::Network::TrainEpochValues ) ), m_stackedWidget->trainWidget(), SLOT( updateTrainEpochValues( DS::Network::TrainEpochValues ) ) );
  connect( m_netManager, SIGNAL( trainDataChanged( DS::TrainData * ) ), m_stackedWidget->testWidget(), SLOT( changeTrainData( DS::TrainData * ) ) );
  connect( m_netManager, SIGNAL( networkTested( DS::Network::TestNetworkValues ) ), m_stackedWidget->testWidget(), SLOT( showNetworkTest( DS::Network::TestNetworkValues ) ) );
  connect( m_netManager, SIGNAL( trainStopped() ), this, SLOT( on_actionStop_Train_triggered() ) );
  connect( m_netManager, SIGNAL( trainFinished() ), this, SLOT( on_actionStop_Train_triggered() ) );
  connect( m_neuronPropertyWidget, SIGNAL( changeNeuronTypeRequested( DS::Neuron *, const QString & ) ), m_netManager, SLOT( changeNeuronType( DS::Neuron *, const QString & ) ) );
  connect( actionStart_Train, SIGNAL( triggered() ), m_netManager, SLOT( startTraining() ) );
  connect( actionStop_Train, SIGNAL( triggered() ), m_netManager, SLOT( stopTraining() ) );
  connect( actionTest_Network, SIGNAL( triggered() ), m_netManager, SLOT( testNetwork() ) );
  connect( actionExit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
  connect( actionAbout_Qt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

MainWindow::~MainWindow()
{}

void MainWindow::on_actionDesign_View_triggered()
{
  const int prevView = m_stackedWidget->currentIndex();

  m_stackedWidget->setCurrentIndex( 0 );
  m_trainToolBar->hide();
  m_testToolBar->hide();
  actionZoom_In->setEnabled( true );
  actionZoom_Out->setEnabled( true );

  switch( prevView )
  {
    case 1:
      saveDockStatus( trainViewStatus );
      break;
    case 2:
      saveDockStatus( testViewStatus );
      break;
  }
  restoreDockStatus( designViewStatus );
}

void MainWindow::on_actionTrain_View_triggered()
{
  const int prevView = m_stackedWidget->currentIndex();

  m_stackedWidget->setCurrentIndex( 1 );
  m_trainToolBar->show();
  m_testToolBar->hide();
  actionZoom_In->setEnabled( false );
  actionZoom_Out->setEnabled( false );

  switch( prevView )
  {
    case 0:
      saveDockStatus( designViewStatus );
      break;
    case 2:
      saveDockStatus( testViewStatus );
      break;
  }
  restoreDockStatus( trainViewStatus );
}

void MainWindow::on_actionTest_View_triggered()
{
  const int prevView = m_stackedWidget->currentIndex();

  m_stackedWidget->setCurrentIndex( 2 );
  m_trainToolBar->hide();
  m_testToolBar->show();
  actionZoom_In->setEnabled( false );
  actionZoom_Out->setEnabled( false );

  switch( prevView )
  {
    case 0:
      saveDockStatus( designViewStatus );
      break;
    case 1:
      saveDockStatus( trainViewStatus );
      break;
  }
  restoreDockStatus( testViewStatus );
}

void MainWindow::on_actionZoom_In_triggered()
{
  m_stackedWidget->graphWidget()->scaleView( 1.2 );
}
void MainWindow::on_actionZoom_Out_triggered()
{
  m_stackedWidget->graphWidget()->scaleView( 0.8 );
}

void MainWindow::on_actionStart_Train_triggered()
{
  actionStart_Train->setEnabled( false );
  actionStop_Train->setEnabled( true );
}

void MainWindow::on_actionStop_Train_triggered()
{
  actionStart_Train->setEnabled( true );
  actionStop_Train->setEnabled( false );
}

void MainWindow::on_actionOpen_Network_triggered()
{
  QString fileName = QFileDialog::getOpenFileName( this, tr("Open Network") );
  if( fileName.isNull() )
    return;
  m_netManager->createNet( new DS::TrainData( fileName ) );
  m_inputDockLayout->addWidget( new InputDataGroup( "DataSet", fileName, m_netManager->net()->trainData()->numData(), m_inputDockLayout->parentWidget() ) );
}

void MainWindow::restoreDockStatus( const QList<QAction *> &list )
{
  foreach( QAction *action, list )
    action->activate( QAction::Trigger );
}

void MainWindow::saveDockStatus( QList<QAction *> &list )
{
  //TODO replace this code with a session manager
  QList<QAction *> tmpList;
  tmpList.append( m_inputDock->toggleViewAction() );
  tmpList.append( m_trainDock->toggleViewAction() );
//   tmpList.append( m_outputDock->toggleViewAction() );
  tmpList.append( m_propertyDock->toggleViewAction() );

  list.clear();
  foreach( QAction *action, tmpList )
  {
    if( action->isChecked() )
    {
      list << action;
      action->activate( QAction::Trigger ); /* uncheck the action */
    }
  }
}
