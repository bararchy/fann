#ifndef SELECTION_H
#define SELECTION_H

class OfficeClient;

class Selection
{
public:
    Selection(OfficeClient *client);
    ~Selection();
    QString selectionName();
    int numColumns();
    int numRows();

    int getSheet();
    int getStartR();
    int getStartC();
    int getEndR();
    int getEndC();
    bool isValid();
    double** getData();
    
    void setSheet(int sheet);
    void setStartR(int startR);
    void setStartC(int startC);
    void setEndR(int endR);
    void setEndC(int endC);
    void setValid(bool valid);
    void setData(double **data);
    void resetData();

private:
    int sheet;
    int startR;
    int startC;
    int endR;
    int endC;
    bool valid;
    double **data;
    OfficeClient *client;
};

#endif // SELECTION_H
