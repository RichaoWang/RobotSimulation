
#include "homewidget.h"
#include "tools/infobar.h"
#include "ddr6robotwidget.h"
#include "components/FluSlider.h"
#include "components/FluCheckBox.h"
#include "components/FluLineEdit.h"
#include "components/FluPushButton.h"
#include "components/FluExpander.h"
#include "components/FluLabel.h"
#include "components/FluDoubleSpinBox.h"

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QHBoxLayout;
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(10, 10, 10, 15);

    /// robot display
    auto ddr6widget = new DDR6RobotWidget(this);
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

    /// todo position visualiztion


    /// -------------------------- opt part --------------------------
    auto optExpander = new FluExpander(this);
    auto optExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    optExpLabel->setText("CheckOption");
    optExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    optExpander->getWrap1Layout()->addWidget(optExpLabel);
    QWidget *checkboxesWidget = makeCheckOptionWidget();
    optExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    optExpander->getWrap2Layout()->addWidget(checkboxesWidget);
    controlScrollView->getMainLayout()->addWidget(optExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------


    /// -------------------------- control part --------------------------
    auto ctrlExpander = new FluExpander(this);
    auto ctrlExpLabel = new FluLabel(FluLabelStyle::BodyStrongTextBlockStyle, this);
    ctrlExpLabel->setText("RobotControl");
    ctrlExpander->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    ctrlExpander->getWrap1Layout()->addWidget(ctrlExpLabel);
    QWidget *robotControlWidget = makeRobotControlWidget();
    ctrlExpander->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    ctrlExpander->getWrap2Layout()->addWidget(robotControlWidget);
    controlScrollView->getMainLayout()->addWidget(ctrlExpander);
    controlScrollView->getMainLayout()->addSpacing(10);
    /// -------------------------------------------------------------


    /// -------------------------- todo teach part --------------------------

    /// -------------------------------------------------------------


    m_mainLayout->addWidget(controlWidget);


    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/HomeWidget.qss", this);
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
    checkboxesWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto checkBoxesLayout = new QGridLayout(this);
    checkBoxesLayout->setContentsMargins(10, 5, 10, 5);
    auto gridCb = new FluCheckBox("Grid", this);
    gridCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto worldCoordCb = new FluCheckBox("WorldCoord", this);
    worldCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto EndCoordCb = new FluCheckBox("EndCoord", this);
    EndCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto DeskCb = new FluCheckBox("Desk", this);
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

    auto ctrlLayout = new QVBoxLayout(this);
    ctrlLayout->setSpacing(2);

    auto sliderLabel = new FluLabel(this);
    sliderLabel->setText("ControlWithJoint");
    ctrlLayout->addWidget(sliderLabel);

    /// slider group
    auto sliderWidget = new QWidget(this);
    sliderWidget->setObjectName("sliderWidget");
    sliderWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
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

    auto posLabel = new FluLabel(this);
    posLabel->setText("ControlWithEulerAngle");
    ctrlLayout->addWidget(posLabel);

    /// line edit group
    auto eleWidget = new QWidget(this);
    eleWidget->setObjectName("eleWidget");
    eleWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
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
    eleLayout->addWidget(pubBtn, 6, 0, 3, 0, Qt::AlignCenter);

    eleWidget->setLayout(eleLayout);

    ctrlLayout->addWidget(eleWidget);

    ctrlWidget->setLayout(ctrlLayout);
    return ctrlWidget;
}
