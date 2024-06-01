#include <QWidget>
#include "components/fluwidgets/FluShortInfoBar.h"
#include "components/fluwidgets/FluInfoBarMgr.h"

namespace InfoBar {
    static void showInfoBar(FluShortInfoBarType type, QString text, QWidget *p, int w = 270) {
        auto infobar = new FluShortInfoBar(type, text, p->window());
        infobar->setFixedWidth(w);
        FluInfoBarMgr::getInstance()->addInfoBar(p->window(), infobar);
    }
}


