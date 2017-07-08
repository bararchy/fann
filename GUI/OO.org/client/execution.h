#ifndef EXECUTION_H
#define EXECUTION_H

#include <QWidget>

#include "ui_execution.h"
#include "office_client.h"
#include "selection.h"

namespace FANN { class neural_net; }


class Execution : public QWidget, private Ui::Execution
{
    Q_OBJECT

public:
    Execution(QWidget *parent=0, FANN::neural_net* net=0, OfficeClient* client=0, Qt::WindowFlags flags=0);

public slots:
    void trainingInputsClicked();
    void trainingReferenceClicked();
    void trainingErrorClicked();
    void testInputsClicked();
    void testReferenceClicked();
    void testErrorClicked();
    void testOutputsClicked();
    void resetError();
    void runTest();
    void runTraining();
    void initializeWeights();
    void setMaxEpochs(int maxEpochs);
    void setDesiredMSE(double desiredMSE);

signals:
    void preparingToTrain(Execution*);
    void preparingToTest(Execution*);

private:
    bool isTrainingSelectionValid();
    bool isTestSelectionValid();

    FANN::neural_net *net;
    OfficeClient *client;
    int maxEpochs;
    double desiredMSE;
    Selection trainingInputsSelection, trainingReferenceSelection, trainingErrorSelection,
        testInputsSelection, testReferenceSelection, testErrorSelection, testOutputsSelection;
};

#endif // EXECUTION_H
