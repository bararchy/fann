#ifndef WEIGHTS_H
#define WEIGHTS_H

#include <QDialog>

#include "ui_weights.h"

class QCloseEvent;
namespace FANN { class neural_net; }
struct connection;

class Weights : public QDialog, public Ui::Weights
{
public:
    Weights(QWidget *parent=0, FANN::neural_net *net=0, Qt::WindowFlags flags=0);
    
protected:
    void closeEvent(QCloseEvent *event);

private:
    FANN::neural_net *net;
    FANN::connection *connections;
};


#endif // WEIGHTS_H
