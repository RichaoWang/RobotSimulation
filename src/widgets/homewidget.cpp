
#include "homewidget.h"
#include "tools/infobar.h"
#include "ddr6robotwidget.h"

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(35, 35, 35, 35);

    m_vScrollView = new FluVScrollView;
    m_vScrollView->setObjectName("vScrollView");
    m_mainLayout->addWidget(m_vScrollView, 1);

    m_vScrollView->getMainLayout()->setAlignment(Qt::AlignTop);
    m_midLayout = new QVBoxLayout(m_vScrollView);
    auto ddr6w = new DDR6RobotWidget;
    m_midLayout->addWidget(ddr6w);


//    QWidget * ddr6robot = DDR6RobotWidget();
//    m_vScrollView->getMainLayout()->addWidget(ddr6robot);


//    m_appBehaviorLabel = new QLabel;
//    m_appBehaviorLabel->setObjectName("appBehaviorLabel");
//    m_appBehaviorLabel->setText("Appearance");
//    m_vScrollView->getMainLayout()->addWidget(m_appBehaviorLabel);
//
//    appThemeSelectBox = new FluSettingsSelectBox;
//    appThemeSelectBox->setTitleInfo("AppTheme", "Select which RobotSimulation theme to display");
//    appThemeSelectBox->setIcon(FluAwesomeType::Color);
//
//    appThemeSelectBox->getComboBox()->addItem("Light");
//    appThemeSelectBox->getComboBox()->addItem("Dark");
//
//    m_vScrollView->getMainLayout()->addWidget(appThemeSelectBox);

//    m_vScrollView->getMainLayout()->addSpacing(30);

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
