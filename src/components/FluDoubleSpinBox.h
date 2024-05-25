#pragma once

#include <QDoubleSpinBox>
#include "./utils/FluUtils.h"

class FluDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
  public:
    FluDoubleSpinBox(QWidget* parent = nullptr) : QDoubleSpinBox(parent)
    {
        setRange(-99999.f,99999.f);
        onThemeChanged();
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluDoubleSpinBox.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluDoubleSpinBox.qss", this);
        }
    }
};
