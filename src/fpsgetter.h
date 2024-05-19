//
// Created by JerryWang on 24-5-14.
//

#ifndef ROBOTSIMULATION_FPSGETTER_H
#define ROBOTSIMULATION_FPSGETTER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <cmath>

class FPSGetter : public QObject {
Q_OBJECT
public:
    FPSGetter();

private:
    int frameCount;
    double fps;
    QElapsedTimer elapsedTimer;

signals:
    void updateFps(double var);
};


#endif //ROBOTSIMULATION_FPSGETTER_H
