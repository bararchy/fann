#ifndef OFFICE_CLIENT_H
#define OFFICE_CLIENT_H

#include <QString>
#include "selection.h"
#include <exception>
#include <stdexcept>

class OfficeClient
{
public:
    virtual ~OfficeClient() {};

    virtual bool openConnection() = 0;
    virtual bool closeConnection() = 0;
    virtual bool openDocument() = 0;


    virtual bool writeToCell(int sheetNum, int row, int column, QString value) = 0;
    virtual bool writeToCell(int sheetNum, int row, int column, double value) = 0;
    virtual bool fillSelection(Selection& s) = 0;
    virtual QString readFromCell(int sheetNum, int row, int column) = 0;
    
    class ConnectionFailureException : public std::exception
    { };

    class RuntimeFailureException : public std::runtime_error
    {
        RuntimeFailureException(const char *msg) : std::runtime_error(msg)
        { }
    };
};

#endif // OFFICE_CLIENT_H
