#pragma once

#include <QSlider>

#include "./utils/FluUtils.h"

class FluSlider : public QSlider
{
    Q_OBJECT
  public:
    FluSlider(QWidget* parent = nullptr) : QSlider(parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluSlider.qss", this);
    }

    FluSlider(Qt::Orientation orientation, QWidget* parent = nullptr) : QSlider(orientation, parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluSlider.qss", this);
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluSlider.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluSlider.qss", this);
        }
    }
};
