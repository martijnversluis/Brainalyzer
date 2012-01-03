#ifndef EEGMARKERFILE_H
#define EEGMARKERFILE_H

#include "textdatafile.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDebug>

struct Marker {
    int number;
    QString type;
    QString description;
    int position;
    int size;
    int channel;
};

class EegMarkerFile : public TextDataFile {

public:
    EegMarkerFile(QString filename);
    void setTypeFilter(QString filter);
    QVector<Marker> markers(QString typeFilter);

};

#endif
