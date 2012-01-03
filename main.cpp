#include <QtCore/QCoreApplication>

#include "fanntestrunner.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString command(argv[1]);
    FannTestRunner testRunner;

    if (command == "create") {
        return testRunner.create();
    }

    if (command == "train") {
        return testRunner.train();
    }

    if (command == "run") {
        return testRunner.run();
    }

    cout << "usage: qt_fann create Create a new training file\n";
    cout << "               train  Train the NN on the generated training file\n";
    cout << "               run    Run the trained NN\n";

    return EXIT_SUCCESS;
}
