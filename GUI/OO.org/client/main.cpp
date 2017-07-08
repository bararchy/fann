#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QString>
#include <QtCore>
#include <QTranslator>
#include <QProcess>

#include <iostream>
#include <cstdlib>
#include <cassert>

#include <doublefann.h>
#include <fann_cpp.h>

#include "main_window.h"
#include "network_console.h"

int startApp(int& argc, char *argv[], QString suiteName);
void printHelp();
QString pluginDir();

const char* const appName = "fann_plugin";

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        if (QString("--detached") == argv[1])
        {
            QProcess::startDetached(pluginDir() + appName + " --standalone " + argv[2]);
            return EXIT_SUCCESS;
        }
        else if (QString("--standalone") == argv[1])
        {
            if (QString("openoffice.org") ==  argv[2] || QString("msoffice") == argv[2])
            {
                return startApp(argc, argv, argv[1]);
            }
            else
            {
                printHelp();
                return EXIT_SUCCESS;
            }
        }

    }
    else if (argc == 1)
    {
        return startApp(argc, argv, "openoffice.org");
    }
    else
    {
        printHelp();
        return EXIT_SUCCESS;
    }
}

QString pluginDir()
{
    const char *dir = std::getenv("OFFICE_HOME");
    assert(dir);
    return QString(dir) + "fann/";
}

int startApp(int &argc, char* argv[], QString suiteName)
{
    QApplication app(argc, argv);

    QTranslator translator;
    QString locale = QLocale::system().name();

    translator.load(QString(appName) + "_" + locale);
    app.installTranslator(&translator);

    try 
    {
        MainWindow mainWindow(suiteName);
        mainWindow.show();
        return app.exec();
    }
    catch (const std::exception &e)
    {
        QString msg = QT_TRANSLATE_NOOP("main", "Unable to start the plugin: ");
        QMessageBox::critical(0, QT_TRANSLATE_NOOP("main", "Eror"), msg + e.what());
        exit(EXIT_FAILURE);
    }
}

void printHelp()
{
    std::cout << "possible options: \n"
        << appName << "--detached --openoffice.org\n"
        // << appName <<  --detached --msoffice\n"
        << appName << std::endl;
}
