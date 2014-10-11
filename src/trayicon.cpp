#include "trayicon.h"
#include "defines.h"
#include <QMenu>
#include <QAction>

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent),
    menu(new QMenu)
{
    setContextMenu(menu);
    setIcon(APP_ICON("litetran"));
    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivate);
}

TrayIcon::~TrayIcon()
{
    delete menu;
}

void TrayIcon::addAction(QAction *act) {
    menu->addAction(act);
}

void TrayIcon::addSeparator() {
    menu->addSeparator();
}

void TrayIcon::onActivate(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger)
        emit clicked();
}
