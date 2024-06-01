#pragma once

#include <QWidget>
#include "components/fluwidgets/utils/FluThemeUtils.h"

class FluWidget : public QWidget
{
    Q_OBJECT
  public:
    FluWidget(QWidget* parent = nullptr);

    virtual ~FluWidget()
    {
    }

    void showEvent(QShowEvent* event);
  public slots:
    virtual void onThemeChanged()
    {
    }

  protected:
    FluTheme m_theme;
};
