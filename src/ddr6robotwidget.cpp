#include "ddr6robotwidget.h"
#include <QDebug>

DDR6RobotWidget::DDR6RobotWidget(QWidget *parent)
        : RRGLWidget(parent) {
    loadRobotModelSTLFile();
    configureModelParams();
}

DDR6RobotWidget::~DDR6RobotWidget() {

}

void DDR6RobotWidget::loadRobotModelSTLFile() {
    //模型由7个小部件组成
    mRobotModel.link0 = new STLFileLoader(":/stl/base_link.STL", 1000);
    mRobotModel.link1 = new STLFileLoader(":/stl/link_1.STL", 1000);
    mRobotModel.link2 = new STLFileLoader(":/stl/link_2.STL", 1000);
    mRobotModel.link3 = new STLFileLoader(":/stl/link_3.STL", 1000);
    mRobotModel.link4 = new STLFileLoader(":/stl/link_4.STL", 1000);
    mRobotModel.link5 = new STLFileLoader(":/stl/link_5.STL", 1000);
    mRobotModel.link6 = new STLFileLoader(":/stl/link_6.STL", 1000);
    mDeskModel.link0 = new STLFileLoader(":/stl/desk.STL", 1);
}

void DDR6RobotWidget::configureModelParams() {
    //注意：经过旋转、平移后坐标系会改变
    mRobotConfig.d = {0, 127.00, -122.00, -101.00, -1.0, 0.00, 0.00};           //沿z轴平移
    mRobotConfig.JVars = {0, 0, 0, 0, 0, 0, 0};                                 //绕z轴旋转角度
    mRobotConfig.a = {0, 0, 0, 0, 0, 0, 0};                                     //沿x轴平移
    mRobotConfig.alpha = {0, 0, 180.00, 0, 0, 0, 0};                            //绕X轴旋转角度

    // 默认开启网格
    mGlobalConfig = {true, false, false, false, false, false, false, false, false};
//    mGlobalConfig = {true, true, true, true, true, true, true, true, true};
}

void DDR6RobotWidget::drawGL() {
    //方法：不断调整每个link的坐标系（glTranslatef、glRotatef），依次组合起所有link
    //TODO: 此处待优化，调整坐标系不通用，这里写死了
    glPushMatrix();

    if (mGlobalConfig.isDrawGrid) drawGrid();
    if (mGlobalConfig.isDrawWorldCoord) drawCoordinates();
    if (mGlobalConfig.isDrawDesk) drawGLForDesk();

    // 基座
    setupColor(160, 160, 160);
    mRobotModel.link0->draw();

    // 一关节
    if (mGlobalConfig.isDrawJoint1Coord) {
        drawSTLCoordinates(255, 0, 0);
    }
    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[1]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[1], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[1], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[1], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link1->draw();

    // 调整坐标系
    glRotatef(90, 1.0, 0.0, 0.0);
    // 二关节  修改2关节的Z轴 +90
    if (mGlobalConfig.isDrawJoint2Coord) {
        drawSTLCoordinates(0, 255, 0);
    }
    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[2]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[2] + 90, 0.0, 0.0, 1.0);       // 绕z轴旋转
    glTranslatef(mRobotConfig.a[2], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[2], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link2->draw();

    // 调整坐标系
    glTranslatef(300, 0.0, 0.0);

    // 三关节
    if (mGlobalConfig.isDrawJoint3Coord) {
        drawSTLCoordinates(0, 0, 255);
    }
    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[3]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[3], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[3], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[3], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link3->draw();

    // 调整坐标系
    glTranslatef(260, 0.0, 0.0);
    glRotatef(-90, 0.0, 0.0, 1.0);                              // 绕x轴旋转

    // 四关节
    if (mGlobalConfig.isDrawJoint4Coord) {
        drawSTLCoordinates(255, 255, 0);
    }
    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[4]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[4], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[4], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[4], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link4->draw();

    // 调整坐标系
    glTranslatef(0.0, 0.0, 110.0);
    glRotatef(-90, 1.0, 0.0, 0.0);                              // 绕x轴旋转

    // 五关节
    if (mGlobalConfig.isDrawJoint5Coord) {
        drawSTLCoordinates(0, 255, 255);
    }
    setupColor(50, 50, 50);
    glTranslatef(0.0, 0.0, mRobotConfig.d[5]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[5], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[5], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[5], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link5->draw();

    // 调整坐标系
    glTranslatef(0.0, 0.0, 110.0);
    glRotatef(90, 1.0, 0.0, 0.0);                               // 绕x轴逆时针旋转90°

    // 六关节
    if (mGlobalConfig.isDrawJoint6Coord) {
        drawSTLCoordinates(255, 0, 255);
    }

    setupColor(160, 160, 160);
    glTranslatef(0.0, 0.0, mRobotConfig.d[6]);                  // z轴方向平移
    glRotatef(mRobotConfig.JVars[6], 0.0, 0.0, 1.0);            // 绕z轴旋转
    glTranslatef(mRobotConfig.a[6], 0.0, 0.0);                  // x轴方向平移
    glRotatef(mRobotConfig.alpha[6], 1.0, 0.0, 0.0);            // 绕x轴旋转
    mRobotModel.link6->draw();

    glPopMatrix();
}

void DDR6RobotWidget::drawGLForDesk() {
    glPushMatrix();
    setupColor(169, 169, 169);
    glTranslatef(0.0, 0.0, 490); //桌子高度
    mDeskModel.link0->draw();
    glPopMatrix();
    glTranslatef(0.0, 0.0, 490); // glPushMatrix\glPopMatrix
}

//用来绘制OpenGL的窗口，只要有更新发生，这个函数就会被调用
void DDR6RobotWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         //清除屏幕和深度缓存
    glPushMatrix();
    glTranslated(0, 0, z_zoom);
    glTranslated(xTran, yTran, 0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0); //绕x轴旋转
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0); //绕y轴旋转
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0); //绕z轴旋转
    glRotated(+90.0, 1.0, 0.0, 0.0);
    drawGL();
    glPopMatrix();
}
