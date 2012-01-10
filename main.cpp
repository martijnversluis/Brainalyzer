#include <QtCore/QCoreApplication>

#include "fanntestrunner.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString command(argv[1]);
    FannTestRunner testRunner;

    if (command == "create") {
        QString channel = (argc >= 3) ? QString(argv[2]) : NULL;
        return testRunner.create(channel);
    }

    if (command == "train") {
        double desiredError = (argc >= 3) ? QString(argv[2]).toFloat() : 0;
        return testRunner.train(desiredError);
    }

    if (command == "run") {
        return testRunner.run();
    }

    if (command == "mse") {
        return testRunner.mse();
    }

    cout << "usage: qt_fann create [<channel>]      Create a new training file\n";
    cout << "               train [<desired error>] Train the NN on the generated training file\n";
    cout << "               run                     Run the trained NN\n";

    return EXIT_SUCCESS;
}
