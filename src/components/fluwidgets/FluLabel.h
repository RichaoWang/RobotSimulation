#pragma once

#include <QLabel>

#include "components/fluwidgets/utils/FluStyleSheetUitls.h"
#include "components/fluwidgets/utils/FluUtils.h"

enum class FluLabelStyle
{
    CaptionTextBlockSylte,
    BodyTextBlockStyle,
    BodyStrongTextBlockStyle,
    SubTitleTextBlockStyle,
    TitleTextBlockStyle,
    TitleLargeTextBlockStyle,
    DisplayTextBlockStyle,
};

class FluLabel : public QLabel
{
    Q_OBJECT
  public:
    FluLabel(QWidget* parent = nullptr) : QLabel(parent)
    {
        m_style = FluLabelStyle::CaptionTextBlockSylte;
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluLabel.qss", this);
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Dark)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluLabel.qss", this);
        }
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    FluLabel(FluLabelStyle style, QWidget* parent = nullptr) : QLabel(parent), m_style(style)
    {
        // reference WinUi3 Gallary Typeography page.
        // setProperty("style", m_style);
        setLabelStyle(style);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluLabel.qss", this);
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Dark)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluLabel.qss", this);
        }
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    void setLabelStyle(FluLabelStyle style)
    {
        m_style = style;
        QString styleString;
        switch (style)
        {
            case FluLabelStyle::CaptionTextBlockSylte:
                styleString = "CaptionTextBlockSylte";
                break;
            case FluLabelStyle::BodyTextBlockStyle:
                styleString = "BodyTextBlockStyle";
                break;
            case FluLabelStyle::BodyStrongTextBlockStyle:
                styleString = "BodyStrongTextBlockStyle";

                break;
            case FluLabelStyle::SubTitleTextBlockStyle:
                styleString = "SubTitleTextBlockStyle";

                break;
            case FluLabelStyle::TitleTextBlockStyle:
                styleString = "TitleTextBlockStyle";

                break;
            case FluLabelStyle::TitleLargeTextBlockStyle:
                styleString = "TitleLargeTextBlockStyle";

                break;
            case FluLabelStyle::DisplayTextBlockStyle:
                styleString = "DisplayTextBlockStyle";
                break;
            default:
                break;
        }
        setProperty("labelStyle", styleString);
    }
  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluLabel.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluLabel.qss", this);
        }
    }

  protected:
    FluLabelStyle m_style;
};
