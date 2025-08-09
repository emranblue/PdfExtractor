#ifndef PDFEXTRACTIONDIALOG_H
#define PDFEXTRACTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>

enum class ExtractionMethod {
    None,
    Date,
    Keyword,
    PageRange
};

class PdfExtractionDialog : public QDialog {
    Q_OBJECT

public:
    explicit PdfExtractionDialog(QWidget *parent = nullptr);

private slots:
    void selectPdfFile();
    void selectOutputFile();
    void processPdf();

private:
    QString pdfPath;
    QLabel *pdfPathLabel;
    QLineEdit *pageRangeEdit;
    QLineEdit *outputEdit;
    QCheckBox *compressCheck;
    QCheckBox *watermarkCheck;
    QLineEdit *watermarkText;
    QLineEdit *keywordEdit;
    QLineEdit *thresholdEdit;
    QRadioButton *matchAllKeywordsRadio;
    QRadioButton *matchAnyKeywordRadio;
    QRadioButton *wholeWordRadio;
    QRadioButton *substringRadio;
    QLineEdit *dateEdit;

    QWidget* createFileSelectionRow(QPushButton* button, QWidget* display);
    QStringList parseDateStrings(const QString &dateString);
    QString generateOutputFilename(ExtractionMethod method);
    bool createWatermarkPdf(const QString &watermarkText, const QString &outputPdfPath);
};

#endif // PDFEXTRACTIONDIALOG_H