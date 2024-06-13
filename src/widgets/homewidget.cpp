
#include "homewidget.h"
#include "components/fluwidgets/FluComboBox.h"
#include "components/fluwidgets/FluSlider.h"
#include "components/fluwidgets/FluCheckBox.h"
#include "components/fluwidgets/FluExpander.h"
#include "components/fluwidgets/FluLabel.h"
#include "components/fluwidgets/FluDoubleSpinBox.h"
#include "components/fluwidgets/FluConfirmFlyout.h"
#include "components/fluwidgets/FluLineEditFlyout.h"
#include "tools/kinematics.h"
#include "tools/infobar.h"
#include <QtConcurrent/QtConcurrent>

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QHBoxLayout;
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(10, 10, 10, 15);

    m_pointSetting = new QSettings("points.ini", QSettings::IniFormat, this);
    auto kv = m_pointSetting->allKeys();
    if (kv.size() == 0) {
        qDebug() << "new points.ini";
        m_pointSetting->setValue("init",
                                 QVariantList{1.4835298641951802, -0.22689280275926285, -1.9547687622336491,
                                              -1.9373154697137058,
                                              -1.3788101090755203, 2.181661564992912});
        m_pointSetting->setValue("zero", QVariantList{0, 0, 0, 0, 0, 0});
    }
    kv = m_pointSetting->allKeys();
    for (auto k: kv) {
        std::vector<double> vector;
        for (QVariant value: m_pointSetting->value(k).toList()) {
            vector.push_back(value.toDouble());
        }
        mPointMap[k] = vector;
    }


    /// robot display widget

    osgWidget = new OsgWidget(this);
    osgWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    m_mainLayout->addWidget(osgWidget);
    m_mainLayout->addWidget(osgWidget);

    /// control widget
    auto controlWidget = new QWidget(this);
    controlWidget->setObjectName("controlWidget");
    controlWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto vCtlLayout = new QVBoxLayout;
    auto controlScrollView = new FluVScrollView;
    controlScrollView->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    vCtlLayout->addWidget(controlScrollView, 1);
    controlScrollView->getMainLayout()->setAlignment(Qt::AlignTop);
    controlWidget->setLayout(vCtlLayout);
    controlScrollView->getMainLayout()->setSpacing(2);

    /// -------------------------- 视角 --------------------------
    auto obsExpander = new FluExpander(this);
    auto obsExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    obsExpLabel->setText("视角");
    obsExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    obsExpander->getWrap1Layout()->addWidget(obsExpLabel);
    auto osbWidget = makeObservationWidget();
    obsExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    obsExpander->getWrap2Layout()->addWidget(osbWidget);
    controlScrollView->getMainLayout()->addWidget(obsExpander);
    controlScrollView->getMainLayout()->addSpacing(10);


    /// -------------------------- opt part --------------------------
    auto optExpander = new FluExpander(this);
    auto optExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    optExpLabel->setText("快捷显示");
    optExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    optExpander->getWrap1Layout()->addWidget(optExpLabel);
    checkboxesWidget = makeCheckOptionWidget();
    optExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    optExpander->getWrap2Layout()->addWidget(checkboxesWidget);
    controlScrollView->getMainLayout()->addWidget(optExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------

    /// -------------------------- points part --------------------------
    auto pointsExpander = new FluExpander(this);
    auto pointsExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    pointsExpLabel->setText("坐标点及示教");
    pointsExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    pointsExpander->getWrap1Layout()->addWidget(pointsExpLabel);
    QWidget *pointsWidget = makePointsWidget();
    pointsExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    pointsExpander->getWrap2Layout()->addWidget(pointsWidget);
    controlScrollView->getMainLayout()->addWidget(pointsExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------


    /// -------------------------- control part --------------------------
    auto ctrlExpander = new FluExpander(this);
    auto ctrlExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    ctrlExpLabel->setText("控制器");
    ctrlExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    ctrlExpander->getWrap1Layout()->addWidget(ctrlExpLabel);
    robotControlWidget = makeRobotControlWidget();
    ctrlExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    ctrlExpander->getWrap2Layout()->addWidget(robotControlWidget);
    controlScrollView->getMainLayout()->addWidget(ctrlExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------

    m_mainLayout->addWidget(controlWidget);


    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/HomeWidget.qss", this);

    /// robot kinematics
    robotKinematics = std::make_unique<RobotKinematics>();

    connectSignsSlots();


    /// default
    checkboxesWidget->findChildren<QCheckBox *>("gridCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox *>("wrist3CoordCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox *>("worldCoordCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox *>("deskCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox *>("posTextCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox *>("ProceduralAnimationBox")[0]->setChecked(true);

//    posDisplayExpander->expand();
    optExpander->expand();
    ctrlExpander->expand();
    pointsExpander->expand();
    obsExpander->expand();

    controlWidget->findChildren<QSlider *>("j1Slider")[0]->setValue(85 * 100);
    controlWidget->findChildren<QSlider *>("j2Slider")[0]->setValue(-13 * 100);
    controlWidget->findChildren<QSlider *>("j3Slider")[0]->setValue(-112 * 100);
    controlWidget->findChildren<QSlider *>("j4Slider")[0]->setValue(-111 * 100);
    controlWidget->findChildren<QSlider *>("j5Slider")[0]->setValue(-79 * 100);
    controlWidget->findChildren<QSlider *>("j6Slider")[0]->setValue(125 * 100);

}


// 创建旋转矩阵的函数
Eigen::Matrix3d createRotationMatrix(double roll, double pitch, double yaw) {
    Eigen::Matrix3d rotationMatrix;

    // 绕x轴的旋转矩阵（Roll）
    Eigen::Matrix3d Rx;
    Rx << 1, 0, 0,
            0, cos(roll), -sin(roll),
            0, sin(roll), cos(roll);

    // 绕y轴的旋转矩阵（Pitch）
    Eigen::Matrix3d Ry;
    Ry << cos(pitch), 0, sin(pitch),
            0, 1, 0,
            -sin(pitch), 0, cos(pitch);

    // 绕z轴的旋转矩阵（Yaw）
    Eigen::Matrix3d Rz;
    Rz << cos(yaw), -sin(yaw), 0,
            sin(yaw), cos(yaw), 0,
            0, 0, 1;

    // 按顺序组合旋转矩阵（Roll -> Pitch -> Yaw）
    rotationMatrix = Rz * Ry * Rx;

    return rotationMatrix;
}

// 创建变换矩阵的函数
Eigen::MatrixXd createTransformationMatrix(const double xyzrpy[6]) {
    Eigen::MatrixXd T(4, 4);

    // 提取平移和旋转信息
    double x = xyzrpy[0];
    double y = xyzrpy[1];
    double z = xyzrpy[2];
    double roll = xyzrpy[3];
    double pitch = xyzrpy[4];
    double yaw = xyzrpy[5];

    // 创建旋转矩阵
    Eigen::Matrix3d rotationMatrix = createRotationMatrix(roll, pitch, yaw);

    // 组合变换矩阵
    T.setIdentity();
    T.block<3, 3>(0, 0) = rotationMatrix;
    T(0, 3) = x;
    T(1, 3) = y;
    T(2, 3) = z;

    return T;
}

void HomeWidget::connectSignsSlots() {
    /// connect

    connect(this, &HomeWidget::sigOsgDrawPosText, osgWidget, &OsgWidget::setRendText, Qt::QueuedConnection);


    connect(this, &HomeWidget::sigJointMoveAnimations, this, [=](std::vector<std::vector<double>> ress) {
        /// todo 备份原来的点 每次循环都会回到初始点 除了最后一个
        QtConcurrent::run([=]() {
                              auto bkslds = robotControlWidget->findChildren<QSlider *>();
                              std::vector<double> bkjoints;
                              for (int i = 1; i < 7; ++i) {
                                  auto v = robotControlWidget->findChildren<QSlider *>("j" + QString::number(i) + "Slider")[0]->value() /
                                           100;
                                  bkjoints.push_back(v);
                              }

                              for (int i = 0; i < ress.size(); ++i) {
                                  for (int i = 0; i < bkjoints.size(); ++i) {
                                      robotControlWidget->findChildren<QSlider *>("j" + QString::number(i + 1) + "Slider")[0]->setValue(
                                              bkjoints[i] * 100);
                                  }
                                  qDebug() << "归位完成";
                                  std::this_thread::sleep_for(std::chrono::seconds(1));
                                  auto res = ress[i];
                                  for (int i = 0; i < res.size(); ++i) {
                                      robotControlWidget->findChildren<QSlider *>("j" + QString::number(i + 1) + "Slider")[0]->setValue(
                                              res[i] * (180.0 / M_PI) * 100);
                                  }
                                  qDebug() << "到达完成";
                                  std::this_thread::sleep_for(std::chrono::seconds(1));

                              }
//                              emit sigMsg(FluShortInfoBarType::Suc, "执行所有解完成");

                          }
        );

    }, Qt::QueuedConnection);

    connect(this, &HomeWidget::sigJointMoveAnimation, this, [=](std::vector<double> res) {
        auto pab = checkboxesWidget->findChildren<QCheckBox *>("ProceduralAnimationBox")[0];
        if (!pab->isChecked()) {
//            auto slds = robotControlWidget->findChildren<QSlider *>();
            for (int i = 0; i < res.size(); ++i) {
                robotControlWidget->findChildren<QSlider *>("j" + QString::number(i + 1) + "Slider")[0]->setValue(
                        res[i] * (180.0 / M_PI) * 100);
            }

            return;
        }
        mCount = 0;
        auto slds = robotControlWidget->findChildren<QSlider *>();
        for (int i = 0; i < slds.size(); ++i) {
            slds[i]->setEnabled(false);
        }

        for (int i = 0; i < res.size(); ++i) {
            res[i] = res[i] * (180.0 / M_PI);
        }

        /// 动画更新机器人
        /// 现在的关节角
        std::vector<double> ori_joints;
        for (int i = 1; i < 7; ++i) {
            auto v = robotControlWidget->findChildren<QSlider *>("j" + QString::number(i) + "Slider")[0]->value() / 100;
            ori_joints.push_back(v);
        }

//        std::vector<double> dif_joints(res.size());
//        qDebug() << "============================================";
//        qDebug() << "原关节角:" << ori_joints;
//        qDebug() << "反解关节角:" << res;
//        std::transform(res.begin(), res.end(), ori_joints.begin(), dif_joints.begin(), std::minus<double>());
//        qDebug() << "差值关节角:" << dif_joints;
//
//        return;

        for (int i = 0; i < res.size(); ++i) {
            auto cur_j = ori_joints[i];
            auto calc_j = res[i];
//            if (0 <= std::abs(calc_j - cur_j) < 1) {
//                continue;
//            }
            auto dif_j = calc_j - cur_j;
//            qDebug() << "关节" << i + 1 << "差值 " << dif_j;
            auto t = new QTimer(this);
            connect(t, &QTimer::timeout, [=]() {
                auto v = robotControlWidget->findChildren<QSlider *>(
                        "j" + QString::number(i + 1) + "Slider")[0]->value() / 100;
                if (dif_j > 0) {
                    v += 1;
                    if (v >= calc_j) {
                        v = calc_j;
                        t->stop();
                        t->destroyed();
                    }
                } else {
                    v -= 1;
                    if (v <= calc_j) {
                        v = calc_j;
                        t->stop();
                        t->destroyed();
                    }

                }
                robotControlWidget->findChildren<QSlider *>("j" + QString::number(i + 1) + "Slider")[0]->setValue(
                        v * 100);

            });


            connect(t, &QTimer::destroyed, [=]() {
                qDebug() << i + 1 << "关闭";
                mCount += 1;
                if (mCount == 6) {
                    qDebug() << "所有定时器关闭...";
                    auto slds = robotControlWidget->findChildren<QSlider *>();
                    for (int k = 0; k < slds.size(); ++k) {
                        slds[k]->setEnabled(true);
                    }
                    InfoBar::showInfoBar(FluShortInfoBarType::Suc, "机器人运动完成", this);
                }
            });

            t->start(1);
        }
    }, Qt::QueuedConnection);


    connect(allIKSolBtn, &QPushButton::clicked, [=]() {
        qDebug() << "所有解按钮被点击";
        auto spinBoxes = robotControlWidget->findChildren<QWidget *>(
                "eleWidget")[0]->findChildren<FluDoubleSpinBox *>();

        double xyzrpy[6];
        for (int i = 0; i < spinBoxes.size(); ++i) {
            auto var = spinBoxes[i]->text().toDouble();
            if (i <= 2) {
                var = var / 1000;
            } else {
                var = var * M_PI / 180;
            }
            xyzrpy[i] = var;
        }


        Eigen::MatrixXd T = createTransformationMatrix(xyzrpy);
        MatrixXd q_sols(6, 8);

        auto solnum = aubo_inverse(q_sols, T);
        if (solnum == 0) {
            InfoBar::showInfoBar(FluShortInfoBarType::Error, "机器人反解失败（0解）", this);
            return;
        }
//        auto res_q = q_sols.col(0);
//        std::vector<double> res_v;
//        for (int i = 0; i < res_q.size(); ++i) {
//            res_v.push_back(res_q(i));
//        }

        std::vector<std::vector<double>> res_v;
        for (int i = 0; i < solnum; ++i) {
            auto cs = q_sols.col(i);
            std::vector<double> cv;
            for (int j = 0; j < cs.size(); ++j) {
                cv.push_back(cs(j));
            }
            res_v.push_back(cv);
        }
//        qDebug() << res_v;  //std::vector(-2.8973, -2.91319, 2.30265, 0.503459, 1.5708, 1.3265)
        emit sigJointMoveAnimations(res_v);
    });

    connect(pubBtn, &QPushButton::clicked, [=]() {

        auto spinBoxes = robotControlWidget->findChildren<QWidget *>(
                "eleWidget")[0]->findChildren<FluDoubleSpinBox *>();

        double xyzrpy[6];
        for (int i = 0; i < spinBoxes.size(); ++i) {
            auto var = spinBoxes[i]->text().toDouble();
            if (i <= 2) {
                var = var / 1000;
            } else {
                var = var * M_PI / 180;
            }
            xyzrpy[i] = var;
        }


        Eigen::MatrixXd T = createTransformationMatrix(xyzrpy);
        MatrixXd q_sols(6, 8);
//        MatrixXd T(4, 4);

        auto solnum = aubo_inverse(q_sols, T);
        if (solnum == 0) {
            InfoBar::showInfoBar(FluShortInfoBarType::Error, "机器人反解失败（0解）", this);
            return;
        }
//        /// 所有解
//        auto q_sols = processIK(T);
        auto res_q = q_sols.col(0);
        std::vector<double> res_v;
        for (int i = 0; i < res_q.size(); ++i) {
            res_v.push_back(res_q(i));
        }
//        std::vector<std::vector<double>> res_v;
//        for (int i = 0; i < solnum; ++i) {
//            auto cs = q_sols.col(i);
//            std::vector<double> cv;
//            for (int j = 0; j < cs.size(); ++j) {
//                cv.push_back(cs(j));
//            }
//            res_v.push_back(cv);
//        }


//        /// todo 发送结果出去
        emit sigJointMoveAnimation(res_v);
//        emit sigJointMoveAnimations(res_v);

    });


    connect(this, &HomeWidget::sigKinCalc, this, [=]() {
        VectorXd q(6);
        auto slds = robotControlWidget->findChildren<QSlider *>();
        for (int j = 0; j < slds.size(); ++j) {
            q[j] = slds[j]->value() / 100.0 * (M_PI / 180.0);
        }

        MatrixXd T(4, 4);
        aubo_forward(T, q);
//        std::cout << T << std::endl;

        auto x = T(0, 3);
        auto y = T(1, 3);
        auto z = T(2, 3);

        Eigen::Matrix3d R = T.block<3, 3>(0, 0);
        auto rx = std::atan2(R(2, 1), R(2, 2)) * 180.0 / M_PI;
        auto ry = std::atan2(-R(2, 0), std::sqrt(R(2, 1) * R(2, 1) + R(2, 2) * R(2, 2))) * 180.0 / M_PI;
        auto rz = std::atan2(R(1, 0), R(0, 0)) * 180.0 / M_PI;


        emit sigOsgDrawPosText(QString::number(x * 1000, 'f', 2), QString::number(y * 1000, 'f', 2),
                               QString::number(z * 1000, 'f', 2), QString::number(rx, 'f', 2),
                               QString::number(ry, 'f', 2), QString::number(rz, 'f', 2));

    }, Qt::QueuedConnection);


    connect(this, &HomeWidget::sigJoinVarChanged, this, [=](int idx, float var) {
        osgWidget->updateJoint(mJointNameVec[idx - 1], var);

        robotControlWidget->findChildren<QLabel *>("j" + QString::number(idx) + "ValueLabel")[0]->setText(
                QString::number(var, 'f', 2));
        emit sigKinCalc();
    }, Qt::QueuedConnection);

    {
        auto jointSliders = robotControlWidget->findChildren<QSlider *>();
        for (int i = 0; i < jointSliders.size(); ++i) {
            QSlider *slider = jointSliders.at(i);
            slider->setMinimum(-18000);
            slider->setMaximum(18000);
            slider->setTickInterval(1);
            connect(slider, &QSlider::valueChanged, this, &HomeWidget::slotUpdateJVarsValue,
                    Qt::ConnectionType::QueuedConnection);
        }
    }


    {
        auto checkBoxes = checkboxesWidget->findChildren<QCheckBox *>();
        for (int i = 0; i < checkBoxes.size(); ++i) {
            QCheckBox *checkBox = checkBoxes.at(i);
            connect(checkBox, &QCheckBox::toggled, this, &HomeWidget::slotUpdateCheckOpt,
                    Qt::ConnectionType::QueuedConnection);
        }
    }

    /// ???
    {
        auto stepBtns = robotControlWidget->findChildren<QWidget *>("endEleWidget")[0]->findChildren<QPushButton *>();
        for (int i = 0; i < stepBtns.size(); ++i) {
            QPushButton *btn = stepBtns.at(i);
            connect(btn, &QCheckBox::clicked, this, &HomeWidget::slotUpdateStepMove,
                    Qt::ConnectionType::QueuedConnection);
        }
    }


}

void HomeWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void HomeWidget::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/HomeWidget.qss", this);
    } else {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/HomeWidget.qss", this);
    }
}

QWidget *HomeWidget::makeCheckOptionWidget() {
    auto checkboxesWidget = new QWidget(this);
    checkboxesWidget->setObjectName("checkboxesWidget");
    checkboxesWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

    auto checkBoxesLayout = new QGridLayout(this);
    checkBoxesLayout->setContentsMargins(10, 5, 10, 5);
    auto gridCb = new FluCheckBox("网格", this);
    gridCb->setObjectName("gridCheckBox");
    gridCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);


    auto worldCoordCb = new FluCheckBox("世界坐标系", this);
    worldCoordCb->setObjectName("worldCoordCheckBox");
    worldCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);


    auto EndCoordCb = new FluCheckBox("末端坐标系", this);
    EndCoordCb->setObjectName("wrist3CoordCheckBox");
//    EndCoordCb->setObjectName("endCoordCheckBox");
    EndCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto DeskCb = new FluCheckBox("实验台", this);
    DeskCb->setObjectName("deskCheckBox");
    DeskCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto PosTextCb = new FluCheckBox("位姿文本显示", this);
    PosTextCb->setObjectName("posTextCheckBox");
    PosTextCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto ProceduralAnimationCb = new FluCheckBox("过程动画", this);
    ProceduralAnimationCb->setObjectName("ProceduralAnimationBox");
    ProceduralAnimationCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    checkBoxesLayout->addWidget(gridCb, 0, 0);
    checkBoxesLayout->addWidget(worldCoordCb, 0, 1);
    checkBoxesLayout->addWidget(EndCoordCb, 1, 0);
    checkBoxesLayout->addWidget(DeskCb, 1, 1);
    checkBoxesLayout->addWidget(PosTextCb, 2, 0);
    checkBoxesLayout->addWidget(ProceduralAnimationCb, 2, 1);
    checkboxesWidget->setLayout(checkBoxesLayout);
    return checkboxesWidget;
}

QWidget *HomeWidget::makeRobotControlWidget() {
    auto ctrlWidget = new QWidget(this);
    ctrlWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

    auto ctrlLayout = new QVBoxLayout(this);
    ctrlLayout->setSpacing(5);

    auto sliderLabel = new FluLabel(this);
    sliderLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    sliderLabel->setText("关节控制");
    ctrlLayout->addWidget(sliderLabel);

    /// slider group
    auto sliderWidget = new QWidget(this);
    sliderWidget->setObjectName("sliderWidget");
    sliderWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    auto sldLayout = new QGridLayout(this);
    sldLayout->setContentsMargins(10, 5, 10, 5);

    auto j1Label = new FluLabel(this);
    j1Label->setText("关节1");
    j1Label->setAlignment(Qt::AlignCenter);

    auto j2Label = new FluLabel(this);
    j2Label->setText("关节2");
    j2Label->setAlignment(Qt::AlignCenter);

    auto j3Label = new FluLabel(this);
    j3Label->setText("关节3");
    j3Label->setAlignment(Qt::AlignCenter);

    auto j4Label = new FluLabel(this);
    j4Label->setText("关节4");
    j4Label->setAlignment(Qt::AlignCenter);

    auto j5Label = new FluLabel(this);
    j5Label->setText("关节5");
    j5Label->setAlignment(Qt::AlignCenter);

    auto j6Label = new FluLabel(this);
    j6Label->setText("关节6");
    j6Label->setAlignment(Qt::AlignCenter);

    auto j1Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j1Slider->setObjectName("j1Slider");
    j1Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto j2Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j2Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    j2Slider->setObjectName("j2Slider");

    auto j3Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j3Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    j3Slider->setObjectName("j3Slider");

    auto j4Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j4Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    j4Slider->setObjectName("j4Slider");

    auto j5Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j5Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    j5Slider->setObjectName("j5Slider");

    auto j6Slider = new FluSlider(Qt::Orientation::Horizontal, this);
    j6Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    j6Slider->setObjectName("j6Slider");

    auto j1ValueLabel = new FluLabel(this);
    j1ValueLabel->setAlignment(Qt::AlignCenter);
    j1ValueLabel->setObjectName("j1ValueLabel");
    j1ValueLabel->setText("0.0");
    j1ValueLabel->setFixedWidth(50);

    auto j2ValueLabel = new FluLabel(this);
    j2ValueLabel->setAlignment(Qt::AlignCenter);
    j2ValueLabel->setObjectName("j2ValueLabel");
    j2ValueLabel->setText("0.0");
    j2ValueLabel->setFixedWidth(50);

    auto j3ValueLabel = new FluLabel(this);
    j3ValueLabel->setAlignment(Qt::AlignCenter);
    j3ValueLabel->setObjectName("j3ValueLabel");
    j3ValueLabel->setText("0.0");
    j3ValueLabel->setFixedWidth(50);

    auto j4ValueLabel = new FluLabel(this);
    j4ValueLabel->setAlignment(Qt::AlignCenter);
    j4ValueLabel->setObjectName("j4ValueLabel");
    j4ValueLabel->setText("0.0");
    j4ValueLabel->setFixedWidth(50);

    auto j5ValueLabel = new FluLabel(this);
    j5ValueLabel->setAlignment(Qt::AlignCenter);
    j5ValueLabel->setObjectName("j5ValueLabel");
    j5ValueLabel->setText("0.0");
    j5ValueLabel->setFixedWidth(50);


    auto j6ValueLabel = new FluLabel(this);
    j6ValueLabel->setAlignment(Qt::AlignCenter);
    j6ValueLabel->setObjectName("j6ValueLabel");
    j6ValueLabel->setText("0.0");
    j6ValueLabel->setFixedWidth(50);

    sldLayout->addWidget(j1Label, 0, 0);
    sldLayout->addWidget(j1Slider, 0, 1);
    sldLayout->addWidget(j1ValueLabel, 0, 2);

    sldLayout->addWidget(j2Label, 1, 0);
    sldLayout->addWidget(j2Slider, 1, 1);
    sldLayout->addWidget(j2ValueLabel, 1, 2);

    sldLayout->addWidget(j3Label, 2, 0);
    sldLayout->addWidget(j3Slider, 2, 1);
    sldLayout->addWidget(j3ValueLabel, 2, 2);

    sldLayout->addWidget(j4Label, 3, 0);
    sldLayout->addWidget(j4Slider, 3, 1);
    sldLayout->addWidget(j4ValueLabel, 3, 2);

    sldLayout->addWidget(j5Label, 4, 0);
    sldLayout->addWidget(j5Slider, 4, 1);
    sldLayout->addWidget(j5ValueLabel, 4, 2);

    sldLayout->addWidget(j6Label, 5, 0);
    sldLayout->addWidget(j6Slider, 5, 1);
    sldLayout->addWidget(j6ValueLabel, 5, 2);

    sliderWidget->setLayout(sldLayout);

    ctrlLayout->addWidget(sliderWidget);
    ctrlLayout->addSpacing(10);

    auto endCroodLabel = new FluLabel(this);
    endCroodLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    endCroodLabel->setText("末端控制");
    ctrlLayout->addWidget(endCroodLabel);
    auto endEleWidget = new QWidget(this);
    endEleWidget->setObjectName("endEleWidget");
    endEleWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    auto endEleLayout = new QGridLayout(this);
    endEleLayout->setContentsMargins(10, 5, 10, 5);

    auto stepWidget = new QWidget(this);
//    stepWidget->setObjectName("stepWidget");
    auto stepLayout = new QHBoxLayout(this);
    auto stepBox = new FluDoubleSpinBox(this);
    stepBox->setValue(10.0);
    stepBox->setAlignment(Qt::AlignCenter);
    auto stepText = new FluLabel(this);
    stepText->setText("步进单位");
    stepText->setAlignment(Qt::AlignCenter);
    stepLayout->addWidget(stepText);
    stepLayout->addWidget(stepBox);
    stepWidget->setLayout(stepLayout);

    auto xupbtn = new FluPushButton(this);
    xupbtn->setObjectName("x_up");
    xupbtn->setText("+X");
    auto xdownbtn = new FluPushButton(this);
    xdownbtn->setText("-X");
    xdownbtn->setObjectName("x_down");

    auto rxupbtn = new FluPushButton(this);
    rxupbtn->setText("+Rx");
    rxupbtn->setObjectName("rx_up");
    auto rxdownbtn = new FluPushButton(this);
    rxdownbtn->setText("-Rx");
    rxdownbtn->setObjectName("rx_down");


    auto yupbtn = new FluPushButton(this);
    yupbtn->setText("+Y");
    yupbtn->setObjectName("y_up");
    auto ydownbtn = new FluPushButton(this);
    ydownbtn->setText("-Y");
    ydownbtn->setObjectName("y_down");

    auto ryupbtn = new FluPushButton(this);
    ryupbtn->setText("+Ry");
    ryupbtn->setObjectName("ry_up");

    auto rydownbtn = new FluPushButton(this);
    rydownbtn->setText("-Ry");
    rydownbtn->setObjectName("ry_down");

    auto zupbtn = new FluPushButton(this);
    zupbtn->setText("+Z");
    zupbtn->setObjectName("z_up");

    auto zdownbtn = new FluPushButton(this);
    zdownbtn->setText("-Z");
    zdownbtn->setObjectName("z_down");

    auto rzupbtn = new FluPushButton(this);
    rzupbtn->setText("+Rz");
    rzupbtn->setObjectName("rz_up");

    auto rzdownbtn = new FluPushButton(this);
    rzdownbtn->setText("-Rz");
    rzdownbtn->setObjectName("rz_down");

    endEleLayout->addWidget(stepWidget, 0, 0, 1, 3);

    endEleLayout->addWidget(xdownbtn, 2, 0);
    endEleLayout->addWidget(xupbtn, 2, 2);
    endEleLayout->addWidget(rxupbtn, 1, 1);
    endEleLayout->addWidget(rxdownbtn, 3, 1);


    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine); // 设置为水平线
    // 设置分割线颜色
    QPalette palette = line->palette();
    palette.setColor(QPalette::WindowText, QColor(230, 230, 230)); // 设置颜色为灰色
    line->setPalette(palette);

    endEleLayout->addWidget(line, 4, 0, 1, 3);

    endEleLayout->addWidget(ydownbtn, 6, 0);
    endEleLayout->addWidget(yupbtn, 6, 2);
    endEleLayout->addWidget(ryupbtn, 5, 1);
    endEleLayout->addWidget(rydownbtn, 7, 1);

    QFrame *line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine); // 设置为水平线
    // 设置分割线颜色
    QPalette palette2 = line2->palette();
    palette2.setColor(QPalette::WindowText, QColor(230, 230, 230)); // 设置颜色为灰色
    line2->setPalette(palette2);
    endEleLayout->addWidget(line2, 8, 0, 1, 3);

    endEleLayout->addWidget(zdownbtn, 10, 0);
    endEleLayout->addWidget(zupbtn, 10, 2);
    endEleLayout->addWidget(rzupbtn, 9, 1);
    endEleLayout->addWidget(rzdownbtn, 11, 1);

    endEleWidget->setLayout(endEleLayout);
    ctrlLayout->addWidget(endEleWidget);

    auto posLabel = new FluLabel(this);
    posLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    posLabel->setText("欧拉角控制");
    ctrlLayout->addWidget(posLabel);

    /// line edit group
    auto eleWidget = new QWidget(this);
    eleWidget->setObjectName("eleWidget");
    eleWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    auto eleLayout = new QGridLayout(this);
    eleLayout->setContentsMargins(10, 5, 10, 5);

    auto xLabel = new FluLabel(this);
    xLabel->setAlignment(Qt::AlignCenter);
    xLabel->setText("X");

    auto yLabel = new FluLabel(this);
    yLabel->setAlignment(Qt::AlignCenter);
    yLabel->setText("Y");

    auto zLabel = new FluLabel(this);
    zLabel->setAlignment(Qt::AlignCenter);
    zLabel->setText("Z");

    auto rxLabel = new FluLabel(this);
    rxLabel->setAlignment(Qt::AlignCenter);
    rxLabel->setText("Rx");

    auto ryLabel = new FluLabel(this);
    ryLabel->setAlignment(Qt::AlignCenter);
    ryLabel->setText("Ry");

    auto rzLabel = new FluLabel(this);
    rzLabel->setAlignment(Qt::AlignCenter);
    rzLabel->setText("Rz");

    auto xLineEdit = new FluDoubleSpinBox(this);
    xLineEdit->setAlignment(Qt::AlignCenter);
    xLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
