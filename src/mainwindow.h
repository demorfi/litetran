#pragma once

#include "languagedb.h"
#include <QMainWindow>

class QTextEdit;
class QTextBrowser;
class QComboBox;
class QPushButton;
class QToolButton;
class QAction;
class QCloseEvent;
class QSettings;
class QTranslator;
class QShortcut;
class QxtGlobalShortcut;
class TextToolbar;
class Settings;
class TrayIcon;
class Translate;
class Popup;
class LanguageDB;
class Clipboard;


#define DEFAULT_SOURCE_LANGUAGE "English"
#define DEFAULT_RESULT_LANGUAGE "Russian"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void about();
    void quit();
    void swap();
    void translate();
    void changeVisibility();
    void updateSettings();
    void languageChanged();
private:
    void changeEvent(QEvent *e);
    bool applicationInFocus();

    QString sourceLanguage() const;
    QString resultLanguage() const;
    QString sourceText() const;
    QString resultText() const;

    QString last_locale;
    QString about_text;
    QString about_title;

    QTextEdit *source_text;
    QTextBrowser *result_text;
    QComboBox *source_combobox;
    QComboBox *result_combobox;

    QPushButton *translate_button;
    QToolButton *swap_button;

    QAction *action_swap;
    QAction *action_settings;
    QAction *action_detect;
    QAction *action_about;
    QAction *action_exit;
    QToolButton *menu_button;
    QMenu *menu_root;
    QSettings *settings;
    QTranslator *ui_translator;
    QShortcut *translate_shortcut;
    QxtGlobalShortcut *translate_shortcut_global;

    Clipboard *clipboard;
    Settings *settings_dialog;
    TrayIcon *tray_icon;
    Translate *translate_engine;
    Popup *popup;
    LanguageDB *langdb;
protected:
    void closeEvent(QCloseEvent *event);
};
