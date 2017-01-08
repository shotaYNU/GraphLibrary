#ifndef progress_h
#define progress_h

#include <iostream>
#include <iomanip>

class Progress {
public:
    Progress() {
        max = 100;
        maxLength = 50;
    }
    ~Progress() {

    }

    void startProgress() {
        cout << "In progress..." << endl;
        flushProgress(0.0f);
    }
    void flushProgress(float progress) {
        cout << getProgressString(progress) << fixed << setprecision(1) << progress * max << "%\r" << flush;
    }
    void endProgress() {
        cout << endl << endl;
    }

private:
    float max;
    int maxLength;

    string getProgressString(float _progress) {
        string progressString = "[";
        int barLength = int(maxLength * _progress);

        for (int i = 0; i < barLength; ++i)
            progressString += "=";
        progressString += (barLength < maxLength) ? ">" : "";
        for (int i = 0; i < maxLength - barLength; ++i)
            progressString += " ";
        progressString += "] ";

        return progressString;
    }
};

#endif
