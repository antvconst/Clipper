#ifndef CLIPPER_SETTINGS_H
#define CLIPPER_SETTINGS_H

#include "clippersettings.h"

ClipperSettings::ClipperSettings(QObject *parent) :
    QObject(parent)
{
    QString settingsFilePath = QDir::homePath()+"/.clipper";
#ifdef Q_OS_LINUX
    settings = new QSettings(settingsFilePath, QSettings::NativeFormat);
#endif
#ifdef Q_OS_WIN
    settings = new QSettings(settingsFilePath, QSettings::IniFormat);
#endif

    this->loadSettings();
}

void ClipperSettings::saveSettings()
{
    settings->beginGroup("Hotkeys");
    settings->setValue("ShortenLink", shortcuts["ShortenLink"]);
    settings->setValue("PastePublish", shortcuts["PastePublish"]);
    settings->setValue("Screenshot", shortcuts["Screenshot"]);
    settings->setValue("PartialScreenshot", shortcuts["PartialScreenshot"]);
    settings->setValue("QRCode", shortcuts["QRCode"]);
    settings->setValue("MainWindow", shortcuts["MainWindow"]);
    settings->endGroup();

    settings->beginGroup("General");
    settings->setValue("LinkShortening", general["LinkShortening"]);
    settings->setValue("PastePublishing", general["PastePublishing"]);
    settings->setValue("ScreenshotMaking", general["ScreenshotMaking"]);
    settings->setValue("PartialScreenshotMaking", general["PartialScreenshotMaking"]);
    settings->setValue("QRCodeMaking", general["QRCodeMaking"]);
    settings->setValue("KeepHistory", general["KeepHistory"]);
    settings->setValue("SaveScreenshots", general["SaveScreenshots"]);
    settings->setValue("EnableMainWindowShortcut", general["EnableMainWindowShortcut"]);
    settings->setValue("ScreenshotDir", this->screenshotPath);
    settings->setValue("HistoryFilePath", this->historyFilePath);
    settings->endGroup();

    emit settingsChanged();
}

void ClipperSettings::loadSettings()
{
    shortcuts["ShortenLink"] = settings->value("Hotkeys/ShortenLink", "Shift+F6").toString();
    shortcuts["PastePublish"] = settings->value("Hotkeys/PastePublish", "Shift+F7").toString();
    shortcuts["Screenshot"] = settings->value("Hotkeys/Screenshot", "Shift+F10").toString();
    shortcuts["PartialScreenshot"] = settings->value("Hotkeys/PartialScreenshot", "Shift+F11").toString();
    shortcuts["QRCode"] = settings->value("Hotkeys/QRCode", "Shift+F9").toString();
    shortcuts["Multicopy"] = settings->value("Hotkeys/Multicopy", "Ctrl+Shift+C").toString();
    shortcuts["MainWindow"] = settings->value("Hotkeys/MainWindow", "Alt+Shift+F12").toString();

    general["LinkShortening"] = settings->value("General/LinkShortening", "1").toBool();
    general["PastePublishing"] = settings->value("General/PastePublishing", "1").toBool();
    general["ScreenshotMaking"] = settings->value("General/ScreenshotMaking", "1").toBool();
    general["PartialScreenshotMaking"] = settings->value("General/PartialScreenshotMaking", "1").toBool();
    general["QRCodeMaking"] = settings->value("General/QRCodeMaking", "1").toBool();
    general["KeepHistory"] = settings->value("General/KeepHistory", "1").toBool();
    general["SaveScreenshots"] = settings->value("General/SaveScreenshots", "1").toBool();
    general["EnableMainWindowShortcut"] = settings->value("General/EnableMainWindowShortcut", "1").toBool();
    screenshotPath = settings->value("General/ScreenshotDir", QDir::homePath()+"/Screenshots").toString();
    historyFilePath = settings->value("General/HistoryFilePath", QDir::homePath()+"/ClipboardHistory.txt").toString();
}

#endif
