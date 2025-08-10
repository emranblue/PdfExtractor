#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    // Getters for settings
    QString defaultPdfPath() const;
    QString defaultPageRange() const;
    QString defaultOutputDirectory() const;
    QString defaultKeywords() const;
    int defaultThreshold() const;
    bool defaultMatchAllKeywords() const;
    bool defaultWholeWordSearch() const;
    QString defaultDate() const;
    bool defaultCompressPdf() const;
    bool defaultAddWatermark() const;
    QString defaultWatermarkText() const;

    // Setters for settings (to populate dialog from current settings)
    void setDefaultPdfPath(const QString &path);
    void setDefaultPageRange(const QString &range);
    void setDefaultOutputDirectory(const QString &dir);
    void setDefaultKeywords(const QString &keywords);
    void setDefaultThreshold(int threshold);
    void setDefaultMatchAllKeywords(bool checked);
    void setDefaultWholeWordSearch(bool checked);
    void setDefaultDate(const QString &date);
    void setDefaultCompressPdf(bool checked);
    void setDefaultAddWatermark(bool checked);
    void setDefaultWatermarkText(const QString &text);

private slots:
    void saveSettings();
    void loadSettings(); // To load settings when dialog opens
    void toggleWatermarkText(bool checked);

private:
    QLineEdit *defaultPdfPathEdit;
    QLineEdit *defaultPageRangeEdit;
    QLineEdit *defaultOutputDirectoryEdit;
    QLineEdit *defaultKeywordsEdit;
    QLineEdit *defaultThresholdEdit;
    QRadioButton *defaultMatchAnyKeywordsRadio;
    QRadioButton *defaultMatchAllKeywordsRadio;
    QRadioButton *defaultWholeWordRadio;
    QRadioButton *defaultSubstringRadio;
    QLineEdit *defaultDateEdit;
    QCheckBox *defaultCompressPdfCheck;
    QCheckBox *defaultAddWatermarkCheck;
    QLineEdit *defaultWatermarkTextEdit;
};

#endif // SETTINGSDIALOG_H