#ifndef DDR6ROBOTWIDGET_H
#define DDR6ROBOTWIDGET_H

#include "rrglwidget.h"

/**
 * @brief The DDR6RobotWidget class
 * 6轴机械臂3d显示
 */
class DDR6RobotWidget : public RRGLWidget {
public:
    DDR6RobotWidget(QWidget *parent = 0);

    ~DDR6RobotWidget();

    ///画机械臂STL模型
    void drawGL() override;

    ///画桌面
    void drawGLForDesk();

public:
    /// 7个小部件组成
    typedef struct DD6RobotModel {
        STLFileLoader *link0;
        STLFileLoader *link1;
        STLFileLoader *link2;
        STLFileLoader *link3;
        STLFileLoader *link4;
        STLFileLoader *link5;
        STLFileLoader *link6;
    } DDR6RobotSTLModel;
    /// 桌子
    typedef struct DeskModel {
        STLFileLoader *link0;
    } DeskModel;

    /// 机械臂模型
    DDR6RobotSTLModel mRobotModel;
    /// 桌子模型
    DeskModel mDeskModel;

protected:
    void paintGL() override;

private:
    /// 加载机械臂stl文件
    void loadRobotModelSTLFile();

    /// 配置模型相关参数
    void configureModelParams();
};

#endif // DDR6ROBOTWIDGET_H
