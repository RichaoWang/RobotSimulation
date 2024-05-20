#pragma once

#include "./utils/FluUtils.h"
#include <QSpinBox>

class FluSpinBox : public QSpinBox
{
    Q_OBJECT
  public:
    FluSpinBox(QWidget* parent = nullptr) : QSpinBox(parent)
    {
        onThemeChanged();
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluSpinBox.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluSpinBox.qss", this);
        }
    }
};
