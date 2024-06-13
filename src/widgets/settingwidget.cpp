
#include "settingwidget.h"
#include "components/fluwidgets/FluIconButton.h"
#include "tools/infobar.h"

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
    m_appBehaviorLabel->setText("设置");
    m_vScrollView->getMainLayout()->addWidget(m_appBehaviorLabel);

    appThemeSelectBox = new FluSettingsSelectBox;
    appThemeSelectBox->setTitleInfo("主题模式", "选择一个应用于RobotSimulation的主题模式");
    appThemeSelectBox->setIcon(FluAwesomeType::Color);

    appThemeSelectBox->getComboBox()->addItem("明亮模式");
    appThemeSelectBox->getComboBox()->addItem("黑暗模式");

    connect(appThemeSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
        else
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);
    });

    appLanguageSelectBox = new FluSettingsSelectBox;
    appLanguageSelectBox->setTitleInfo("语言", "选择一个应用于RobotSimulation的语言");
    appLanguageSelectBox->setIcon(FluAwesomeType::LocaleLanguage);

    appLanguageSelectBox->getComboBox()->addItem("中文");
    appLanguageSelectBox->getComboBox()->addItem("English");

    connect(appLanguageSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            qDebug() << "cn";
        else {
            /// todo en Language translation
            qDebug() << "en";
            InfoBar::showInfoBar(FluShortInfoBarType::Warn, "English translation is not yet available", this);
        }
    });


    m_vScrollView->getMainLayout()->addWidget(appThemeSelectBox);
    m_vScrollView->getMainLayout()->addWidget(appLanguageSelectBox);

    m_vScrollView->getMainLayout()->addSpacing(30);

    m_aboutLabel = new QLabel;
    m_aboutLabel->setObjectName("aboutLabel");
    m_aboutLabel->setText("关于");
    m_vScrollView->getMainLayout()->addWidget(m_aboutLabel);

    auto aboutLabelBox = new FluSettingsLabelBox;
    aboutLabelBox->setTitleInfo("RobotSimulation",
                                "RobotSimulation是一个兴趣使然的开源项目，使用了宽松的MIT协议，主要用于机械手的仿真测试");
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
