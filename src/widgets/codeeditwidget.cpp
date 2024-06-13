
#include "codeeditwidget.h"
#include "components/fluwidgets/FluPivot.h"
#include "components/fluwidgets/FluLabel.h"
#include "components/fluwidgets/FluPushButton.h"
//#include "components/fluwidgets/FluMenuBar.h"
//#include "components/fluwidgets/FluMenu.h"
#include "tools/infobar.h"


CodeEditWidget::CodeEditWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(35, 10, 35, 10);


    auto pivot = new FluPivot(this);
//    pivot->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    /// todo node edit
    auto nodeTitleItem = new FluPivotTitleBarItem(this);
    nodeTitleItem->setKey("节点编程");
    nodeTitleItem->setSelected(true);
    /// todo test label
    auto testnodelabel = new FluLabel(FluLabelStyle::DisplayTextBlockStyle, this);
    testnodelabel->setText("未实现节点编程");
    testnodelabel->setAlignment(Qt::AlignCenter);
    pivot->addPivotItem(nodeTitleItem, testnodelabel);


    /// todo code edit
    auto codeTitleItem = new FluPivotTitleBarItem(this);
    codeTitleItem->setKey("代码编程");
    codeTitleItem->setSelected(false);
    /// todo code edit widget
    QWidget * codeEditWidget = makeCodeEditPage();
    pivot->addPivotItem(codeTitleItem, codeEditWidget);


    m_mainLayout->addWidget(pivot);


    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/CodeEditWidget.qss", this);
}

void CodeEditWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CodeEditWidget::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/CodeEditWidget.qss", this);

    } else {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/CodeEditWidget.qss", this);

    }

}

QWidget *CodeEditWidget::makeCodeEditPage() {
    auto codeEditWidget = new QWidget(this);
    codeEditWidget->setObjectName("codeEditPage");
    auto vCodeEditLayout = new QVBoxLayout(this);

    /// todo rs code editor
    rsCodeEdit = makeRSCodeEditWidget();
    vCodeEditLayout->addWidget(rsCodeEdit);

    auto ctrlWidget = new QWidget(this);
    ctrlWidget->setObjectName("codeEditCtrlWidget");

    auto ctrlLayout = new QHBoxLayout(this);
    ctrlLayout->setContentsMargins(50, 10, 50, 10);
    ctrlLayout->setSpacing(50);

    auto newBtn = new FluPushButton(this);
    newBtn->setText("New");

    auto loadBtn = new FluPushButton(this);
    loadBtn->setText("Load");

    auto saveBtn = new FluPushButton(this);
    saveBtn->setText("Save");

    auto clearBtn = new FluPushButton(this);
    clearBtn->setText("Clear");
    connect(clearBtn, &QPushButton::clicked, [=]() {
        /// todo 清楚code edit
        rsCodeEdit->clear();
    });

    ctrlLayout->addWidget(newBtn);
    ctrlLayout->addWidget(loadBtn);
    ctrlLayout->addWidget(saveBtn);
    ctrlLayout->addWidget(clearBtn);
    ctrlWidget->setLayout(ctrlLayout);


    codeEditWidget->setLayout(vCodeEditLayout);

    vCodeEditLayout->addWidget(ctrlWidget);


    return codeEditWidget;
}

QCodeEditor *CodeEditWidget::makeRSCodeEditWidget() {
    auto _rsCodeEdit = new QCodeEditor(this);

    auto rsCode = CodeHelper::loadCode(":/code_edit/robotsimulation.rs");
    auto rsCompleter = new QRSCompleter(this);
    auto rsHighlighter = new QRSHighlighter;

    _rsCodeEdit->setPlainText(rsCode);
    _rsCodeEdit->setSyntaxStyle(QSyntaxStyle::defaultStyle());
    _rsCodeEdit->setCompleter(rsCompleter);
    _rsCodeEdit->setHighlighter(rsHighlighter);

    return _rsCodeEdit;
}
