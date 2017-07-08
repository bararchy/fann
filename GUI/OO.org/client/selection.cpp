#include <QString>

#include <cassert>

#include "selection.h"
#include "office_client.h"

Selection::Selection(OfficeClient *client) :
    sheet(-1), startR(-1), startC(-1), endR(-1), endC(-1), valid(false), data(0), client(client)
{

}

Selection::~Selection()
{
    resetData();
}

QString Selection::selectionName()
{
    if (valid)
    {
        return QString("Sheet ") + QString::number(sheet+1) + " " +
            char('A'+startC) + QString::number(startR+1) + ":" +
            char('A'+endC) + QString::number(endR+1);
    }
    else
    {
        return "Not assigned";
    }
}

int Selection::numColumns()
{
    if (valid == 0)
    {
        return -1;
    }
    return endC - startC + 1;
}

double** Selection::getData()
{
    if (!valid)
    {
        return 0;
    }
    if (!data)
    {
        int const rows = numRows();
        int const cols = numColumns();
        if ((data = new (std::nothrow) double*[rows]) == 0)
            return 0;

        for (int i = 0; i < rows; ++i)
        {
            data[i] = new (std::nothrow) double[cols];
            if (data[i])
            {
                for (int j = 0; j < cols; ++j)
                {
                    data[i][j] = client->readFromCell(sheet, i, j).toDouble();
                }
            }
            else
            {
                for (int j = 0; j < i; ++j)
                {
                    delete [] data[i];
                }
                delete [] data;
                return 0;
            }
        }
    }
    return data;
}

int Selection::numRows()
{
    if (valid == 0)
    {
        return -1;
    }
    return endR - startR + 1;
}

int Selection::getSheet()
{
    return sheet;
}

int Selection::getStartR()
{
    return startR;
}

int Selection::getStartC()
{
    return startC;
}

int Selection::getEndR()
{
    return endR;
}

int Selection::getEndC()
{
    return endC;
}

bool Selection::isValid()
{
    return valid;
}

void Selection::setSheet(int sheet)
{
    assert(sheet >=0 && "Illegal sheet number");
    if (valid && sheet != this->sheet)
    {
        resetData();
    }
    this->sheet = sheet;
}

void Selection::setStartR(int startR)
{
    assert(startR >=0 && "Illegal row number");
    if (valid && startR != this->startR)
    {
        resetData();
    }
    this->startR = startR;
}

void Selection::setStartC(int startC)
{
    assert(startC >=0 && "Illegal column number");
    if (valid && startC != this->startC)
    {
        resetData();
    }
    this->startC = startC;
}

void Selection::setEndR(int endR)
{
    assert(endR >=0 && "Illegal row number");
    if (valid && endR != this->endR)
    {
        resetData();
    }
    this->endR = endR;
}

void Selection::setEndC(int endC)
{
    assert(endC >=0 && "Illegal column number");
    if (valid && endC != this->endC)
    {
        resetData();
    }
    this->endC = endC;
}

void Selection::setValid(bool valid)
{
    if (!valid)
    {
        resetData();
    }
    this->valid = valid;
}

void Selection::setData(double **data)
{
    resetData();
    this->data = data;
}

void Selection::resetData()
{
    if (!data)
    {
        return;
    }
    for (int i = 0; startR + i < endR; ++i)
    {
        delete [] data[i];
    }
    delete [] data;
    data = 0;
}
