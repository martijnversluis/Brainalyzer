#ifndef FANNTESTRUNNER_H
#define FANNTESTRUNNER_H

#include <QString>
#include <QTime>
#include <QFile>
#include <QTextStream>

#include <iostream>
#include <iomanip>
#include <doublefann.h>

#include "raweegcapture.h"
#include "eegmarkerfile.h"
#include "raweegcapture.h"
#include "progressindicator.h"

using namespace std;

#define STIMULUS_COUNT      3
#define CHANNEL             "FC1"
#define MARKER_TYPE         "Stimulus"
#define NUM_SESSIONS        1
#define SAMPLING_INTERVAL   2
#define TIME_FRAME          150
#define OFFSET              50

#define CAPTURE_FILE_NAME   "data/1_30_Raw Data.wsv"
#define MARKER_FILE_NAME    "data/1_30.vmrk"
#define TRAIN_FILE_NAME     "data/train.data"
#define NN_FILE_NAME        "data/nn.net"

#define DEBUG   true
#define debug   if (DEBUG) qDebug() <<

class FannTestRunner {

public:
    FannTestRunner();
    int create();
    int train();
    int run();
    int mse();

private:
    struct fann* ann;
    RawEegCapture* rawCapture;
    EegMarkerFile* markerFile;
    QVector<Marker> markers;
    unsigned markerCount;
    int seed;
    QTime epochTime;

    void createSimpleFann();
    unsigned encodeStimulusType(QString description);
    QString decodeStimulusType(fann_type* output);

};

#endif
