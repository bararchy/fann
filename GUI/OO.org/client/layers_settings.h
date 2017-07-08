#ifndef LAYERS_SETTINGS_H
#define LAYERS_SETTINGS_H

#include <QWidget>

#include <vector>

#include "ui_layers_settings.h"

namespace FANN { class neural_net; }

class LayersSettings : public QWidget, public Ui::LayersSettings
{
    Q_OBJECT

public:
    LayersSettings(QWidget *parent=0, FANN::neural_net* net=0, bool newNet=true, Qt::WindowFlags flags=0);
    void saveData();
    void loadData();

public slots:
    void layersNumberChanged(const int number);
    void neuronsNumberChanged(const QString &numberString);
    void layersChoiceChanged(const int number);
    void networkTypeChanged(const int index);
    int getHiddenLayerNeuronsNum(const int layer);

private:
    void disableControls();

    std::vector<int> hiddenNeuronsCount;
    FANN::neural_net *net;
};

#endif  // LAYERS_SETTINGS_H
