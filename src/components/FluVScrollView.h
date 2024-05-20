#pragma once

#include "FluWidget.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include "./utils/FluStyleSheetUitls.h"
#include "./utils/FluUtils.h"

class FluVScrollView : public QScrollArea
{
    Q_OBJECT
  public:
    FluVScrollView(QWidget* parent = nullptr) : QScrollArea(parent)
    {
        setWidgetResizable(true);
        setMinimumSize(0, 0);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_contextWidget = new QWidget(this);
        setWidget(m_contextWidget);
        m_vMainLayout = new QVBoxLayout(m_contextWidget);
        m_contextWidget->setObjectName("contextWidget");

        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVScrollView.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    ~FluVScrollView()
    {
    }

    QVBoxLayout* getMainLayout()
    {
        return m_vMainLayout;
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVScrollView.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluVScrollView.qss", this);
        }
    }

  protected:
    QWidget* m_contextWidget;
    QVBoxLayout* m_vMainLayout;
};
