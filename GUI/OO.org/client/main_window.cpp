#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include <cassert>
#include <iostream>
#include <stdexcept>

#include <doublefann.h>
#include <fann_cpp.h>

#include "about_dialog.h"
#include "execution.h"
#include "main_window.h"
#include "network_console.h"
#include "network_overview.h"
#include "office_client.h"
#include "ooorg_client.h"
#include "training_settings.h"

OfficeClient* createOfficeSuiteClient(const QString&)
{
    try
    {
        return new OOorgClient;
    }
    catch (const OfficeClient::ConnectionFailureException& e)
    {
        return 0;
    }
    catch (const std::bad_alloc &e)
    {
        return 0;
    }
    catch (...)
    {
        return 0;
    }
}

MainWindow::MainWindow(QString suiteName,  QWidget *parent, Qt::WindowFlags flags)
      : QMainWindow(parent, flags), Ui::MainWindow(), centralwidget(0),
        client(createOfficeSuiteClient(suiteName)), net(0)
{
    if (!client)
    {
        throw std::runtime_error("Failed to create office client");
    }
    setupUi(this);

    connect(newAction, SIGNAL(triggered()), this, SLOT(newNetwork()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openNetwork()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveNetwork()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutApplication()));
}

void MainWindow::closeEvent(QCloseEvent*)
{
    if (client)
    {
        delete client;
    }
    if (net)
    {
        delete net;
    }
}

void MainWindow::createTabs()
{
    if (centralwidget)
    {
        centralwidget->close();
    }
    setFixedSize(QSize(375, 421));
    setGeometry(QRect(geometry().topLeft(), QSize(370, 416)));
    centralwidget = new QWidget(this);
    tabs = new QTabWidget(centralwidget);
    tabs->setGeometry(QRect(10, 10, 351, 370));
    overviewTab = new QWidget();
    tabs->addTab(overviewTab, tr("Overview"));
    trainingTab = new QWidget();
    tabs->addTab(trainingTab, tr("Training parameters"));
    executionTab = new QWidget();
    tabs->addTab(executionTab, tr("Execution"));
    setCentralWidget(centralwidget);
    tabs->setCurrentIndex(0);

    networkOverview = new NetworkOverview(this, net);
    QVBoxLayout *overviewLayout = new QVBoxLayout();
    overviewLayout->addWidget( networkOverview );
    overviewTab->setLayout( overviewLayout );

    trainingSettings = new TrainingSettings(this, net, client);
    QVBoxLayout *trainingLayout = new QVBoxLayout();
    trainingLayout->addWidget( trainingSettings );
    trainingTab->setLayout( trainingLayout );
    updateGeometry();
    
    execution = new Execution(this, net, client);
    QVBoxLayout *executionLayout = new QVBoxLayout();
    executionLayout->addWidget( execution );
    executionTab->setLayout( executionLayout );

    connect(execution, SIGNAL(preparingToTrain(Execution*)),
            trainingSettings, SLOT(saveData(Execution*)));
    connect(trainingSettings, SIGNAL(initializeWeights()), execution, SLOT(initializeWeights()));
    assert(networkOverview && execution && trainingSettings && "Illegal program state, tabs uninitialized");
}

void MainWindow::propagateTrainingSettings()
{
    assert(execution && "Illegal program state, tabs uninitialized");
    execution->setMaxEpochs(trainingSettings->trainingEpochsEdit->text().toInt());
    execution->setDesiredMSE(trainingSettings->errorLimitEdit->text().toInt());
}

bool MainWindow::newNetwork()
{
    if (!assureDecision())
    {
        return false;
    }
    FANN::neural_net *newNet = new FANN::neural_net;
    if (NetworkConsole (this, true, newNet).exec() == QDialog::Accepted)
    {
        setNet(newNet);
        return true;
    }
    else
    {
        delete newNet;
        return false;
    }

}

bool MainWindow::openNetwork()
{
    if (assureDecision() == 0)
    {
        return false;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open network"), "",
            tr("All Files (*);;Net Files (*.net)"));
    if (fileName.isEmpty())
    {
        return false;
    }
    FANN::neural_net *newNet = new FANN::neural_net;
    if (newNet->create_from_file(fileName.toStdString()) == 0)
    {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open network"));
        delete newNet;
        return false;
    }
    setNet(newNet);
    return true;
}

bool MainWindow::saveNetwork()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save network"), "",
            tr("All Files (*);;Net Files (*.net)"));
    if (fileName.isEmpty())
    {
        return false;
    }
    if (net->save(fileName.toStdString()) == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to save network"));
        return false;
    }
    return true;
}

void MainWindow::setNet(FANN::neural_net *const net)
{
    if (this->net)
    {
        delete this->net;
    }
    this->net = net;
    createTabs();
}

FANN::neural_net* MainWindow::getNet()
{
    return net;
}

bool MainWindow::assureDecision()
{
    if (net)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"), tr("Do you want to save your network?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            return saveNetwork();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::aboutApplication()
{
    AboutDialog(this).exec();
}