//    xLineEdit->setMinimumWidth(150);
//    xLineEdit->setMinimumHeight(30);

    auto yLineEdit = new FluDoubleSpinBox(this);
    yLineEdit->setAlignment(Qt::AlignCenter);
    yLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
//    yLineEdit->setMinimumHeight(30);

    auto zLineEdit = new FluDoubleSpinBox(this);
    zLineEdit->setAlignment(Qt::AlignCenter);
    zLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
//    zLineEdit->setMinimumHeight(30);

    auto rxLineEdit = new FluDoubleSpinBox(this);
    rxLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    rxLineEdit->setAlignment(Qt::AlignCenter);
//    rxLineEdit->setMinimumHeight(30);

    auto ryLineEdit = new FluDoubleSpinBox(this);
    ryLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    ryLineEdit->setAlignment(Qt::AlignCenter);
//    ryLineEdit->setMinimumHeight(30);

    auto rzLineEdit = new FluDoubleSpinBox(this);
    rzLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    rzLineEdit->setAlignment(Qt::AlignCenter);
//    rzLineEdit->setMinimumHeight(30);


    auto _xLabel = new FluLabel(this);
    _xLabel->setAlignment(Qt::AlignCenter);
    _xLabel->setText("mm");

    auto _yLabel = new FluLabel(this);
    _yLabel->setAlignment(Qt::AlignCenter);
    _yLabel->setText("mm");

    auto _zLabel = new FluLabel(this);
    _zLabel->setAlignment(Qt::AlignCenter);
    _zLabel->setText("mm");

    auto _rxLabel = new FluLabel(this);
    _rxLabel->setAlignment(Qt::AlignCenter);
    _rxLabel->setText("°");

    auto _ryLabel = new FluLabel(this);
    _ryLabel->setAlignment(Qt::AlignCenter);
    _ryLabel->setText("°");

    auto _rzLabel = new FluLabel(this);
    _rzLabel->setAlignment(Qt::AlignCenter);
    _rzLabel->setText("°");


    eleLayout->addWidget(xLabel, 0, 0);
    eleLayout->addWidget(xLineEdit, 0, 1);
    eleLayout->addWidget(_xLabel, 0, 2);

    eleLayout->addWidget(yLabel, 1, 0);
    eleLayout->addWidget(yLineEdit, 1, 1);
    eleLayout->addWidget(_yLabel, 1, 2);


    eleLayout->addWidget(zLabel, 2, 0);
    eleLayout->addWidget(zLineEdit, 2, 1);
    eleLayout->addWidget(_zLabel, 2, 2);

    eleLayout->addWidget(rxLabel, 3, 0);
    eleLayout->addWidget(rxLineEdit, 3, 1);
    eleLayout->addWidget(_rxLabel, 3, 2);

    eleLayout->addWidget(ryLabel, 4, 0);
    eleLayout->addWidget(ryLineEdit, 4, 1);
    eleLayout->addWidget(_ryLabel, 4, 2);

    eleLayout->addWidget(rzLabel, 5, 0);
    eleLayout->addWidget(rzLineEdit, 5, 1);
    eleLayout->addWidget(_rzLabel, 5, 2);

    pubBtn = new FluPushButton(this);
    pubBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    pubBtn->setText("执行最优解");

    allIKSolBtn = new FluPushButton(this);
    allIKSolBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    allIKSolBtn->setText("执行所有解");


    eleLayout->addWidget(pubBtn, 6, 0, 1, 3, Qt::AlignVCenter);
    eleLayout->addWidget(allIKSolBtn, 7, 0, 1, 3, Qt::AlignVCenter);

    eleWidget->setLayout(eleLayout);

    ctrlLayout->addWidget(eleWidget);

    ctrlWidget->setLayout(ctrlLayout);
    return ctrlWidget;
}

