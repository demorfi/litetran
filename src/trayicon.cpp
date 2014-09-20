#include "trayicon.h"
#include "defines.h"
#include <QMenu>
#include <QAction>

TrayIcon::TrayIcon(QObject *parent) :
    menu(new QMenu())
{
#ifdef WITH_UNITY
    QString desktop = getenv("XDG_CURRENT_DESKTOP");
    isUnity = (desktop.toLower() == "unity");
    if(isUnity) {
        qt_icon = NULL;
        GtkWidget *item;
        gtk_menu = gtk_menu_new();
        item = gtk_menu_item_new_with_label(tr("Show/Hide").toStdString().c_str());
        gtk_menu_shell_append(GTK_MENU_SHELL(gtk_menu), item);
        g_signal_connect(item, "activate",
                         G_CALLBACK(showIndicator), this);
        gtk_widget_show(item);

        indicator = app_indicator_new(
                    "litetran",
                    "litetran",
                    APP_INDICATOR_CATEGORY_APPLICATION_STATUS
                    );

        app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
        app_indicator_set_menu(indicator, GTK_MENU(gtk_menu));
    } else {
#else
    isUnity = false;
    {
#endif
        qt_icon = new QSystemTrayIcon(parent);
        qt_icon->setContextMenu(menu);
        qt_icon->setIcon(APP_ICON("litetran"));
        connect(qt_icon, &QSystemTrayIcon::activated, this, &TrayIcon::onActivate);
    }
}

TrayIcon::~TrayIcon()
{
    delete menu;
    if(qt_icon != NULL) {
        delete qt_icon;
    }
}

void TrayIcon::addAction(QAction *act) {
    menu->addAction(act);
#ifdef WITH_UNITY
    if(isUnity) {
        GtkWidget *item;
        item = gtk_menu_item_new_with_label(act->text().toStdString().c_str());
        gtk_menu_shell_append(GTK_MENU_SHELL(gtk_menu), item);
        g_signal_connect(item, "activate",
                         G_CALLBACK(actionHandleIndicator), act);
        gtk_widget_show(item);
    }
#endif
}

void TrayIcon::addSeparator() {
    menu->addSeparator();
#ifdef WITH_UNITY
    if(isUnity) {
        GtkWidget *item;
        item = gtk_separator_menu_item_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(gtk_menu), item);
        gtk_widget_show(item);
    }
#endif
}

void TrayIcon::setToolTip(QString tooltip)
{
    if(isUnity) {
#ifdef WITH_UNITY
        //TODO: set indicator tooltip
#endif
    } else {
        qt_icon->setToolTip(tooltip);
    }
}

void TrayIcon::setVisible(bool visible)
{
    if(isUnity) {
#ifdef WITH_UNITY
        //TODO: set indicator visibility
#endif
    } else {
        qt_icon->setVisible(visible);
    }
}

void TrayIcon::onActivate(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger)
        emit clicked();
}

#ifdef WITH_UNITY
void actionHandleIndicator(GtkMenu *menu, gpointer data) {
    Q_UNUSED(menu);
    QAction *action = static_cast<QAction *>(data);
    action->trigger();
}

void showIndicator(GtkMenu *menu, gpointer data) {
    Q_UNUSED(menu);
    TrayIcon *tray = static_cast<TrayIcon *>(data);
    tray->onActivate(QSystemTrayIcon::Trigger);
}
#endif
