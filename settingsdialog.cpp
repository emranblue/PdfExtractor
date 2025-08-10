#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSettings>
#include <QDebug>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Settings");
    setFixedSize(400, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *defaultValuesGroup = new QGroupBox("Default Values", this);
    QFormLayout *formLayout = new QFormLayout(defaultValuesGroup);

    defaultPdfPathEdit = new QLineEdit(this);
    formLayout->addRow("Default PDF Path:", defaultPdfPathEdit);

    defaultPageRangeEdit = new QLineEdit(this);
    formLayout->addRow("Default Page Range:", defaultPageRangeEdit);

    defaultOutputDirectoryEdit = new QLineEdit(this);
    formLayout->addRow("Default Output Directory:", defaultOutputDirectoryEdit);

    defaultKeywordsEdit = new QLineEdit(this);
    formLayout->addRow("Default Keywords:", defaultKeywordsEdit);

    defaultThresholdEdit = new QLineEdit(this);
    formLayout->addRow("Default Threshold:", defaultThresholdEdit);

    defaultMatchAnyKeywordsRadio = new QRadioButton("Match ANY keyword (OR)", this);
    defaultMatchAllKeywordsRadio = new QRadioButton("Match ALL keywords (AND)", this);
    QHBoxLayout *keywordRadioLayout = new QHBoxLayout();
    keywordRadioLayout->addWidget(defaultMatchAnyKeywordsRadio);
    keywordRadioLayout->addWidget(defaultMatchAllKeywordsRadio);
    formLayout->addRow("Keyword Logic:", keywordRadioLayout);

    defaultWholeWordRadio = new QRadioButton("Whole Word", this);
    defaultSubstringRadio = new QRadioButton("Substring", this);
    QHBoxLayout *searchModeLayout = new QHBoxLayout();
    searchModeLayout->addWidget(defaultWholeWordRadio);
    searchModeLayout->addWidget(defaultSubstringRadio);
    formLayout->addRow("Search Mode:", searchModeLayout);

    defaultDateEdit = new QLineEdit(this);
    formLayout->addRow("Default Date:", defaultDateEdit);

    defaultCompressPdfCheck = new QCheckBox("Compress PDF", this);
    formLayout->addRow("Compress PDF:", defaultCompressPdfCheck);

    defaultAddWatermarkCheck = new QCheckBox("Add Watermark", this);
    defaultWatermarkTextEdit = new QLineEdit(this);
    defaultWatermarkTextEdit->setPlaceholderText("Watermark text");
    defaultWatermarkTextEdit->setEnabled(false);
    QHBoxLayout *watermarkLayout = new QHBoxLayout();
    watermarkLayout->addWidget(defaultAddWatermarkCheck);
    watermarkLayout->addWidget(defaultWatermarkTextEdit);
    formLayout->addRow("Watermark:", watermarkLayout);

    mainLayout->addWidget(defaultValuesGroup);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    connect(defaultAddWatermarkCheck, &QCheckBox::toggled, this, &SettingsDialog::toggleWatermarkText);

    loadSettings(); // Load settings when dialog is initialized
}

void SettingsDialog::saveSettings() {
    QSettings settings("MyCompany", "PdfExtractor"); // Use your company/app name
    settings.beginGroup("DefaultSettings");
    settings.setValue("pdfPath", defaultPdfPathEdit->text());
    settings.setValue("pageRange", defaultPageRangeEdit->text());
    settings.setValue("outputDirectory", defaultOutputDirectoryEdit->text());
    settings.setValue("keywords", defaultKeywordsEdit->text());
    settings.setValue("threshold", defaultThresholdEdit->text().toInt());
    settings.setValue("matchAllKeywords", defaultMatchAllKeywordsRadio->isChecked());
    settings.setValue("wholeWordSearch", defaultWholeWordRadio->isChecked());
    settings.setValue("date", defaultDateEdit->text());
    settings.setValue("compressPdf", defaultCompressPdfCheck->isChecked());
    settings.setValue("addWatermark", defaultAddWatermarkCheck->isChecked());
    settings.setValue("watermarkText", defaultWatermarkTextEdit->text());
    settings.endGroup();
    qDebug() << "Settings saved.";
}