QWidget *HomeWidget::makePosDisplayWidget() {
    auto posDisplayWidget = new QWidget(this);
    posDisplayWidget->setObjectName("posDisplayWidget");
    posDisplayWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    posDisplayWidget->setContentsMargins(5, 10, 5, 10);

    auto posDisPlayLayout = new QGridLayout(this);

    auto xVisualPosLabel = new FluLabel(this);
    xVisualPosLabel->setText("X:");
    xVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto yVisualPosLabel = new FluLabel(this);
    yVisualPosLabel->setText("Y:");
    yVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto zVisualPosLabel = new FluLabel(this);
    zVisualPosLabel->setText("Z:");
    zVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto rxVisualPosLabel = new FluLabel(this);
    rxVisualPosLabel->setText("Rx:");
    rxVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto ryVisualPosLabel = new FluLabel(this);
    ryVisualPosLabel->setText("Ry:");
    ryVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto rzVisualPosLabel = new FluLabel(this);
    rzVisualPosLabel->setText("Rz:");
    rzVisualPosLabel->setAlignment(Qt::AlignCenter);

    auto xPosLabel = new FluLabel(this);
    xPosLabel->setAlignment(Qt::AlignCenter);
    xPosLabel->setObjectName("xPosLabel");

    auto yPosLabel = new FluLabel(this);
    yPosLabel->setAlignment(Qt::AlignCenter);
    yPosLabel->setObjectName("yPosLabel");

    auto zPosLabel = new FluLabel(this);
    zPosLabel->setAlignment(Qt::AlignCenter);
    zPosLabel->setObjectName("zPosLabel");

    auto rxPosLabel = new FluLabel(this);
    rxPosLabel->setAlignment(Qt::AlignCenter);
    rxPosLabel->setObjectName("rxPosLabel");

    auto ryPosLabel = new FluLabel(this);
    ryPosLabel->setAlignment(Qt::AlignCenter);
    ryPosLabel->setObjectName("ryPosLabel");

    auto rzPosLabel = new FluLabel(this);
    rzPosLabel->setAlignment(Qt::AlignCenter);
    rzPosLabel->setObjectName("rzPosLabel");

    posDisPlayLayout->addWidget(xVisualPosLabel, 0, 0);
    posDisPlayLayout->addWidget(xPosLabel, 0, 1);

    posDisPlayLayout->addWidget(yVisualPosLabel, 0, 2);
    posDisPlayLayout->addWidget(yPosLabel, 0, 3);

    posDisPlayLayout->addWidget(zVisualPosLabel, 0, 4);
    posDisPlayLayout->addWidget(zPosLabel, 0, 5);


    posDisPlayLayout->addWidget(rxVisualPosLabel, 1, 0);
    posDisPlayLayout->addWidget(rxPosLabel, 1, 1);

    posDisPlayLayout->addWidget(ryVisualPosLabel, 1, 2);
    posDisPlayLayout->addWidget(ryPosLabel, 1, 3);

    posDisPlayLayout->addWidget(rzVisualPosLabel, 1, 4);
    posDisPlayLayout->addWidget(rzPosLabel, 1, 5);

    posDisplayWidget->setLayout(posDisPlayLayout);

    return posDisplayWidget;
}

