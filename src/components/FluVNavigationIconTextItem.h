#pragma once

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
#include "FluVNavigationItem.h"
#include "./utils/FluUtils.h"
#include <stack>

class FluVNavigationView;

class FluVNavigationIconTextItem : public FluVNavigationItem {
Q_OBJECT
public:
    FluVNavigationIconTextItem(QWidget *parent = nullptr);

    FluVNavigationIconTextItem(QString iconPath, QString text, QWidget *parent = nullptr);

    FluVNavigationIconTextItem(FluAwesomeType awesomeType, QString text, QWidget *parent = nullptr);

    FluVNavigationIconTextItem(QString text, QWidget *parent = nullptr);

    FluVNavigationIconTextItem(FluVNavigationIconTextItem *item);

    ~FluVNavigationIconTextItem() {
        // LOG_DEBUG << "called";
    }

    void copyItem(FluVNavigationIconTextItem *item);

    QWidget *getWrapWidget1() {
        return m_wrapWidget1;
    }

    QWidget *getWrapWidget2() {
        return m_wrapWidget2;
    }

    QPushButton *getIconBtn() {
        return m_iconBtn;
    }

    QLabel *getLabel() {
        return m_label;
    }

    FluAwesomeType getAwesomeType() {
        return m_awesomeType;
    }

    bool getHideIcon() {
        return m_bHideIcon;
    }

    void hideLabelArrow() {
        m_label->hide();
        m_arrow->hide();
    }

    void showLabelArrow() {
        m_label->show();
        if (!m_items.empty())
            m_arrow->show();
    }

    std::vector<FluVNavigationIconTextItem *> getChildItems();

    void getAllItems(std::vector<FluVNavigationIconTextItem *> &totalItems) {
        std::vector<FluVNavigationIconTextItem *> childItems = getChildItems();
        for (auto childItem: childItems) {
            totalItems.push_back(childItem);
            childItem->getAllItems(totalItems);
        }
    }

    void addItem(FluVNavigationIconTextItem *item);

    int calcItemW1Width();

    int calcItemW2Height(FluVNavigationIconTextItem *item);

    void adjustItemHeight(FluVNavigationIconTextItem *item);

    int getDepth();

    FluVNavigationIconTextItem *getRootItem();

    void clearAllItemsSelectState() {
        FluVNavigationIconTextItem *rootItem = getRootItem();
        clearItemsSelectState(rootItem);
    }

    void clearItemsSelectState(FluVNavigationIconTextItem *item);

    void updateAllItemsStyleSheet() {
        FluVNavigationIconTextItem *rootItem = getRootItem();
        updateItemsStyleSheet(rootItem);
    }

    void updateItemsStyleSheet(FluVNavigationIconTextItem *item);

    void updateSelected(bool b);

    void enableThisItem(bool b);

    bool isDown() {
        return m_bDown;
    }

    bool isLeaf() {
        return m_items.empty();
    }

    void mouseReleaseEvent(QMouseEvent *event) override;

    // to enable qss
    void paintEvent(QPaintEvent *event) {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }

signals:

    void itemClicked();

public slots:

    void onItemClicked();

    void onItemClickedDirect() {
        std::stack<FluVNavigationIconTextItem *> itemStack;
        FluVNavigationIconTextItem *item = this;
        itemStack.push(item);
        while (item->m_parentItem != nullptr) {
            item = item->m_parentItem;
            itemStack.push(item);
        }

        while (!itemStack.empty()) {
            auto item = itemStack.top();
            if ((item->m_bDown && !item->getChildItems().empty()) || item->getChildItems().empty())
                item->onItemClicked();
            itemStack.pop();
        }
    }

    QString extractPart(QString input) {
        QStringList parts = input.split(QRegExp("[^a-zA-Z0-9]+|\\."), Qt::SplitBehavior::enum_type::SkipEmptyParts);
        for (const QString &part: parts) {
            if (!part.isEmpty() && part != "img") { // 排除 "img" 部分
                return part;
            }
        }
        return QString();
    }

    void onThemeChanged() {
        // LOG_DEBUG << "called";
        // todo 只考虑 icon Path 条件
        auto sp = extractPart(m_iconPath);
        if (sp.isEmpty()) {
            return;
        }

        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
            m_iconBtn->setIcon(QIcon(QString(":/img/%1-light.png").arg(sp)));
            m_arrow->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::ChevronDown, QColor(8, 8, 8)));
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVNavigationIconTextItem.qss", this);
        } else {
            m_iconBtn->setIcon(QIcon(QString(":/img/%1-dark.png").arg(sp)));
            m_arrow->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::ChevronDown, QColor(239, 239, 239)));
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluVNavigationIconTextItem.qss", this);
        }
    }

protected:
    QWidget *m_wrapWidget1;
    QWidget *m_wrapWidget2;

//    QWidget *m_emptyWidget;

    QWidget *m_indicator;
    QPushButton *m_iconBtn;
    QLabel *m_label;
    QPushButton *m_arrow;
    QVBoxLayout *m_hLayout1;
    std::vector<FluVNavigationIconTextItem *> m_items;
    QString m_iconPath;
    QString m_text;
    QVBoxLayout *m_vMainLayout;
    QVBoxLayout *m_vLayout1;

    FluVNavigationIconTextItem *m_parentItem;

    FluAwesomeType m_awesomeType;  // the icon display which

    bool m_bHideIcon;
    bool m_bDown;
    bool m_bLong;
    bool m_bSelected;
    bool m_bEnableThisItem;
};
