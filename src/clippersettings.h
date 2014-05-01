#ifndef CLIPPERSETTINGS_H
#define CLIPPERSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QDir>

class ClipperSettings : public QObject
{
    Q_OBJECT

public:
    QMap<QString, QString> shortcuts;
    QMap<QString, bool> general;
    QString screenshotPath;
    QString historyFilePath;

    void saveSettings();
    void loadSettings();

    static ClipperSettings* instance()
    {
        static ClipperSettings *instance = new ClipperSettings();
        return instance;
    }

signals:
    void settingsChanged();

private:
    QSettings *settings;
    ClipperSettings(QObject *parent = 0);
    ClipperSettings(const ClipperSettings& root);
    ClipperSettings& operator=(const ClipperSettings&);
};

#endif // CLIPPERSETTINGS_H