QWidget *HomeWidget::makePointsWidget() {
    auto pointsWidget = new QWidget(this);
    pointsWidget->setObjectName("pointsWidget");
    pointsWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    pointsWidget->setContentsMargins(5, 10, 5, 10);

    auto pointsLayout = new QGridLayout(pointsWidget);
    pointsLayout->setSpacing(10);
    auto pointsCB = new FluComboBox(this);

    for (auto p: mPointMap) {
        pointsCB->addItem(p.first);
    }

    auto addBtn = new FluIconButton(FluAwesomeType::Add, this);
    addBtn->setToolTip("添加当前点");
    auto runBtn = new FluIconButton(FluAwesomeType::Robot, this);
    runBtn->setToolTip("运行当前选择的点");
    auto delBtn = new FluIconButton(FluAwesomeType::Delete, this);
    delBtn->setToolTip("删除当前点");

    pointsLayout->addWidget(pointsCB, 0, 0, 2, 0, Qt::AlignVCenter);

    pointsLayout->addWidget(runBtn, 2, 0);
    pointsLayout->addWidget(addBtn, 2, 1);
    pointsLayout->addWidget(delBtn, 2, 2);

    pointsWidget->setLayout(pointsLayout);

    connect(runBtn, &QPushButton::clicked, [=]() {
        auto t = pointsCB->currentText();
        auto p = mPointMap[t];
        emit sigJointMoveAnimation(p);
    });

    connect(delBtn, &QPushButton::clicked, [=]() {
        auto delFlyout = new FluConfirmFlyout(delBtn, FluFlyoutPosition::Left);
        delFlyout->setTitle("你是否希望删除当前点？");
        delFlyout->setInfo("注意：删除操作不可逆，点击确定按钮即可删除当前选择点");
        connect(delFlyout, &FluConfirmFlyout::sigOkClick, [=] {
            mPointMap.erase(pointsCB->currentText());
            InfoBar::showInfoBar(FluShortInfoBarType::Suc, "删除" + pointsCB->currentText() + "成功", this);
            pointsCB->removeItem(pointsCB->currentIndex());
        });
        delFlyout->show();
    });

    connect(addBtn, &QPushButton::clicked, [=]() {
        auto addFlyout = new FluLineEditFlyout(addBtn, FluFlyoutPosition::Left);
        addFlyout->setTitle("请输入新增点位名称");
        connect(addFlyout, &FluLineEditFlyout::sigOkClick, [=](QString s) {
            pointsCB->addItem(s);
            std::vector<double> rads;
            for (int i = 1; i < 7; ++i) {
                auto v = robotControlWidget->findChildren<QSlider *>("j" + QString::number(i) + "Slider")[0]->value() /
                         100 * (M_PI / 180.0);
                rads.push_back(v);
            }
            mPointMap[s] = rads;
            pointsCB->setCurrentText(s);
            InfoBar::showInfoBar(FluShortInfoBarType::Suc, "保存" + s + "成功", this);
        });

        addFlyout->show();
    });


    return pointsWidget;
}

