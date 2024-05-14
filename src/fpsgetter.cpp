//
// Created by JerryWang on 24-5-14.
//

#include "fpsgetter.h"


FPSGetter::FPSGetter() {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=] {
        frameCount++;
        qint64 elapsed = elapsedTimer.elapsed();
        if (elapsed > 1000) {
            fps = frameCount * 1000.0 / elapsed;
            frameCount = 0;
            elapsedTimer.restart();
            emit updateFps(fps);
        }
    });
    timer->start(16);
    elapsedTimer.start();
}