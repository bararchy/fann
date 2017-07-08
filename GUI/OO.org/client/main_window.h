#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include <doublefann.h>
#include <fann_cpp.h>

#include "ui_main_window.h"

class NetworkOverview;
class Execution;
class TrainingSettings;
class OfficeClient;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QString suiteName, QWidget *parent = 0, Qt::WindowFlags flags=Qt::WindowStaysOnTopHint|Qt::WindowMinimizeButtonHint);
    FANN::neural_net *getNet();

public slots:
    void setNet(FANN::neural_net* const net);
    bool newNetwork();
    bool openNetwork();
    bool saveNetwork();
    bool assureDecision();
    void aboutApplication();
    void propagateTrainingSettings();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createTabs();

    QWidget *centralwidget;
    QTabWidget *tabs;
    QWidget *overviewTab;
    QWidget *trainingTab;
    QWidget *executionTab;
    NetworkOverview *networkOverview;
    Execution *execution;
    TrainingSettings *trainingSettings;

    OfficeClient *client;
    FANN::neural_net *net;
};

#endif // MAIN_WINDOW_H
