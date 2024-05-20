#pragma once

#include <QPushButton>
#include "./utils/FluUtils.h"

class FluRepeatButton : public QPushButton
{
    Q_OBJECT
  public:
    FluRepeatButton(QWidget* parent = nullptr);

    void setClickCount(int nCount);

    int getClickCount();

  signals:
    void clickedCountChanged(int nClickCount);

  public slots:
    void onThemeChanged();

  private:
    int m_nClickedCount;
};
