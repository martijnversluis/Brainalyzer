#include "fanntestrunner.h"

QString stimulusTypes[STIMULUS_COUNT] = {"S  2", "S  3", "S  4"};
//QString stimulusTypes[STIMULUS_COUNT] = {"S  2", "S  5", "S  6"};

FannTestRunner::FannTestRunner() {}

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
    int type = 0;
    double max = 0;

    for (int i = 0; i < STIMULUS_COUNT; i++) {
        double value = output[i];

        if (value == max) {
            return NULL;
        }

        if (value > max || i == 0) {
            max = value;
            type = i;
        }
    }

    if (type != -1) {
        return stimulusTypes[type];
    }

    return NULL;
}

int FannTestRunner::create(QString channel) {
    if (channel.length() == 0) {
        channel = CHANNEL;
    }

    unsigned num_input = TIME_FRAME / SAMPLING_INTERVAL;
    unsigned num_output = STIMULUS_COUNT;

    RawEegCapture* rawCapture = new RawEegCapture(CAPTURE_FILE_NAME);
    rawCapture->selectColumn(channel);
    debug "Loaded capture " << CAPTURE_FILE_NAME << " [CHANNEL: " << CHANNEL << "]";

    EegMarkerFile* markerFile = new EegMarkerFile(MARKER_FILE_NAME);
    QVector<Marker> markers = markerFile->markers(MARKER_TYPE);
    unsigned num_markers = markers.size();
    debug "Loaded " << num_markers << " \"" << MARKER_TYPE << "\" markers from " << MARKER_FILE_NAME;

    QFile* file = new QFile(TRAIN_FILE_NAME);
    file->open(QFile::WriteOnly);
    QTextStream stream(file);
    cout << "Writing to training data file " << TRAIN_FILE_NAME << ": ";

    stream << num_markers << " " << num_input << " " << num_output << "\n";

    unsigned offsetInLines = OFFSET / SAMPLING_INTERVAL;
    ProgressIndicator indicator(num_markers);

    for (unsigned m = 0; m < num_markers; m++) {
        Marker marker = markers[m];
        rawCapture->gotoLine(marker.position + offsetInLines);

        for (unsigned i = 0; i < num_input; i++) {
            if (i > 0) stream << " ";
            stream << rawCapture->read();
        }

        stream << "\n";

        unsigned stimulusType = encodeStimulusType(marker.description);

        for (unsigned i = 0; i < num_output; i++) {
            if (i > 0) stream << " ";
            stream << ((i == stimulusType) ? 1 : 0);
        }

        stream << "\n";
        indicator.setValue(m + 1);
    }

    stream << "\n";
    file->close();
    cout << "\n";

    return EXIT_SUCCESS;
}

int FannTestRunner::train(float desiredError) {
    if (desiredError == 0.0) {
        desiredError = 0.0001;
    }

    const unsigned int numInput = TIME_FRAME / SAMPLING_INTERVAL;
    const unsigned int numOutput = STIMULUS_COUNT;
    const unsigned int numLayers = 4;
    const unsigned int maxEpochs = 10000;
    const unsigned int epochsBetweenReports = 1000;

    this->ann = fann_create_standard(numLayers, numInput, 25, 10, numOutput);
    struct fann_train_data *data = fann_read_train_from_file(TRAIN_FILE_NAME);
    fann_set_input_scaling_params(this->ann, data, 0.0f, 1.0f);

    fann_set_learning_rate(this->ann, 0.5);
    fann_set_training_algorithm(this->ann, FANN_TRAIN_QUICKPROP);

    fann_set_activation_function_hidden(this->ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(this->ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_data(this->ann, data, maxEpochs, epochsBetweenReports, desiredError);
    fann_save(this->ann, NN_FILE_NAME);

    return EXIT_SUCCESS;
}

int FannTestRunner::mse() {
    this->ann = fann_create_from_file(NN_FILE_NAME);
    struct fann_train_data *data =
        fann_read_train_from_file(TRAIN_FILE_NAME);

    cout << "Current MSE: " << fann_test_data(this->ann, data) << endl;
    return EXIT_SUCCESS;
}

int FannTestRunner::run() {
    this->ann = fann_create_from_file(NN_FILE_NAME);
    struct fann_train_data *capture = fann_read_train_from_file(TRAIN_FILE_NAME);

    unsigned success = 0;
    unsigned total = 0;

    for (unsigned i = 0; i < capture->num_data; i++) {
        fann_type* output = fann_run(this->ann, capture->input[i]);
        QString type = decodeStimulusType(output);

        if (type != NULL) {
            total++;

            if (type == decodeStimulusType(capture->output[i])) {
                success++;
            }
        }
    }

    cout << "\n" << total << " tested, " << success << " passed => " << (int)((success * 100) / total) << "%\n";

    fann_destroy(this->ann);
    qDebug() << "Destroyed NN";

    return EXIT_SUCCESS;
}
