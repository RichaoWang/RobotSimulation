#pragma once

#include "FluWidget.h"
#include <QGridLayout>
#include <QLabel>
#include "components/fluwidgets/utils/FluUtils.h"
#include "FluCalendarItem.h"

class FluCalendarView;

class FluCalendarSelectYearView : public FluWidget
{
  public:
    FluCalendarSelectYearView(QWidget* parent = nullptr);

    FluCalendarItem* getItem(int nIndex)
    {
        if (nIndex < 0 || nIndex >= m_labelList.size())
            return nullptr;

        return m_labelList.at(nIndex);
    }

    void setYears(int nYear, int nMonth);

    void updateStyleSheet();

    void gotoPreYears();

    void gotoNextYears();

    void getRange(int nYear, int& nStartYear, int& nEndYear);

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluCalendarSelectYearView.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluCalendarSelectYearView.qss", this);
        }
    }

  protected:
    QGridLayout* m_gMainLayout;
    QList<FluCalendarItem*> m_labelList;
    FluCalendarView* m_parentView;
};
