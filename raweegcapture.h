#ifndef RAWEEGCAPTURE_H
#define RAWEEGCAPTURE_H

#include "textdatafile.h"

#include <QVector>
#include <QStringList>
#include <QDebug>

class RawEegCapture : public TextDataFile {

public:
    RawEegCapture(QString filename);
    QStringList columnNames();
    QVector<double> readData(int columnNumber, int start, int stop);
    QVector<double> readData(QString columnName, int start, int stop);
    double read();
    double read(int columnNumber);
    double read(QString columnName);
    void selectColumn(QString columnName);
    void selectColumn(int columnNumber);

private:
    void readColumnNames();
    int columnNumberFor(QString columnName);

    QVector<double> data;
    QStringList colNames;
    int selectedColumn;

};

#endif
