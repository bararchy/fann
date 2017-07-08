#ifndef TRAINING_SETTINGS_H
#define TRAINING_SETTINGS_H

#include <QWidget>

#include "ui_training_settings.h"
#include "office_client.h"

class Execution;

namespace FANN { class neural_net; }

class TrainingSettings : public QWidget, public Ui::TrainingSettings
{
    Q_OBJECT

public:
    TrainingSettings(QWidget *parent=0, FANN::neural_net *net=0, OfficeClient *client=0, Qt::WindowFlags flags=0);

public slots:
    void trainingAlgorithmChanged(const int index);
    void randomizeWeights();
    void saveData(Execution* execution);

signals:
    void initializeWeights();

private:
    FANN::neural_net *net;
    OfficeClient *client;
};

#endif  // TRAINING_SETTINGS_H
