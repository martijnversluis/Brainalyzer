#include "eegmarkerfile.h"

EegMarkerFile::EegMarkerFile(QString filename):
    TextDataFile(filename)
{
    while (!this->readLine().contains("[Marker Infos]"));
}

QVector<Marker> EegMarkerFile::markers(QString typeFilter = "") {
    QVector<Marker> markers;

    while (!this->atEnd()) {
        QString line = this->readLine();

        if (!line.startsWith(';')) {
            QStringList parts = line.split(",");
            QStringList nameAndType = parts[0].split('=');

            if (typeFilter.length() > 0 && typeFilter == nameAndType[1]) {
                Marker marker;
                marker.number = nameAndType[0].mid(2).toInt();
                marker.type = nameAndType[1];
                marker.description = parts[1];
                marker.position = parts[2].toInt();
                marker.size = parts[3].toInt();
                marker.channel = parts[4].toInt();

                markers.push_back(marker);
            }
        }
    }

    return markers;
}
