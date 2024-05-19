#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//#include <cstdio>
//#include "FK.h"
//
//int main() {
//    FK_Type FK = {0};
////    Forward_Kinematics(0, 0, 0, 0, 0, 0, &FK);
//    Forward_Kinematics(0, -PI/2, 0, 0, 0, 0, &FK);
//    printf("x : %Lf\r\ny : %Lf\r\nz : %Lf\r\ngama : %Lf\r\nbeta : %Lf\r\nalpha : %Lf\r\n",FK.x,FK.y,FK.z,FK.gama,FK.beta,FK.alpha);
//
////    x : 0.000000
////    y : 0.000012
////    z : 351.000000
////    gama : 3.141593
////    beta : -0.000000
////    alpha : 0.000000
//}