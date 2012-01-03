#include "fanntestrunner.h"

QString stimulusTypes[STIMULUS_COUNT] = {"S  2", "S  3", "S  4"};
//QString stimulusTypes[STIMULUS_COUNT] = {"S  2", "S  5", "S  6"};

FannTestRunner::FannTestRunner() {
    this->epochTime = QTime::currentTime();

    this->rawCapture = new RawEegCapture(CAPTURE_FILE_NAME);
    debug "Loaded raw capture file " << CAPTURE_FILE_NAME;

    this->rawCapture->selectColumn(CHANNEL);
    debug "Selected channel " << CHANNEL;

    this->markerFile = new EegMarkerFile(MARKER_FILE_NAME);
    debug "Loaded marker file " << MARKER_FILE_NAME;

    this->markers = markerFile->markers(MARKER_TYPE);
    this->markerCount = this->markers.size();
    debug "Extracted " << this->markerCount << " markers";
}

void FannTestRunner::initializeFann() {
    fann_set_activation_function_hidden(this->ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(this->ann, FANN_SIGMOID_SYMMETRIC);
}

unsigned FannTestRunner::encodeStimulusType(QString description) {
    for (int i = 0; i < STIMULUS_COUNT; i++) {
        if (description == stimulusTypes[i]) {
            return i;
        }
    }

    debug "Unknown stimulus " << description;
    return 0;
}

QString FannTestRunner::decodeStimulusType(fann_type *output) {
    int type;
    double max;

    for (int i = 0; i < STIMULUS_COUNT; i++) {
        double value = output[i];

        if (value > max || i == 0) {
            max = value;
            type = i;
        }
    }

    if (type != -1) {
        return stimulusTypes[type];
    }

    debug "Unknown stimulus code";
    return "";
}

int FannTestRunner::create() {
    QFile* file = new QFile(TRAIN_FILE_NAME);
    file->open(QFile::WriteOnly);
    QTextStream stream(file);

    unsigned num_input = TIME_FRAME / SAMPLING_INTERVAL;
    unsigned num_output = STIMULUS_COUNT;
    unsigned num_markers = this->markerCount;

    cout << "Writing to training data file: ";
    cout.flush();
    stream << num_markers << " " << num_input << " " << num_output << "\n";

    unsigned offsetInLines = OFFSET / SAMPLING_INTERVAL;
    ProgressIndicator indicator(num_markers);

    for (unsigned m = 0; m < num_markers; m++) {
        Marker marker = markers[m];
        this->rawCapture->gotoLine(marker.position + offsetInLines);

        unsigned index = 0;
        bool first = true;

        while (index++ < num_input) {
            if (first) {
                first = false;
            } else {
                stream << " ";
            }

            stream << this->rawCapture->read();
        }

        stream << "\n";

        unsigned stimulusType = encodeStimulusType(marker.description);
        for (unsigned i = 0; i < num_output; i++) {
            if (i > 0) {
                stream << " ";
            }

            stream << ((i == stimulusType) ? 1 : 0);
        }

        indicator.setValue(m + 1);
        stream << "\n";
    }

    stream << "\n";
    file->close();
    cout << "\n";

    return EXIT_SUCCESS;
}

int FannTestRunner::train() {
    const unsigned int num_input = TIME_FRAME / SAMPLING_INTERVAL;
    const unsigned int num_output = STIMULUS_COUNT;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;

    this->ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    this->initializeFann();
    fann_set_activation_function_hidden(this->ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(this->ann, FANN_SIGMOID_SYMMETRIC);
    fann_train_on_file(this->ann, TRAIN_FILE_NAME, max_epochs, epochs_between_reports, desired_error);
    fann_save(this->ann, NN_FILE_NAME);

    return EXIT_SUCCESS;
}

int FannTestRunner::run() {
    this->ann = fann_create_from_file(NN_FILE_NAME);
    this->initializeFann();

    unsigned num_input = TIME_FRAME / SAMPLING_INTERVAL;
    unsigned success = 0;
    unsigned total = 0;

    cout << "Now testing the NN: ";
    unsigned offsetInLines = OFFSET / SAMPLING_INTERVAL;
    ProgressIndicator indicator(this->markerCount);

    for (unsigned i = 0; i < this->markerCount; i++) {
        Marker marker = markers[i];
        this->rawCapture->gotoLine(marker.position + offsetInLines);

        fann_type* output;
        fann_type input[num_input];

        for (unsigned index = 0; index < num_input; index++) {
            input[index] = this->rawCapture->read();
        }

        output = fann_run(this->ann, input);
        total++;

        QString type = decodeStimulusType(output);
        //qDebug() << output[0] << " " << output[1] << " " << output[2];
        //qDebug() << "Determined type to be " << type << ", was " << marker.description;

        if (type == marker.description) {
            success++;
        }

        //sleep(1);
        indicator.setValue(i + 1);
    }

    cout << "\n" << total << " tested, " << success << " passed => " << (int)((success * 100) / total) << "%\n";

    fann_destroy(this->ann);
    qDebug() << "Destroyed NN";

    return EXIT_SUCCESS;
}
