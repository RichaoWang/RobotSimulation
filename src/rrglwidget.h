#ifndef RRGLWIDGET_H
#define RRGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include "stlfileloader.h"

class RRGLWidget : public QGLWidget {
    //显示机械臂gl基类，提供了鼠标旋转、方法缩小、平移、网格、坐标系等基本功能
Q_OBJECT

public:
    RRGLWidget(QWidget *parent = 0);

    ~RRGLWidget();

    /// 画网格
    void drawGrid();

    /// 画坐标系
    void drawCoordinates();

    /// 画每个组件变换后的坐标系
    void drawSTLCoordinates(int r, int g, int b,std::string text);

    /// 画STL模型
    virtual void drawGL();

    /// 设置显示颜色
    void setupColor(int r, int g, int b);

    /// x轴旋转
    void setXRotation(int angle);

    /// y轴旋转
    void setYRotation(int angle);

    /// X方向Y方向平移
    void setXYTranslate(int dx, int dy);

    /// 规范化角度
    int normalizeAngle(int angle);

    /// 设置Z轴距离
    void setZoom(int zoom);

signals:

    /// 信号：x轴旋转角度变化
    void xRotationChanged(int angle);

    /// 信号：y轴旋转角度变化
    void yRotationChanged(int angle);

    /// 信号：z轴旋转角度变化
    void zRotationChanged(int angle);

protected:
    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

public:
    /// x旋转角度
    GLdouble xRot = -2584;
    /// y旋转角度
    GLdouble yRot = 1376;
    /// z旋转角度
    GLdouble zRot = 0.0;
    /// Z轴距离
    int z_zoom = -1600;
    /// 界面显示X位置
    int xTran = 0;
    /// 界面显示Y位置
    int yTran = -500;
    /// 最后一次变化的位置
    QPoint lastPos;

    ///机械臂配置信息结构体
    typedef struct RobotConfig {
        QVector<float> d;     // z轴方向平移
        QVector<float> a;     // x轴方向平移
        QVector<float> alpha; // 绕x轴旋转
        QVector<float> JVars; // 绕z轴旋转
    } RobotConfig;

    /// XY平台配置信息结构体
    typedef struct XYPlatformConfig {
        float x;
        float y;
    } XYPlatformConfig;

    typedef struct GlobalConfig {
        bool isDrawGrid;        //网格
        bool isDrawWorldCoord;  //世界坐标系
        bool isDrawJoint1Coord; //关节1坐标系
        bool isDrawJoint2Coord; //关节2坐标系
        bool isDrawJoint3Coord; //关节3坐标系
        bool isDrawJoint4Coord; //关节4坐标系
        bool isDrawJoint5Coord; //关节5坐标系
        bool isDrawJoint6Coord; //关节6坐标系
        bool isDrawDesk;        //桌子
        bool isDrawEnd;        //末端
    } GlobalConfig;

    /// 全局场景开关配置
    GlobalConfig mGlobalConfig;
    /// 机械臂旋转平移配置
    RobotConfig mRobotConfig;
};

#endif // RRGLWIDGET_H
