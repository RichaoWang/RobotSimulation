//#include "mainwindow.h"
//#include "components/FluFrameLessWidget.h"
//#include <QApplication>
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    FluFrameLessWidget w;
//    w.show();
//    return a.exec();
//}


#include <QApplication>
#include "./components/FluWidget.h"
#include "widgets/robotsimulationwindow.h"
#include "./components/utils/FluLogUtils.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    FluLogUtils::__init();
    RobotSimulationWindow w;
    w.show();

    return app.exec();
}
