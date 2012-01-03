#include "progressindicator.h"

ProgressIndicator::ProgressIndicator(double max):
    currentValue(0), maxValue(max), progress(0), progressShown(false)
{
    this->showProgress();
}

void ProgressIndicator::setValue(double value) {
    int newProgress = (100 * value) / maxValue;

    if (this->progress != newProgress) {
        this->progress = newProgress;
        this->showProgress();
    }
}

void ProgressIndicator::showProgress() {
    if (!this->progressShown) {
        this->progressShown = true;
    } else {
        for (unsigned i = 0; i < (WIDTH + 1); i++) {
            std::cout << "\b";
        }
    }

    std::cout << std::setw(WIDTH) << std::setfill(' ') << this->progress << "%";
    std::cout.flush();
}
