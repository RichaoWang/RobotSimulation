#pragma once

#include "FluWidget.h"
#include <QGridLayout>
#include <QLabel>
#include "./utils/FluUtils.h"
#include "FluCalendarItem.h"

class FluCalendarView;
class FluCalendarSelectMonthView : public FluWidget
{
  public:
    FluCalendarSelectMonthView(QWidget* parent = nullptr);

    void setYearMonth(int nYear, int nMonth);

    void updateStyleSheet();

    QDate getPreYear();

    QDate getNextYear();

    void gotoPreYear();

    void gotoNextYear();

    FluCalendarItem* getItem(int nIndex);

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluCalendarSelectMonthView.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluCalendarSelectMonthView.qss", this);
        }
    }

  protected:
    QGridLayout* m_gMainLayout;
    QList<FluCalendarItem*> m_labelList;
    // QDate m_curYear;
    FluCalendarView* m_parentView;
};