void HomeWidget::slotUpdateCheckOpt(bool arg) {
    QCheckBox *cb = (QCheckBox *) sender();
    QString cbName = cb->objectName();
    auto key = cbName.split("CheckBox").at(0);
//    qDebug() << "key: " << key;
//    emit sigCheckOptChanged(key, arg);
    osgWidget->updateSceneDisplay(key.toStdString(), arg);
}

void HomeWidget::slotUpdateJVarsValue(int var) {
    QSlider *senderSlider = (QSlider *) sender();
    QString sliderName = senderSlider->objectName();
    QString idx = sliderName.at(1);
    emit sigJoinVarChanged(idx.toInt(), var / 100.0);
}

QWidget *HomeWidget::makeObservationWidget() {
    QWidget *obsWidget = new QWidget(this);
    obsWidget->setObjectName("obsWidget");
    obsWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

    auto obsLayout = new QGridLayout(obsWidget);

    obsLayout->setContentsMargins(10, 5, 10, 5);
    auto initObsBtn = new FluPushButton(this);
    auto frontObsBtn = new FluPushButton(this);
    auto topObsBtn = new FluPushButton(this);
    auto sideObsBtn = new FluPushButton(this);
    initObsBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    frontObsBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    topObsBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    sideObsBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    initObsBtn->setText("初始视图");
    frontObsBtn->setText("正视图");
    topObsBtn->setText("俯视图");
    sideObsBtn->setText("侧视图");
    obsLayout->addWidget(initObsBtn, 0, 0);
    obsLayout->addWidget(frontObsBtn, 0, 1);
    obsLayout->addWidget(topObsBtn, 1, 0);
    obsLayout->addWidget(sideObsBtn, 1, 1);
    obsWidget->setLayout(obsLayout);
    connect(initObsBtn, &QPushButton::clicked, [=] {
        osgWidget->changeObservationSlot(OBSERVATION_ENUM::INIT);
        InfoBar::showInfoBar(FluShortInfoBarType::Info, "已切换至初始视图", this);

    });
    connect(frontObsBtn, &QPushButton::clicked, [=] {
        osgWidget->changeObservationSlot(OBSERVATION_ENUM::FONT);
        InfoBar::showInfoBar(FluShortInfoBarType::Info, "已切换至正视图", this);

    });
    connect(topObsBtn, &QPushButton::clicked, [=] {
        osgWidget->changeObservationSlot(OBSERVATION_ENUM::TOP);
        InfoBar::showInfoBar(FluShortInfoBarType::Info, "已切换至俯视图", this);

    });
    connect(sideObsBtn, &QPushButton::clicked, [=] {
        osgWidget->changeObservationSlot(OBSERVATION_ENUM::SIDE);
        InfoBar::showInfoBar(FluShortInfoBarType::Info, "已切换至侧视图", this);
    });

    return obsWidget;
}

void HomeWidget::slotUpdateStepMove() {
    QPushButton *btn = (QPushButton *) sender();
    QString name = btn->objectName();
    qDebug() << name;
    auto key = name.split("_");
    auto k1 = key[0];
    auto k2 = key[1];
    qDebug() << k1;
    qDebug() << k2;
    auto sp = robotControlWidget->findChildren<QWidget *>("endEleWidget")[0]->findChildren<FluDoubleSpinBox *>()[0];
    auto v = sp->value();
    qDebug() << v;
}

HomeWidget::~HomeWidget() {
    m_pointSetting->clear();
    for (auto p: mPointMap) {
        QVariantList list;
        for (double value: p.second) {
            list.append(value);
        }
        m_pointSetting->setValue(p.first, list);
    }
    qDebug() << "save points...";
}
