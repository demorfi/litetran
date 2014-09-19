#pragma once

#include <QSystemTrayIcon>
#include <QAction>

class QMenu;
class QAction;

#ifdef WITH_UNITY
#undef signals
extern "C" {
  #include <libappindicator/app-indicator.h>
  #include <gtk/gtk.h>
  void actionHandleIndicator(GtkMenu *, gpointer);
  void showIndicator(GtkMenu *, gpointer);
}
#define signals public
#endif

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = 0);
    ~TrayIcon();
    void addAction(QAction *act);
    void addSeparator();
signals:
    void clicked();
public slots:
    void onActivate(QSystemTrayIcon::ActivationReason reason);
private:
    QMenu *menu;
    QSystemTrayIcon *qt_icon;
#ifdef WITH_UNITY
    AppIndicator *indicator;
    GtkWidget *gtk_menu;
#endif
    bool isUnity;
};
