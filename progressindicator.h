#ifndef PROGRESSINDICATOR_H
#define PROGRESSINDICATOR_H

#define WIDTH 3

#include <iostream>
#include <iomanip>

class ProgressIndicator {

public:
    ProgressIndicator(double max);
    void setValue(double value);
    void showProgress();

private:
    double currentValue;
    double maxValue;
    int progress;
    bool progressShown;

};

#endif
