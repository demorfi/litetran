#include "settings.h"
#include "defines.h"
#include "platform/autostart.h"
#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDir>
#include <QDebug>
#include <QSpacerItem>
#include <QMessageBox>
#include <QKeySequenceEdit>
#include <QGroupBox>

#define DEFAULT_LANGUAGE "English"
#define DEFAULT_TRANSLATE_SHORTCUT "Alt+T"
#define DEFAULT_APPEAR_SHORTCUT "Ctrl+Shift+T"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    settings(new QSettings(this)),
    tray_checkbox(new QCheckBox(this)),
    dictionary_checkbox(new QCheckBox(this)),
    autotranslate_checkbox(new QCheckBox(this)),
    run_at_startup_checkbox(new QCheckBox(this)),
    translate_shortcut_checkbox(new QCheckBox(this)),
    appear_shortcut_checkbox(new QCheckBox(this)),
    translate_shortcut_edit(new QKeySequenceEdit(this)),
    appear_shortcut_edit(new QKeySequenceEdit(this)),
    language_combobox(new QComboBox(this)),
    label_language(new QLabel(this)),
    groupbox_app(new QGroupBox(this)),
    groupbox_keyboard(new QGroupBox(this)),
    button_box(new QDialogButtonBox(this)),
    autostart_manager(new AutoStart(this))
{
    connect(translate_shortcut_checkbox, &QCheckBox::toggled, translate_shortcut_edit, &QKeySequenceEdit::setEnabled);
    connect(button_box, &QDialogButtonBox::accepted, this, &Settings::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &Settings::reject);

    QFormLayout *app_layout = new QFormLayout;
    app_layout->addRow(label_language, language_combobox);
    app_layout->addRow(tray_checkbox, new QWidget(this));
    app_layout->addRow(run_at_startup_checkbox, new QWidget(this));
    app_layout->addRow(dictionary_checkbox, new QWidget(this));
    app_layout->addRow(autotranslate_checkbox, new QWidget(this));

    QFormLayout *keyboard_layout = new QFormLayout;
    keyboard_layout->addRow(translate_shortcut_checkbox, translate_shortcut_edit);
    keyboard_layout->addRow(appear_shortcut_checkbox, appear_shortcut_edit);

    groupbox_app->setLayout(app_layout);
    groupbox_keyboard->setLayout(keyboard_layout);
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(groupbox_app);
    main_layout->addWidget(groupbox_keyboard);
    main_layout->addWidget(button_box);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(main_layout);

    button_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    // Initialize languages combobox.
    QDir qmdir(APP_I18N_DIR);
    const QStringList qmfiles = qmdir.entryList(QStringList() << "*.qm");

    foreach(QString qmfile, qmfiles) {
        qmfile = qmfile.split('.').first();
        language_combobox->addItem(qmfile);
    }

    language_combobox->adjustSize();
    settings->beginGroup("Settings");

    // Try to set language from locale, English is a fallback language.
    default_language = detectSystemLanguage();
    if(!qmfiles.contains(default_language + ".qm"))
        default_language = "English";

    read();
}

int Settings::exec() {
    read();
    return QDialog::exec();
}

bool Settings::translateShortcutEnabled()
{
    return translate_shortcut_checkbox->isChecked();
}

bool Settings::appearShortcutEnabled()
{
    return appear_shortcut_checkbox->isChecked();
}

bool Settings::trayIconEnabled()
{
    return tray_checkbox->isChecked();
}

bool Settings::dictionaryEnabled()
{
    return dictionary_checkbox->isChecked();
}

bool Settings::autoTranslate()
{
    return autotranslate_checkbox->isChecked();
}

bool Settings::runAtStartup()
{
    return run_at_startup_checkbox->isChecked();
}

QKeySequence Settings::translateShortcut() const
{
    return translate_shortcut_edit->keySequence();
}

QKeySequence Settings::appearShortcut() const
{
    return appear_shortcut_edit->keySequence();
}

QString Settings::language() const
{
    return language_combobox->currentText();
}

QString Settings::detectSystemLanguage() const
{
    return QLocale::languageToString(QLocale().language());
}

void Settings::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    if(e->type() == QEvent::LanguageChange) {
            msg_key_overlap = tr("Translate and reverse key sequences shouldn't be overlapped!");
            msg_key_overlap_title = tr("Invalid shortcuts");
            tray_checkbox->setText(tr("Show in tray"));
            translate_shortcut_checkbox->setText(tr("Translation"));
            appear_shortcut_checkbox->setText(tr("Show LiteTran window"));
            dictionary_checkbox->setText(tr("Show multiple translations"));
            autotranslate_checkbox->setText(tr("Auto translate text"));
            run_at_startup_checkbox->setText(tr("Add to Autostart"));
            label_language->setText(tr("Language"));
            setWindowTitle(tr("Settings"));
            groupbox_app->setTitle(tr("Application"));
            groupbox_keyboard->setTitle(tr("HotKeys"));
    }
}

void Settings::accept()
{
    if(hasShortcutIntersection()) {
        QMessageBox::warning(this, msg_key_overlap_title, msg_key_overlap);
        return;
    }

    settings->setValue("TranslateShortcutEnabled", translate_shortcut_checkbox->isChecked());
    settings->setValue("AppearShortcutEnabled", appear_shortcut_checkbox->isChecked());
    settings->setValue("TranslateShortcut", translate_shortcut_edit->keySequence().toString());
    settings->setValue("AppearShortcut", appear_shortcut_edit->keySequence().toString());
    settings->setValue("Language", language_combobox->currentText());
    settings->setValue("TrayIconEnabled", tray_checkbox->isChecked());
    settings->setValue("ShowDictionary", dictionary_checkbox->isChecked());
    settings->setValue("AutoTranslate", autotranslate_checkbox->isChecked());
    autostart_manager->setAutoStart(run_at_startup_checkbox->isChecked());

    QDialog::accept();
}

void Settings::read()
{
    language_combobox->setCurrentText(settings->value("Language", default_language).toString());
    translate_shortcut_checkbox->setChecked(settings->value("TranslateShortcutEnabled", true).toBool());
    appear_shortcut_checkbox->setChecked(settings->value("AppearShortcutEnabled", true).toBool());
    translate_shortcut_edit->setKeySequence(settings->value("TranslateShortcut", DEFAULT_TRANSLATE_SHORTCUT).toString());
    appear_shortcut_edit->setKeySequence(settings->value("AppearShortcut", DEFAULT_APPEAR_SHORTCUT).toString());
    tray_checkbox->setChecked(settings->value("TrayIconEnabled", true).toBool());
    dictionary_checkbox->setChecked(settings->value("ShowDictionary", true).toBool());
    autotranslate_checkbox->setChecked(settings->value("AutoTranslate", true).toBool());
    run_at_startup_checkbox->setChecked(autostart_manager->autoStart());
}

bool Settings::hasShortcutIntersection()
{
    QStringList shortcuts = QStringList()
            << translate_shortcut_edit->keySequence().toString()
            << appear_shortcut_edit->keySequence().toString();

    if (shortcuts.removeDuplicates() > 0)
        return true;
    return false;
}
