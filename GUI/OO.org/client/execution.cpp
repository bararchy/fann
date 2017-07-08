#include <QMessageBox>
#include <QProgressDialog>

#include <cassert>
#include <iostream>

#include <doublefann.h>
#include <fann_cpp.h>

#include "execution.h"
#include "office_client.h"
#include "data_chooser.h"
#include "util.h"

Execution::Execution( QWidget *parent, FANN::neural_net *net, OfficeClient *client_, Qt::WindowFlags flags) :
    QWidget(parent, flags), Ui::Execution(), net(net), client(client_), maxEpochs(0), desiredMSE(0),
    trainingInputsSelection(client_),
    trainingReferenceSelection(client_),
    trainingErrorSelection(client_),
    testInputsSelection(client_),
    testReferenceSelection(client_),
    testErrorSelection(client_),
    testOutputsSelection(client_)
{
    assert(net && "Illegal program state, net uninitialized");
    setupUi( this );

    connect(trainingInputs, SIGNAL(clicked(bool)), this, SLOT(trainingInputsClicked()));
    connect(trainingReference, SIGNAL(clicked(bool)), this, SLOT(trainingReferenceClicked()));
    connect(trainingError, SIGNAL(clicked(bool)), this, SLOT(trainingErrorClicked()));

    connect(testInputs, SIGNAL(clicked(bool)), this, SLOT(testInputsClicked()));
    connect(testReference, SIGNAL(clicked(bool)), this, SLOT(testReferenceClicked()));
    connect(testError, SIGNAL(clicked(bool)), this, SLOT(testErrorClicked()));
    connect(testOutputs, SIGNAL(clicked(bool)), this, SLOT(testOutputsClicked()));
    
    connect(trainButton, SIGNAL(clicked(bool)), this, SLOT(runTraining()));
    connect(testButton, SIGNAL(clicked(bool)), this, SLOT(runTest()));
    connect(resetErrorButton, SIGNAL(clicked()), this, SLOT(resetError()));
}

void Execution::trainingInputsClicked()
{
    DataChooser(this, trainingInputsSelection, client).exec();
    trainingInputs->setText(trainingInputsSelection.selectionName());
}

void Execution::trainingReferenceClicked()
{
    DataChooser(this, trainingReferenceSelection, client).exec();
    trainingReference->setText(trainingReferenceSelection.selectionName());
}

void Execution::trainingErrorClicked()
{
    DataChooser(this, trainingErrorSelection, client).exec();
    trainingError->setText(trainingErrorSelection.selectionName());
}

void Execution::testInputsClicked()
{
    DataChooser(this, testInputsSelection, client).exec();
    testInputs->setText(testInputsSelection.selectionName());
}

void Execution::testReferenceClicked()
{
    DataChooser(this, testReferenceSelection, client).exec();
    testReference->setText(testReferenceSelection.selectionName());
}

void Execution::testErrorClicked()
{
    DataChooser(this, testErrorSelection, client).exec();
    testError->setText(testErrorSelection.selectionName());
}

void Execution::testOutputsClicked()
{
    DataChooser(this, testOutputsSelection, client).exec();
    testOutputs->setText(testOutputsSelection.selectionName());
}

void Execution::runTest()
{
    assert(net && "Invalid program state, net uninitialized");
    if (isTestSelectionValid() == 0)
    {
        return;
    }
    emit preparingToTest(this);

    double **inputPatterns = testInputsSelection.getData();
    double **referencePatterns = testReferenceSelection.getData();
    const int numPatterns = trainingInputsSelection.numRows();

    resetErrorButton->setDisabled(true);
    const bool test = testReferenceSelection.isValid();

    for (int i = 0; i < numPatterns; ++i)
    {
        if (test)
        {
            double *outputs = net->test(inputPatterns[i], referencePatterns[i]);
            Selection& es = testErrorSelection;
            if (es.isValid() && (i < es.numRows()))
            {
                client->writeToCell(es.getSheet(), es.getStartR() + i,
                        es.getStartC(), net->get_MSE());
            }
            Selection& os = testOutputsSelection;
            if (os.isValid() && (i < os.numRows()))
            {
                for (int j = 0; j < os.numColumns(); ++j)
                {
                    client->writeToCell(os.getSheet(), os.getStartR() + i,
                            os.getStartC() + j, outputs[j]);
                }
            }
        }
        else // run
        {
            double *outputs = net->run(inputPatterns[i]);
            Selection& os = testOutputsSelection;
            if (os.isValid() && (i < os.numRows()))
            {
                for (int j = 0; j < os.numColumns(); ++j)
                {
                    client->writeToCell(os.getSheet(), os.getStartR() + i,
                            os.getStartC() + j, outputs[j]);
                }
            }
        }
    }
    resetErrorButton->setEnabled(true);
}

