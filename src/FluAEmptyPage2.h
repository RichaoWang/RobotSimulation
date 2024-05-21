#pragma once

#include "./components/FluWidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include "./components//FluSearchLineEdit.h"

class FluVScrollView;
class FluAEmptyPage2 : public FluWidget
{
    Q_OBJECT
  public:
    FluAEmptyPage2(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event);

  protected:
    QLabel* m_titleLabel;
    //  QLabel* m_subTitleLabel;

    QVBoxLayout* m_mainLayout;
    FluVScrollView* m_vScrollView;
    QLabel* m_infoLabel;
};
