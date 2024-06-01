#include <QApplication>
#include "./components/fluwidgets/FluWidget.h"
#include "widgets/robotsimulationwindow.h"
#include "./components/fluwidgets/utils/FluLogUtils.h"

void setFont(const QApplication &app) {
    QFont font;
    QStringList l;
    l << "微软雅黑";
    font.setFamilies(l);
    app.setFont(font);
}


int main(int argc, char **argv) {
    QApplication app(argc, argv);
#ifdef _WIN32
    setFont(app);
#endif
    FluLogUtils::__init();
    RobotSimulationWindow w;
    w.show();

    return app.exec();
}