bool Execution::isTestSelectionValid()
{
    const int numInput = net->get_num_input();
    const int numReference = net->get_num_output();
    const int numOutput = numReference;
    // no inputs
    if (testInputsSelection.isValid() == 0)
    {
        QMessageBox::warning(this, tr("Information"), tr("Select input columns"));
        return false;
    }
    // error calculation requires reference outputs
    if (testErrorSelection.isValid())
    {
        if (testReferenceSelection.isValid() == 0)
        {
            QMessageBox::warning(this, tr("Information"),
                tr("Select reference columns to calculate error"));
            return false;
        }
    }
    // we need at least one output column, output neurons or errors
    if (testOutputsSelection.isValid()  == 0 && testErrorSelection.isValid() == 0)
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select actual output or error columns"));
        return false;
    }
    // input columns must match input neurons
    if (numInput != testInputsSelection.numColumns())
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected inputs columns must match net inputs"));
        return false;
    }
    // reference columns must match output neurons
    if (testReferenceSelection.isValid() && (numReference != testReferenceSelection.numColumns()))
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected reference columns must match net outputs"));
        return false;
    }
    // output columns must match output neurons
    if (testOutputsSelection.isValid() && (numOutput != testOutputsSelection.numColumns()))
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected ouput columns must match net outputs"));
        return false;
    }
    if (testReferenceSelection.isValid() &&
           (testInputsSelection.numRows() != testReferenceSelection.numRows()))
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select equal number of input and reference patterns"));
        return false;
    }
    return true;
}

void Execution::runTraining()
{
    assert(net && "Illegal program state, netuninitialized");
    if (isTrainingSelectionValid() == 0)
    {
        return;
    }
    emit preparingToTrain(this);

    double **inputPatterns = trainingInputsSelection.getData();
    double **referenceOutputs = trainingReferenceSelection.getData();
    const int numPatterns = trainingInputsSelection.numRows();

    FANN::training_data data;
    data.set_train_data(numPatterns, net->get_num_input(), inputPatterns,
            net->get_num_output(), referenceOutputs);

    resetErrorButton->setDisabled(true);
    for (int i = 0; i < maxEpochs; ++i)
    {
        Selection& s = trainingErrorSelection;
        std::cout << i << std::endl;
        double mse = net->train_epoch(data);
        if (s.isValid() && (i < s.numRows()))
        {
            client->writeToCell(s.getSheet(), s.getStartR() + i, s.getStartC(), mse);
        }
        if (mse < desiredMSE)
        {
            QMessageBox::information(this, tr("Information"),
                    tr("Net has reached the desired MSE of ") + QString::number(mse) +
                    tr(" after ") + QString::number(i) + tr(" epochs."));
            break;
        }
    }
    resetErrorButton->setEnabled(true);
}

bool Execution::isTrainingSelectionValid()
{
    const int numInput = net->get_num_input();
    const int numReference = net->get_num_output();
    if (trainingInputsSelection.isValid() == 0)
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select inputs columns"));
        return false;
    }
    if (trainingReferenceSelection.isValid() == 0)
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select reference columns"));
        return false;
    }
    if (numInput != trainingInputsSelection.numColumns())
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected inputs columns must match net inputs"));
        return false;
    }
    if (numReference != trainingReferenceSelection.numColumns())
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected reference columns must match net outputs"));
        return false;
    }
    if (trainingInputsSelection.numRows() != trainingReferenceSelection.numRows())
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select equal number of input and reference patterns"));
        return false;
    }
    return true;
}

void Execution::resetError()
{
    net->reset_MSE();
    currentErrorLabel->setText("0");
}

void Execution::initializeWeights()
{
    assert(net && "Invalid program state, net uninitialized");
    const int numInputs = net->get_num_input();
    if (trainingInputsSelection.isValid() == 0)
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Select inputs columns for Training in Execution tab to initialize weights"));
        return;
    }
    if (numInputs != trainingInputsSelection.numColumns())
    {
        QMessageBox::warning(this, tr("Information"),
            tr("Selected inputs columns must match net inputs"));
        return;
    }

    double **inputPatterns = trainingInputsSelection.getData();
    FANN::training_data data;
    data.set_train_data(trainingInputsSelection.numRows(), numInputs, inputPatterns, 0, 0);
    net->init_weights(data);
}

void Execution::setMaxEpochs(int maxEpochs)
{
    assert(maxEpochs >= 0 && "Illegal program state, negative maxEpochs");
    this->maxEpochs = maxEpochs;
}

void Execution::setDesiredMSE(double desiredMSE)
{
    assert(desiredMSE >= 0 && "Illegal program state, negative desiredMSE");
    this->desiredMSE = desiredMSE;
}
