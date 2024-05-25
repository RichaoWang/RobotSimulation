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
            emit updateFps(std::round(fps * 100) / 100);
        }
    });
    timer->start(16);
    elapsedTimer.start();
}