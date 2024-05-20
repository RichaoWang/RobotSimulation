#pragma once

#include <QRadioButton>
#include "./utils/FluUtils.h"
#include <QProxyStyle>
#include <QPainter>
#include <QStyleOptionButton>

class FluRadioButton : public QRadioButton
{
    Q_OBJECT
  public:
    explicit FluRadioButton(QWidget* parent = nullptr) : QRadioButton(parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluRadioButton.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    explicit FluRadioButton(const QString& text, QWidget* parent = nullptr) : QRadioButton(text, parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluRadioButton.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluRadioButton.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluRadioButton.qss", this);
        }
    }
};
