
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
#include "widgets/robotcontrolwidget.h"

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QHBoxLayout;
//    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(10, 10, 10, 15);

//    m_vScrollView = new FluVScrollView;
//    m_vScrollView->setObjectName("vScrollView");
//    m_mainLayout->addWidget(m_vScrollView, 1);
//    m_vScrollView->getMainLayout()->setAlignment(Qt::AlignTop);

    /// robot display
    auto ddr6widget = new DDR6RobotWidget(this);
    ddr6widget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    m_mainLayout->addWidget(ddr6widget);
    m_mainLayout->addWidget(ddr6widget);


    /// control
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

    /// control-checkbox group
    auto cbLabel = new QLabel;
    cbLabel->setObjectName("cbLabel");
    cbLabel->setText("CheckOption");
    auto checkboxesWidget = new QWidget;
    checkboxesWidget->setObjectName("checkboxesWidget");
    checkboxesWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    auto checkBoxesLayout = new QGridLayout;
    checkBoxesLayout->setContentsMargins(10, 5, 10, 5);
    auto gridCb = new FluCheckBox("Grid");
    gridCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto worldCoordCb = new FluCheckBox("WorldCoord");
    worldCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto EndCoordCb = new FluCheckBox("EndCoord");
    EndCoordCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    auto DeskCb = new FluCheckBox("Desk");
    DeskCb->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

    checkBoxesLayout->addWidget(gridCb, 0, 0);
    checkBoxesLayout->addWidget(worldCoordCb, 0, 1);
    checkBoxesLayout->addWidget(EndCoordCb, 1, 0);
    checkBoxesLayout->addWidget(DeskCb, 1, 1);
    checkboxesWidget->setLayout(checkBoxesLayout);
    controlScrollView->getMainLayout()->addWidget(cbLabel);
    controlScrollView->getMainLayout()->addWidget(checkboxesWidget);

    controlScrollView->getMainLayout()->addSpacing(10);


    /// control-slider group
    auto sldLabel = new QLabel;
    sldLabel->setObjectName("sldLabel");
    sldLabel->setText("RobotJoints");
    auto sliderWidget = new QWidget;
    sliderWidget->setObjectName("sliderWidget");
    sliderWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    auto sldLayout = new QGridLayout;
    sldLayout->setContentsMargins(10, 5, 10, 5);

    auto j1Label = new QLabel("Joint1 ");
    j1Label->setObjectName("j1Label");
    j1Label->setAlignment(Qt::AlignCenter);

    auto j2Label = new QLabel("Joint2 ");
    j2Label->setObjectName("j2Label");
    j2Label->setAlignment(Qt::AlignCenter);

    auto j3Label = new QLabel("Joint3 ");
    j3Label->setObjectName("j3Label");
    j3Label->setAlignment(Qt::AlignCenter);

    auto j4Label = new QLabel("Joint4 ");
    j4Label->setObjectName("j4Label");
    j4Label->setAlignment(Qt::AlignCenter);

    auto j5Label = new QLabel("Joint5 ");
    j5Label->setObjectName("j5Label");
    j5Label->setAlignment(Qt::AlignCenter);

    auto j6Label = new QLabel("Joint6 ");
    j6Label->setObjectName("j6Label");
    j6Label->setAlignment(Qt::AlignCenter);

    auto j1Slider = new FluSlider(Qt::Orientation::Horizontal);
    j1Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j2Slider = new FluSlider(Qt::Orientation::Horizontal);
    j2Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j3Slider = new FluSlider(Qt::Orientation::Horizontal);
    j3Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j4Slider = new FluSlider(Qt::Orientation::Horizontal);
    j4Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j5Slider = new FluSlider(Qt::Orientation::Horizontal);
    j5Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j6Slider = new FluSlider(Qt::Orientation::Horizontal);
    j6Slider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);

    auto j1ValueLabel = new QLabel("0°");
    j1ValueLabel->setAlignment(Qt::AlignCenter);
    j1ValueLabel->setObjectName("j1ValueLabel");

    auto j2ValueLabel = new QLabel("0°");
    j2ValueLabel->setAlignment(Qt::AlignCenter);
    j2ValueLabel->setObjectName("j2ValueLabel");

    auto j3ValueLabel = new QLabel("0°");
    j3ValueLabel->setAlignment(Qt::AlignCenter);
    j3ValueLabel->setObjectName("j3ValueLabel");

    auto j4ValueLabel = new QLabel("0°");
    j4ValueLabel->setAlignment(Qt::AlignCenter);
    j4ValueLabel->setObjectName("j4ValueLabel");

    auto j5ValueLabel = new QLabel("0°");
    j5ValueLabel->setAlignment(Qt::AlignCenter);
    j5ValueLabel->setObjectName("j5ValueLabel");

    auto j6ValueLabel = new QLabel("0°");
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
    controlScrollView->getMainLayout()->addWidget(sldLabel);
    controlScrollView->getMainLayout()->addWidget(sliderWidget);

    controlScrollView->getMainLayout()->addSpacing(10);

    /// control-lineedit group  RobotEndPosition
    auto eleLabel = new QLabel;
    eleLabel->setObjectName("eleLabel");
    eleLabel->setText("RobotEndPosition");
    auto eleWidget = new QWidget;
    eleWidget->setObjectName("eleWidget");
    eleWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    auto eleLayout = new QGridLayout;
    eleLayout->setContentsMargins(10, 5, 10, 5);

    auto xLabel = new QLabel("X");
    xLabel->setObjectName("xLabel");
    xLabel->setAlignment(Qt::AlignCenter);

    auto yLabel = new QLabel("Y");
    yLabel->setAlignment(Qt::AlignCenter);
    yLabel->setObjectName("yLabel");

    auto zLabel = new QLabel("Z");
    zLabel->setAlignment(Qt::AlignCenter);
    zLabel->setObjectName("zLabel");

    auto rxLabel = new QLabel("Rx");
    rxLabel->setAlignment(Qt::AlignCenter);
    rxLabel->setObjectName("rxLabel");

    auto ryLabel = new QLabel("Ry");
    ryLabel->setAlignment(Qt::AlignCenter);
    ryLabel->setObjectName("ryLabel");

    auto rzLabel = new QLabel("Rz");
    rzLabel->setAlignment(Qt::AlignCenter);
    rzLabel->setObjectName("rzLabel");


    auto xLineEdit = new FluLineEdit;
    xLineEdit->setAlignment(Qt::AlignCenter);
    xLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    xLineEdit->setMinimumWidth(150);
    xLineEdit->setMinimumHeight(30);

    auto yLineEdit = new FluLineEdit;
    yLineEdit->setAlignment(Qt::AlignCenter);
    yLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    yLineEdit->setMinimumHeight(30);

    auto zLineEdit = new FluLineEdit;
    zLineEdit->setAlignment(Qt::AlignCenter);
    zLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    zLineEdit->setMinimumHeight(30);

    auto rxLineEdit = new FluLineEdit;
    rxLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    rxLineEdit->setAlignment(Qt::AlignCenter);
    rxLineEdit->setMinimumHeight(30);

    auto ryLineEdit = new FluLineEdit;
    ryLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    ryLineEdit->setAlignment(Qt::AlignCenter);
    ryLineEdit->setMinimumHeight(30);

    auto rzLineEdit = new FluLineEdit;
    rzLineEdit->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    rzLineEdit->setAlignment(Qt::AlignCenter);
    rzLineEdit->setMinimumHeight(30);


    auto _xLabel = new QLabel("mm");
    _xLabel->setAlignment(Qt::AlignCenter);
    _xLabel->setObjectName("_xLabel");

    auto _yLabel = new QLabel("mm");
    _yLabel->setAlignment(Qt::AlignCenter);
    _yLabel->setObjectName("_yLabel");

    auto _zLabel = new QLabel("mm");
    _zLabel->setAlignment(Qt::AlignCenter);
    _zLabel->setObjectName("_zLabel");

    auto _rxLabel = new QLabel("°");
    _rxLabel->setAlignment(Qt::AlignCenter);
    _rxLabel->setObjectName("_rxLabel");

    auto _ryLabel = new QLabel("°");
    _ryLabel->setAlignment(Qt::AlignCenter);
    _ryLabel->setObjectName("_ryLabel");

    auto _rzLabel = new QLabel("°");
    _rzLabel->setAlignment(Qt::AlignCenter);
    _rzLabel->setObjectName("_rzLabel");

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

    auto posBtn = new FluPushButton;
    posBtn->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
    posBtn->setText("Publish");
    eleLayout->addWidget(posBtn, 6, 0, 3, 0, Qt::AlignCenter);

    eleWidget->setLayout(eleLayout);
    controlScrollView->getMainLayout()->addWidget(eleLabel);
    controlScrollView->getMainLayout()->addWidget(eleWidget);

    /// todo test expend
    auto exp = new FluExpander;
    auto titlabel = new FluLabel;
    titlabel->setObjectName("titlabel");
    titlabel->setText("Title Text");
//    titlabel->setFont(QFont("", 20, 75));
    exp->getWrap1Layout()->setAlignment(Qt::AlignCenter);
    exp->getWrap1Layout()->addWidget(titlabel);
    controlScrollView->getMainLayout()->addWidget(exp);

    auto testw = new QWidget;
    testw->setFixedHeight(200);
    exp->getWrap2Layout()->setAlignment(Qt::AlignCenter);
    exp->getWrap2Layout()->addWidget(testw);

    auto testspb=new FluDoubleSpinBox;
    controlScrollView->getMainLayout()->addWidget(testspb);


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
