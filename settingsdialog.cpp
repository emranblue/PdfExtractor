#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSettings>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileDialog> // Added for QFileDialog
#include <QColorDialog>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Settings");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *defaultValuesGroup = new QGroupBox("Default Values", this);
    QFormLayout *formLayout = new QFormLayout(defaultValuesGroup);

    defaultPdfPathEdit = new QLineEdit(this);
    formLayout->addRow("Default PDF Path:", defaultPdfPathEdit);

    defaultPageRangeEdit = new QLineEdit(this);
    formLayout->addRow("Default Page Range:", defaultPageRangeEdit);

    defaultOutputDirectoryEdit = new QLineEdit(this);
    defaultOutputDirectoryBrowseButton = new QPushButton("Browse...", this);
    QHBoxLayout *outputDirLayout = new QHBoxLayout();
    outputDirLayout->addWidget(defaultOutputDirectoryEdit);
    outputDirLayout->addWidget(defaultOutputDirectoryBrowseButton);
    formLayout->addRow("Default Output Directory:", outputDirLayout);

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
    defaultWatermarkColorButton = new QPushButton("Color", this);
    defaultWatermarkFontSizeSpinBox = new QSpinBox(this);
    defaultWatermarkFontSizeSpinBox->setRange(1, 200);

    QHBoxLayout *watermarkLayout = new QHBoxLayout();
    watermarkLayout->addWidget(defaultAddWatermarkCheck);
    watermarkLayout->addWidget(defaultWatermarkTextEdit);
    watermarkLayout->addWidget(defaultWatermarkColorButton);
    watermarkLayout->addWidget(defaultWatermarkFontSizeSpinBox);
    formLayout->addRow("Watermark:", watermarkLayout);

    appFontSizeSpinBox = new QSpinBox(this);
    appFontSizeSpinBox->setRange(6, 24);
    formLayout->addRow("Application Font Size:", appFontSizeSpinBox);

    themeColorButton = new QPushButton("Select Theme Color", this);
    formLayout->addRow("Theme Color:", themeColorButton);

    mainLayout->addWidget(defaultValuesGroup);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    connect(defaultAddWatermarkCheck, &QCheckBox::toggled, this, &SettingsDialog::toggleWatermarkText);
    connect(defaultWatermarkColorButton, &QPushButton::clicked, this, &SettingsDialog::selectWatermarkColor);
    connect(themeColorButton, &QPushButton::clicked, this, &SettingsDialog::selectThemeColor);
    connect(defaultOutputDirectoryBrowseButton, &QPushButton::clicked, this, &SettingsDialog::selectDefaultOutputDirectory);

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
    settings.setValue("watermarkColor", watermarkColor);
    settings.setValue("watermarkFontSize", defaultWatermarkFontSizeSpinBox->value());
    settings.setValue("appFontSize", appFontSizeSpinBox->value());
    settings.setValue("themeColor", m_themeColor);
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
    watermarkColor = settings.value("watermarkColor", QColor(Qt::red)).value<QColor>();
    defaultWatermarkFontSizeSpinBox->setValue(settings.value("watermarkFontSize", 72).toInt());
    appFontSizeSpinBox->setValue(settings.value("appFontSize", 12).toInt());
    m_themeColor = settings.value("themeColor", QColor(Qt::white)).value<QColor>();
    settings.endGroup();
    qDebug() << "Settings loaded.";

    // Ensure watermark text edit is enabled/disabled correctly on load
    toggleWatermarkText(defaultAddWatermarkCheck->isChecked());
}

void SettingsDialog::toggleWatermarkText(bool checked) {
    defaultWatermarkTextEdit->setEnabled(checked);
    defaultWatermarkColorButton->setEnabled(checked);
    defaultWatermarkFontSizeSpinBox->setEnabled(checked);
}

void SettingsDialog::selectDefaultOutputDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this, "Select Default Output Directory",
                                                        defaultOutputDirectoryEdit->text());
    if (!directory.isEmpty()) {
        defaultOutputDirectoryEdit->setText(directory);
    }
}

void SettingsDialog::selectWatermarkColor() {
    QColor color = QColorDialog::getColor(watermarkColor, this, "Select Watermark Color");
    if (color.isValid()) {
        watermarkColor = color;
    }
}

void SettingsDialog::selectThemeColor() {
    QColor color = QColorDialog::getColor(m_themeColor, this, "Select Theme Color");
    if (color.isValid()) {
        m_themeColor = color;
    }
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
QColor SettingsDialog::defaultWatermarkColor() const { return watermarkColor; }
int SettingsDialog::defaultWatermarkFontSize() const { return defaultWatermarkFontSizeSpinBox->value(); }
int SettingsDialog::appFontSize() const { return appFontSizeSpinBox->value(); }
QColor SettingsDialog::themeColor() const { return m_themeColor; }

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
void SettingsDialog::setDefaultWatermarkColor(const QColor &color) { watermarkColor = color; }
void SettingsDialog::setDefaultWatermarkFontSize(int size) { defaultWatermarkFontSizeSpinBox->setValue(size); }
void SettingsDialog::setAppFontSize(int size) { appFontSizeSpinBox->setValue(size); }
void SettingsDialog::setThemeColor(const QColor &color) { m_themeColor = color; }