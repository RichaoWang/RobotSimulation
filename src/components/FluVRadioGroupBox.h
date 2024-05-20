#pragma once

#include <QButtonGroup>
#include <QGroupBox>
#include "./utils/FluUtils.h"
#include <QVBoxLayout>
#include "FluRadioButton.h"
class FluVRadioGroupBox : public QGroupBox
{
    Q_OBJECT
  public:
    FluVRadioGroupBox(QWidget* parent = nullptr) : QGroupBox(parent)
    {
        m_vMainLayout = new QVBoxLayout;
        // m_vMainLayout->setContentsMargins(0,0,0,0);

        setLayout(m_vMainLayout);
        m_vMainLayout->addSpacing(15);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVRadioGroupBox.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    FluVRadioGroupBox(QString title, QWidget* parent = nullptr) : QGroupBox(title, parent)
    {
        m_vMainLayout = new QVBoxLayout;
        // m_vMainLayout->setContentsMargins(5, 5, 5, 5);
        // m_vMainLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(m_vMainLayout);
        m_vMainLayout->addSpacing(15);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVRadioGroupBox.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

    void addRadioButton(FluRadioButton* radioButton)
    {
        m_vMainLayout->addWidget(radioButton);
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVRadioGroupBox.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluVRadioGroupBox.qss", this);
        }
    }

  protected:
    QVBoxLayout* m_vMainLayout;
};
