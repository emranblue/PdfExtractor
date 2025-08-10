#ifndef PDFEXTRACTIONDIALOG_H
#define PDFEXTRACTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextEdit> // New include for QTextEdit
#include <QPushButton> // New include for QPushButton
#include <QMenuBar> // New include for QMenuBar
#include <QAction> // New include for QAction
#include "errorcodes.h" // Include the new error codes header

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
    void processPdf(); // This slot will now call a function that returns PdfExtractorError
    void showPageNumbers(); // New slot for showing page numbers only
    void copyPageNumbersToClipboard(); // New slot for clipboard functionality
    void showAboutDialog(); // New slot for displaying the About dialog
    void openSettingsDialog(); // New slot for opening the settings dialog

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
    QTextEdit *pageNumbersDisplay; // New QTextEdit for displaying page numbers
    QPushButton *copyPageNumbersButton; // New button for copying page numbers

    QWidget* createFileSelectionRow(QPushButton* button, QWidget* display);
    QStringList parseDateStrings(const QString &dateString);
    QString generateOutputFilename(ExtractionMethod method);
    PdfExtractorError createWatermarkPdf(const QString &watermarkText, const QString &outputPdfPath);
    PdfExtractorError executePdfExtractionLogic(); // New function to encapsulate logic and return error
    QPair<QStringList, PdfExtractorError> findMatchingPageNumbers(ExtractionMethod method, const QStringList &keywordList); // New function to find matching page numbers
    void displayError(PdfExtractorError errorCode); // New function to display error messages
};

#endif // PDFEXTRACTIONDIALOG_H