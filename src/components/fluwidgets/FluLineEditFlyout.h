#pragma once

#include "FluLabel.h"
#include "FluVFlyout.h"
#include "components/fluwidgets/utils/FluUtils.h"
#include "FluStyleButton.h"
#include "FluPushButton.h"
#include "FluTextEdit.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>


/// new add: 支持lineEdit 的 flyout
class FluLineEditFlyout : public FluVFlyout {
Q_OBJECT
public:
    FluLineEditFlyout(QWidget *target, FluFlyoutPosition position = FluFlyoutPosition::Top) : FluVFlyout(target,
                                                                                                         position) {
        m_titleLabel = new FluLabel;
        m_titleLabel->setWordWrap(true);
        m_titleLabel->setLabelStyle(FluLabelStyle::SubTitleTextBlockStyle);

        m_textEdit = new FluTextEdit;
        m_textEdit->setFixedHeight(25);

        m_vShadowLayout->addWidget(m_titleLabel);
        m_vShadowLayout->addWidget(m_textEdit);

        m_hBtnLayout = new QHBoxLayout;
        m_vShadowLayout->addLayout(m_hBtnLayout);

        m_okBtn = new FluStyleButton;
        m_cancelBtn = new FluPushButton;
        m_hBtnLayout->addWidget(m_okBtn);
        m_hBtnLayout->addWidget(m_cancelBtn);

        m_cancelBtn->setFixedWidth(100);
        m_okBtn->setFixedWidth(100);
        setFixedWidth(260);

        m_okBtn->setText("确定");
        m_cancelBtn->setText("返回");

        connect(m_okBtn, &FluStyleButton::clicked, [=]() {
            QString s = m_textEdit->toPlainText();
            emit sigOkClick(s);
            close();
        });

        connect(m_cancelBtn, &FluPushButton::clicked, [=]() {
            emit sigCancelClick();
            close();
        });

        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluConfirmFlyout.qss", this);
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Dark) {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluConfirmFlyout.qss", this);
        }
    }

    void setTitle(QString title) {
        m_titleLabel->setText(title);
    }


    void setOk(QString ok) {
        m_okBtn->setText(ok);
    }

    void setCancel(QString cancel) {
        m_cancelBtn->setText(cancel);
    }

    void paintEvent(QPaintEvent *event) {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }

public slots:

    void onThemeChanged() {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluConfirmFlyout.qss", this);
        } else {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluConfirmFlyout.qss", this);
        }
    }

signals:

    void sigOkClick(QString s);

    void sigCancelClick();

protected:
    FluLabel *m_titleLabel;
    FluTextEdit *m_textEdit;
    FluStyleButton *m_okBtn;
    FluPushButton *m_cancelBtn;

    QHBoxLayout *m_hBtnLayout;
};
