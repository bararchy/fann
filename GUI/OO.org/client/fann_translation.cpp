#include <QtCore>

#include "fann_translation.h"


namespace FANN
{

    char const *const TRAINING_ALGORITHMS[] =
    {
        QT_TRANSLATE_NOOP("TrainingAlgorithms", "Incremental"),
        QT_TRANSLATE_NOOP("TrainingAlgorithms", "Batch"),
        QT_TRANSLATE_NOOP("TrainingAlgorithms", "Reverse Propagation"),
        QT_TRANSLATE_NOOP("TrainingAlgorithms", "Quick Propagation")
    };

    char const *const ACTIVATION_FUNCTIONS[] =
    {
        QT_TRANSLATE_NOOP("ActivationFunctions", "Linear"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Threshold"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Threshold Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sigmoid"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sigmoid Stepwise"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sigmoid Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sigmoid Symmetric Stepwise"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Gaussian"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Gaussian Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Gaussian Stepwise"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Elliot"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Elliot Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Linear Piece"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Linear Piece Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sin Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Cos Symmetric"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Sin"),
        QT_TRANSLATE_NOOP("ActivationFunctions", "Cos")
    };

    char const *const ERROR_FUNCTIONS[] =
    {
        QT_TRANSLATE_NOOP("ErrorFunctions", "Linear"),
        QT_TRANSLATE_NOOP("ErrorFunctions", "TanH")
    };

    char const *const STOP_FUNCTIONS[] =
    {
        QT_TRANSLATE_NOOP("StopFunctions", "Mean Square Error"),
        QT_TRANSLATE_NOOP("StopFunctions", "Bit")
    };

    char const *const NET_TYPES[] =
    {
        QT_TRANSLATE_NOOP("NetTypes", "Layer"),
        QT_TRANSLATE_NOOP("NetTypes", "Shortcut")
    };

}
