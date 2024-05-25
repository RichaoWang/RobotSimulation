
#include "homewidget.h"
#include "tools/infobar.h"
#include "components/FluSlider.h"
#include "components/FluCheckBox.h"
#include "components/FluLineEdit.h"
#include "components/FluPushButton.h"
#include "components/FluExpander.h"
#include "components/FluLabel.h"
#include "components/FluDoubleSpinBox.h"
#include "components/FluComboBox.h"

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QHBoxLayout;
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(10, 10, 10, 15);

    /// robot display widget
    ddr6widget = new DDR6RobotWidget(this);
    ddr6widget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    m_mainLayout->addWidget(ddr6widget);
    m_mainLayout->addWidget(ddr6widget);

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

    /// -------------------------- pos display part --------------------------
    auto posDisplayExpander = new FluExpander(this);
    auto posDisplayExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    posDisplayExpLabel->setText("RealtimePosition");
    posDisplayExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    posDisplayExpander->getWrap1Layout()->addWidget(posDisplayExpLabel);
    QWidget *posDisplayWidget = makePosDisplayWidget();
    posDisplayExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    posDisplayExpander->getWrap2Layout()->addWidget(posDisplayWidget);
    controlScrollView->getMainLayout()->addWidget(posDisplayExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------


    /// -------------------------- opt part --------------------------
    auto optExpander = new FluExpander(this);
    auto optExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    optExpLabel->setText("CheckOption");
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
    pointsExpLabel->setText("Points");
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
    ctrlExpLabel->setText("RobotControl");
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


    robotKinematics=std::make_unique<RobotKinematics>();

    connectSignsSlots();


    /// default option
    checkboxesWidget->findChildren<QCheckBox*>("gridCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox*>("endAxisCheckBox")[0]->setChecked(true);
//    checkboxesWidget->findChildren<QCheckBox*>("worldAxisCheckBox")[0]->setChecked(true);
    checkboxesWidget->findChildren<QCheckBox*>("deskCheckBox")[0]->setChecked(true);

    posDisplayExpander->expand();
    optExpander->expand();
    ctrlExpander->expand();
    pointsExpander->expand();
}

void HomeWidget::connectSignsSlots() {
    connect(this, &HomeWidget::sigJoinVarChanged, this, [=](int idx, int var) {
        if (idx == 2 || idx == 3 || idx == 4) {
            ddr6widget->mRobotConfig.JVars[idx] = -var;
        } else {
            ddr6widget->mRobotConfig.JVars[idx] = var;
        }
        ddr6widget->update();
    }, Qt::QueuedConnection);
    {
        auto jointSliders = robotControlWidget->findChildren<QSlider *>();
        for (int i = 0; i < jointSliders.size(); ++i) {
            QSlider *slider = jointSliders.at(i);
            slider->setMinimum(-180);
            slider->setMaximum(180);
            slider->setTickInterval(1);
            connect(slider, &QSlider::valueChanged, this, &HomeWidget::slotUpdateJVarsValue,
                    Qt::ConnectionType::QueuedConnection);
        }
    }


    connect(this, &HomeWidget::sigCheckOptChanged, ddr6widget, [=](QString key, bool arg) {
        if (key == "grid") {
            ddr6widget->mGlobalConfig.isDrawGrid = arg;
        } else if (key == "worldAxis") {
            ddr6widget->mGlobalConfig.isDrawWorldCoord = arg;
        } else if (key == "j1") {
            ddr6widget->mGlobalConfig.isDrawJoint1Coord = arg;
        } else if (key == "j2") {
            ddr6widget->mGlobalConfig.isDrawJoint2Coord = arg;
        } else if (key == "j3") {
            ddr6widget->mGlobalConfig.isDrawJoint3Coord = arg;
        } else if (key == "j4") {
            ddr6widget->mGlobalConfig.isDrawJoint4Coord = arg;
        } else if (key == "j5") {
            ddr6widget->mGlobalConfig.isDrawJoint5Coord = arg;
        } else if (key == "j6") {
            ddr6widget->mGlobalConfig.isDrawJoint6Coord = arg;
        } else if (key == "desk") {
            ddr6widget->mGlobalConfig.isDrawDesk = arg;
        } else if (key == "endAxis") {
            ddr6widget->mGlobalConfig.isDrawEnd = arg;
        } else {
            return;
        }
        ddr6widget->update();
    }, Qt::ConnectionType::QueuedConnection);

    {
        auto checkBoxes = checkboxesWidget->findChildren<QCheckBox *>();
        for (int i = 0; i < checkBoxes.size(); ++i) {
            QCheckBox *checkBox = checkBoxes.at(i);
            connect(checkBox, &QCheckBox::toggled, this, &HomeWidget::slotUpdateCheckOpt,
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
    auto gridCb = new FluCheckBox("Grid", this);
    gridCb->setObjectName("gridCheckBox");
    gridCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto worldCoordCb = new FluCheckBox("WorldAxis", this);
    worldCoordCb->setObjectName("worldAxisCheckBox");
    worldCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto EndCoordCb = new FluCheckBox("EndAxis", this);
    EndCoordCb->setObjectName("endAxisCheckBox");
    EndCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto DeskCb = new FluCheckBox("Desk", this);
    DeskCb->setObjectName("deskCheckBox");
    DeskCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    checkBoxesLayout->addWidget(gridCb, 0, 0);
    checkBoxesLayout->addWidget(worldCoordCb, 0, 1);
    checkBoxesLayout->addWidget(EndCoordCb, 1, 0);
    checkBoxesLayout->addWidget(DeskCb, 1, 1);
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
    sliderLabel->setText("ControlWithJoint");
    ctrlLayout->addWidget(sliderLabel);

    /// slider group
    auto sliderWidget = new QWidget(this);
    sliderWidget->setObjectName("sliderWidget");
    sliderWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    auto sldLayout = new QGridLayout(this);
    sldLayout->setContentsMargins(10, 5, 10, 5);

    auto j1Label = new FluLabel(this);
    j1Label->setText("Joint1");
    j1Label->setAlignment(Qt::AlignCenter);

    auto j2Label = new FluLabel(this);
    j2Label->setText("Joint2");
    j2Label->setAlignment(Qt::AlignCenter);

    auto j3Label = new FluLabel(this);
    j3Label->setText("Joint3");
    j3Label->setAlignment(Qt::AlignCenter);

    auto j4Label = new FluLabel(this);
    j4Label->setText("Joint4");
    j4Label->setAlignment(Qt::AlignCenter);

    auto j5Label = new FluLabel(this);
    j5Label->setText("Joint5");
    j5Label->setAlignment(Qt::AlignCenter);

    auto j6Label = new FluLabel(this);
    j6Label->setText("Joint6");
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

    auto j2ValueLabel = new FluLabel(this);
    j2ValueLabel->setAlignment(Qt::AlignCenter);
    j2ValueLabel->setObjectName("j2ValueLabel");

    auto j3ValueLabel = new FluLabel(this);
    j3ValueLabel->setAlignment(Qt::AlignCenter);
    j3ValueLabel->setObjectName("j3ValueLabel");

    auto j4ValueLabel = new FluLabel(this);
    j4ValueLabel->setAlignment(Qt::AlignCenter);
    j4ValueLabel->setObjectName("j4ValueLabel");

    auto j5ValueLabel = new FluLabel(this);
    j5ValueLabel->setAlignment(Qt::AlignCenter);
    j5ValueLabel->setObjectName("j5ValueLabel");

    auto j6ValueLabel = new FluLabel(this);
    j6ValueLabel->setAlignment(Qt::AlignCenter);
    j6ValueLabel->setObjectName("j6ValueLabel");

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

//    auto posLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle,this);
//    auto posLabel = new FluLabel(FluLabelStyle::SubTitleTextBlockStyle,this);
    auto posLabel = new FluLabel(this);
    posLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    posLabel->setText("ControlWithEulerAngle");
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

    auto pubBtn = new FluPushButton(this);
    pubBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    pubBtn->setText("Publish");
    eleLayout->addWidget(pubBtn, 6, 0, 3, 0, Qt::AlignVCenter);

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
    pointsCB->addItem("ZeroPoint");
    pointsCB->addItem("InitPoint");

    auto addBtn = new FluIconButton(FluAwesomeType::Add, this);
    addBtn->setToolTip("add current point");
    auto runBtn = new FluIconButton(FluAwesomeType::Robot, this);
    runBtn->setToolTip("run select point");
    auto delBtn = new FluIconButton(FluAwesomeType::Delete, this);
    delBtn->setToolTip("delete select point");

    pointsLayout->addWidget(pointsCB, 0, 0, 2, 0, Qt::AlignVCenter);

    pointsLayout->addWidget(runBtn, 3, 0);
    pointsLayout->addWidget(addBtn, 3, 1);
    pointsLayout->addWidget(delBtn, 3, 2);

    pointsWidget->setLayout(pointsLayout);

    return pointsWidget;
}

void HomeWidget::slotUpdateCheckOpt(bool arg) {
    QCheckBox *cb = (QCheckBox *) sender();
    QString cbName = cb->objectName();
    auto key = cbName.split("CheckBox").at(0);
    emit sigCheckOptChanged(key, arg);
}

void HomeWidget::slotUpdateJVarsValue(int var) {
    QSlider *senderSlider = (QSlider *) sender();
    QString sliderName = senderSlider->objectName();
    QString idx = sliderName.at(1);
//    auto label = this->findChildren<QLabel *>("j" + idx + "Label");
//    label.at(0)->setText(QString::number(var));
//    emit sigJointVar
    emit sigJoinVarChanged(idx.toInt(), var);
}
