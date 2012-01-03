#include "raweegcapture.h"

RawEegCapture::RawEegCapture(QString filename):
    TextDataFile(filename)
{
    this->readColumnNames();
}

void RawEegCapture::readColumnNames() {
    this->colNames = this->readLine().split(" ");
}

QStringList RawEegCapture::columnNames() {
    return this->colNames;
}

int RawEegCapture::columnNumberFor(QString columnName) {
    int index = this->colNames.indexOf(columnName);

    if (index == -1 || index >= this->colNames.size()) {
        qDebug() << "Failed to select " << columnName << " [" << columnName << "] from " << this->colNames.join(", ");
    }

    return index;
}

void RawEegCapture::selectColumn(QString columnName) {
    this->selectColumn(this->columnNumberFor(columnName));
}

void RawEegCapture::selectColumn(int columnNumber) {
    this->selectedColumn = columnNumber;
}

double RawEegCapture::read() {
    return this->read(this->selectedColumn);
}

double RawEegCapture::read(QString columnName) {
    return this->read(this->columnNumberFor(columnName));
}

double RawEegCapture::read(int columnNumber) {
    QString line = this->readLine();
    QStringList items = line.split(" ");

    if (columnNumber == -1 || columnNumber >= items.size()) {
        qDebug() << "Failed to select [" << columnNumber << "] from line  " << lineNumber << " (size = " << items.size() << ")";
        return 0;
    }

    return items[columnNumber].toDouble();
}

QVector<double> RawEegCapture::readData(QString columnName, int start = 0, int stop = -1) {
    int column = this->columnNumberFor(columnName);

    if (column == -1) {
        QVector<double> emptyVector;
        return emptyVector;
    }

    return this->readData(column, start, stop);
}

QVector<double> RawEegCapture::readData(int columnNumber, int start = 0, int stop = -1) {
    QVector<double> data;

    for (int i = 0; i < start; i++) {
        this->readLine();
    }

    int row = 0;
    while ((stop == -1 || row < stop) && !this->file->atEnd()) {
        data.push_back(this->read(columnNumber));
        row++;
    }

    return data;
}
