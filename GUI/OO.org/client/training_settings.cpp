#include <QMessageBox>
#include <QProgressDialog>

#include <cassert>
#include <iostream>
#include <memory>

#include <doublefann.h>
#include <fann_cpp.h>


#include "fann_translation.h"
#include "data_chooser.h"
#include "execution.h"
#include "strict_double_validator.h"
#include "strict_int_validator.h"
#include "training_settings.h"
#include "util.h"

const int EPOCHS_BETWEEN_REPORTS = 1000;

TrainingSettings::TrainingSettings(QWidget *parent, FANN::neural_net *net, OfficeClient *client, Qt::WindowFlags flags)
    : QWidget(parent, flags), Ui::TrainingSettings(), net(net), client(client)
{
    assert(parent && net && client && "Invalid program state, data uninitialized");
    
    setupUi(this);
    std::auto_ptr<StrictIntValidator> tee(new StrictIntValidator(0, 1000000000, trainingEpochsEdit));
    std::auto_ptr<StrictDoubleValidator> 
        ele(new StrictDoubleValidator(-999.0, 999.0, 10, errorLimitEdit)), 
        minwe(new StrictDoubleValidator(-1000, 1000, 6, minWeightEdit)), 
        maxwe(new StrictDoubleValidator(-1000, 1000, 6, maxWeightEdit));

    populateComboBox(trainingAlgorithmCombo, FANN::TRAINING_ALGORITHMS, "TrainingAlgorithms", 4);
    trainingAlgorithmCombo->setCurrentIndex(FANN::TRAIN_RPROP);
    populateComboBox(errorFunctionCombo, FANN::ERROR_FUNCTIONS, "ErrorFunctions", 2);
    errorFunctionCombo->setCurrentIndex(FANN::ERRORFUNC_TANH);
    populateComboBox(stopFunctionCombo, FANN::STOP_FUNCTIONS, "StopFunctions", 2);
    trainingEpochsEdit->setValidator(tee.release());
    errorLimitEdit->setValidator(ele.release());
    minWeightEdit->setValidator(minwe.release());
    maxWeightEdit->setValidator(maxwe.release());

    connect(trainingAlgorithmCombo, SIGNAL(currentIndexChanged(int)),
        this, SLOT(trainingAlgorithmChanged(int)));
    connect(initializeWeightsButton, SIGNAL(clicked()), this, SIGNAL(initializeWeights()));
}

void TrainingSettings::trainingAlgorithmChanged(const int index)
{
    assert(learningRateSpin && learningMomentumSpin && "GUI uninitialized");
    assert(index >= 0 && index <= 4 && "Illegal algorithm");
    learningMomentumSpin->setEnabled(index == FANN::TRAIN_INCREMENTAL);
    learningRateSpin->setEnabled(index != FANN::TRAIN_RPROP);
}

void TrainingSettings::randomizeWeights()
{
    net->randomize_weights(minWeightEdit->text().toDouble(), maxWeightEdit->text().toDouble());
}

void TrainingSettings::saveData(Execution *execution)
{
    assert(net && "Invalid program state, net uninitialized");
    assert(execution && "execution must not be NULL");
    net->set_training_algorithm(FANN::training_algorithm_enum(trainingAlgorithmCombo->currentIndex()));
    net->set_learning_momentum(learningMomentumSpin->value());
    net->set_train_error_function(FANN::error_function_enum(errorFunctionCombo->currentIndex()));
    net->set_train_stop_function(FANN::stop_function_enum(stopFunctionCombo->currentIndex()));
    net->set_learning_rate(learningRateSpin->value());
    
    execution->setDesiredMSE(errorLimitEdit->text().toDouble());
    execution->setMaxEpochs(trainingEpochsEdit->text().toInt());
}
