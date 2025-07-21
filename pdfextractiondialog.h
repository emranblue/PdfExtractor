#ifndef PDFEXTRACTIONDIALOG_H
#define PDFEXTRACTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

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

    QWidget* createFileSelectionRow(QPushButton* button, QWidget* display);
};

#endif // PDFEXTRACTIONDIALOG_H
