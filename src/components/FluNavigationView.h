#pragma once

#include "./utils/FluIconUtils.h"
#include "./utils/FluLogUtils.h"
#include "./utils/FluUtils.h"
#include "./utils/FluStyleSheetUitls.h"
#include "FluDef.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStyleOption>
#include <QTransform>
#include <QVBoxLayout>
#include "FluWidget.h"
#include "FluVScrollView.h"

class FluNavigationItem;
class FluNavigationIconTextItem;
class FluNavigationView : public FluWidget
{
    Q_OBJECT
  public:
    FluNavigationView(QWidget *parent = nullptr);

    void addItemToTopLayout(QWidget *item);

    void addItemToMidLayout(QWidget *item);

    void addItemToBottomLayout(QWidget *item);

    void clearAllItemsSelectState();
    void updateAllItemsStyleSheet();

    void setLong(bool b)
    {
        m_bLong = b;
    }

    bool isLong()
    {
        return m_bLong;
    }

    std::vector<FluNavigationItem *> getAllItems();

    FluNavigationItem *getItemByKey(QString key);

    void paintEvent(QPaintEvent *event) override{};

  public slots:
    void onMenuItemClicked(){

  };
    void onThemeChanged()
    {
        //LOG_DEBUG << "called";
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluNavigationView.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluNavigationView.qss", this);
        }
    }

  public:
    QVBoxLayout *m_vLayout;

    QWidget *m_topWrapWidget;
    FluVScrollView *m_midVScrollView;
    QWidget *m_bottomWrapWidget;

    QVBoxLayout *m_vTopWrapLayout;
    QVBoxLayout *m_vBottomLayout;

    bool m_bLong;
};