void SettingsDialog::loadSettings() {
    QSettings settings("MyCompany", "PdfExtractor");
    settings.beginGroup("DefaultSettings");
    defaultPdfPathEdit->setText(settings.value("pdfPath", "").toString());
    defaultPageRangeEdit->setText(settings.value("pageRange", "").toString());
    defaultOutputDirectoryEdit->setText(settings.value("outputDirectory", "").toString());
    defaultKeywordsEdit->setText(settings.value("keywords", "").toString());
    defaultThresholdEdit->setText(QString::number(settings.value("threshold", 0).toInt()));
    defaultMatchAllKeywordsRadio->setChecked(settings.value("matchAllKeywords", false).toBool());
    defaultMatchAnyKeywordsRadio->setChecked(!defaultMatchAllKeywordsRadio->isChecked()); // Set the other radio button
    defaultWholeWordRadio->setChecked(settings.value("wholeWordSearch", false).toBool());
    defaultSubstringRadio->setChecked(!defaultWholeWordRadio->isChecked()); // Set the other radio button
    defaultDateEdit->setText(settings.value("date", "").toString());
    defaultCompressPdfCheck->setChecked(settings.value("compressPdf", false).toBool());
    defaultAddWatermarkCheck->setChecked(settings.value("addWatermark", false).toBool());
    defaultWatermarkTextEdit->setText(settings.value("watermarkText", "").toString());
    settings.endGroup();
    qDebug() << "Settings loaded.";

    // Ensure watermark text edit is enabled/disabled correctly on load
    toggleWatermarkText(defaultAddWatermarkCheck->isChecked());
}

void SettingsDialog::toggleWatermarkText(bool checked) {
    defaultWatermarkTextEdit->setEnabled(checked);
}

// Getters
QString SettingsDialog::defaultPdfPath() const { return defaultPdfPathEdit->text(); }
QString SettingsDialog::defaultPageRange() const { return defaultPageRangeEdit->text(); }
QString SettingsDialog::defaultOutputDirectory() const { return defaultOutputDirectoryEdit->text(); }
QString SettingsDialog::defaultKeywords() const { return defaultKeywordsEdit->text(); }
int SettingsDialog::defaultThreshold() const { return defaultThresholdEdit->text().toInt(); }
bool SettingsDialog::defaultMatchAllKeywords() const { return defaultMatchAllKeywordsRadio->isChecked(); }
bool SettingsDialog::defaultWholeWordSearch() const { return defaultWholeWordRadio->isChecked(); }
QString SettingsDialog::defaultDate() const { return defaultDateEdit->text(); }
bool SettingsDialog::defaultCompressPdf() const { return defaultCompressPdfCheck->isChecked(); }
bool SettingsDialog::defaultAddWatermark() const { return defaultAddWatermarkCheck->isChecked(); }
QString SettingsDialog::defaultWatermarkText() const { return defaultWatermarkTextEdit->text(); }

// Setters
void SettingsDialog::setDefaultPdfPath(const QString &path) { defaultPdfPathEdit->setText(path); }
void SettingsDialog::setDefaultPageRange(const QString &range) { defaultPageRangeEdit->setText(range); }
void SettingsDialog::setDefaultOutputDirectory(const QString &dir) { defaultOutputDirectoryEdit->setText(dir); }
void SettingsDialog::setDefaultKeywords(const QString &keywords) { defaultKeywordsEdit->setText(keywords); }
void SettingsDialog::setDefaultThreshold(int threshold) { defaultThresholdEdit->setText(QString::number(threshold)); }
void SettingsDialog::setDefaultMatchAllKeywords(bool checked) { defaultMatchAllKeywordsRadio->setChecked(checked); defaultMatchAnyKeywordsRadio->setChecked(!checked); }
void SettingsDialog::setDefaultWholeWordSearch(bool checked) { defaultWholeWordRadio->setChecked(checked); defaultSubstringRadio->setChecked(!checked); }
void SettingsDialog::setDefaultDate(const QString &date) { defaultDateEdit->setText(date); }
void SettingsDialog::setDefaultCompressPdf(bool checked) { defaultCompressPdfCheck->setChecked(checked); }
void SettingsDialog::setDefaultAddWatermark(bool checked) { defaultAddWatermarkCheck->setChecked(checked); }
void SettingsDialog::setDefaultWatermarkText(const QString &text) { defaultWatermarkTextEdit->setText(text); }