
#include "settingwidget.h"
#include "components/FluIconButton.h"

SettingWidget::SettingWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(35, 35, 35, 35);

    m_vScrollView = new FluVScrollView;
    m_vScrollView->setObjectName("vScrollView");
    m_mainLayout->addWidget(m_vScrollView, 1);

    m_vScrollView->getMainLayout()->setAlignment(Qt::AlignTop);

    m_appBehaviorLabel = new QLabel;
    m_appBehaviorLabel->setObjectName("appBehaviorLabel");
    m_appBehaviorLabel->setText("Appearance");
    m_vScrollView->getMainLayout()->addWidget(m_appBehaviorLabel);

    appThemeSelectBox = new FluSettingsSelectBox;
    appThemeSelectBox->setTitleInfo("AppTheme", "Select which RobotSimulation theme to display");
    appThemeSelectBox->setIcon(FluAwesomeType::Color);

    appThemeSelectBox->getComboBox()->addItem("Light");
    appThemeSelectBox->getComboBox()->addItem("Dark");

    connect(appThemeSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
        else
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);
    });

    m_vScrollView->getMainLayout()->addWidget(appThemeSelectBox);

    m_vScrollView->getMainLayout()->addSpacing(30);

    m_aboutLabel = new QLabel;
    m_aboutLabel->setObjectName("aboutLabel");
    m_aboutLabel->setText("About");
    m_vScrollView->getMainLayout()->addWidget(m_aboutLabel);

    auto aboutLabelBox = new FluSettingsLabelBox;
    aboutLabelBox->setTitleInfo("RobotSimulation", "RobotSimulation© 2024 RichaoWang, All rights reserved.");
    aboutLabelBox->setIcon(QIcon(":icon/icon.ico"), 30, 30);
    aboutLabelBox->setVersion("");  // empty it!;
    m_vScrollView->getMainLayout()->addWidget(aboutLabelBox);
    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/SettingWidget.qss", this);
}

void SettingWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void SettingWidget::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/SettingWidget.qss", this);
    } else {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/SettingWidget.qss", this);
    }
}
