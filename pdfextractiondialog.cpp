#include "pdfextractiondialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QDebug>
#include <iostream>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter> // Needed for drawing text on the PDF
#include <QRegularExpression>
#include <cstdlib> // For system()
#include <QFile>   // For reading error output
#include <QTextStream>

PdfExtractionDialog::PdfExtractionDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("PDF Extraction Tool");
    setFixedSize(500, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Input group
    QGroupBox *inputGroup = new QGroupBox("Input Parameters", this);
    QFormLayout *inputLayout = new QFormLayout(inputGroup);

    // PDF file selection
    QPushButton *selectFileButton = new QPushButton("Browse...", this);
    pdfPathLabel = new QLabel("No file selected", this);
    inputLayout->addRow("PDF File:", createFileSelectionRow(selectFileButton, pdfPathLabel));

    // Page range input
    pageRangeEdit = new QLineEdit(this);
    pageRangeEdit->setPlaceholderText("e.g. 1-5, 1,3,5, or 1-end");
    inputLayout->addRow("Page Range:", pageRangeEdit);

    // Output filename
    QPushButton *outputBrowseButton = new QPushButton("Browse...", this);
    outputEdit = new QLineEdit(this);
    outputEdit->setPlaceholderText("output_extracted.pdf");
    inputLayout->addRow("Output File:", createFileSelectionRow(outputBrowseButton, outputEdit));

    // Options group
    QGroupBox *optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    compressCheck = new QCheckBox("Compress output PDF", this);
    optionsLayout->addWidget(compressCheck);

    watermarkCheck = new QCheckBox("Add watermark", this);
    watermarkText = new QLineEdit(this);
    watermarkText->setPlaceholderText("Watermark text");
    watermarkText->setEnabled(false);

    QHBoxLayout *watermarkLayout = new QHBoxLayout();
    watermarkLayout->addWidget(watermarkCheck);
    watermarkLayout->addWidget(watermarkText);
    optionsLayout->addLayout(watermarkLayout);

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(optionsGroup);

    QPushButton *processButton = new QPushButton("Extract Pages", this);
    processButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");
    mainLayout->addWidget(processButton);

    connect(selectFileButton, &QPushButton::clicked, this, &PdfExtractionDialog::selectPdfFile);
    connect(outputBrowseButton, &QPushButton::clicked, this, &PdfExtractionDialog::selectOutputFile);
    connect(processButton, &QPushButton::clicked, this, &PdfExtractionDialog::processPdf);
    connect(watermarkCheck, &QCheckBox::toggled, watermarkText, &QLineEdit::setEnabled);

    setLayout(mainLayout);
}

QWidget* PdfExtractionDialog::createFileSelectionRow(QPushButton* button, QWidget* display) {
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(button);
    layout->addWidget(display);
    return container;
}

void PdfExtractionDialog::selectPdfFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select a PDF file",
        QDir::currentPath(),
        "PDF Files (*.pdf)");

    if (!fileName.isEmpty()) {
        pdfPath = fileName;
        pdfPathLabel->setText(QFileInfo(fileName).fileName());

        QString defaultOutput = QFileInfo(fileName).path() + "/" +
                               QFileInfo(fileName).completeBaseName() + "_extracted.pdf";
        outputEdit->setText(QDir::toNativeSeparators(defaultOutput));
    }
}

void PdfExtractionDialog::selectOutputFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save extracted PDF",
        outputEdit->text().isEmpty() ? QDir::currentPath() : outputEdit->text(),
        "PDF Files (*.pdf)");

    if (!fileName.isEmpty()) {
        outputEdit->setText(QDir::toNativeSeparators(fileName));
    }
}

#include <cstdlib> // For system()
#include <QFile>   // For reading error output
#include <QTextStream>

void PdfExtractionDialog::processPdf() {
    if (pdfPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a PDF file first.");
        return;
    }

    QString pageRange = pageRangeEdit->text().trimmed();
    if (pageRange.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a page range.");
        return;
    }

    // Validate pageRange format (e.g., 1-5, 250,687,1000, 250-687, 1-end)
    QRegularExpression pageRangePattern("^(?:[0-9]+(?:-(?:[0-9]+|end))?)(?:,[0-9]+(?:-(?:[0-9]+|end))?)*$");
    if (!pageRangePattern.match(pageRange).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid page range format. Use formats like 1-5, 250,687,1000, 250-687, or 1-end.");
        return;
    }

    // Replace commas with spaces for pdftk compatibility
    QString pdftkPageRange = pageRange;
    pdftkPageRange.replace(",", " ");

    QString outputFile = outputEdit->text().trimmed();
    if (outputFile.isEmpty()) {
        outputFile = QFileInfo(pdfPath).path() + "/" +
                     QFileInfo(pdfPath).completeBaseName() + "_extracted.pdf";
        outputEdit->setText(outputFile);
    }

    // Escape special characters to prevent command injection
    QString escapedPdfPath = QString(pdfPath).replace("\"", "\\\"").replace(" ", "\\ ");
    QString escapedOutputFile = QString(outputFile).replace("\"", "\\\"").replace(" ", "\\ ");
    QString escapedPageRange = QString(pdftkPageRange).replace("\"", "\\\"");

    // Check if pdftk is installed
    if (system("pdftk --version > /dev/null 2>&1") != 0) {
        QMessageBox::critical(this, "Error",
            "pdftk is not installed. Please install it first:\nsudo apt install pdftk");
        return;
    }

    // Construct pdftk command
    QString errorFile = QDir::tempPath() + "/pdftk_error.txt";
    QString command = QString("pdftk \"%1\" cat %2 output \"%3\" 2> \"%4\"")
                     .arg(escapedPdfPath, escapedPageRange, escapedOutputFile, errorFile);

    if (compressCheck->isChecked()) {
        command += " compress"; // Note: Verify pdftk syntax for compression
    }

    if (watermarkCheck->isChecked() && !watermarkText->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Watermarking requires a PDF file, not text. Feature not implemented.");
        return;
        // To implement watermarking, generate a PDF with QPdfWriter
    }

    qDebug() << "Executing pdftk command:" << command;

    // Execute command
    int exitCode = system(command.toStdString().c_str());

    if (exitCode == 0) {
        QMessageBox::information(this, "Success",
            QString("PDF successfully extracted to:\n%1").arg(QDir::toNativeSeparators(outputFile)));
    } else {
        // Read error output from temporary file
        QString errorMessage;
        QFile errorFileHandle(errorFile);
        if (errorFileHandle.open(QIODevice::ReadOnly)) {
            QTextStream in(&errorFileHandle);
            errorMessage = in.readAll();
            errorFileHandle.close();
            errorFileHandle.remove(); // Clean up
        } else {
            errorMessage = "Failed to read pdftk error output.";
        }
        QMessageBox::critical(this, "Error",
            QString("pdftk failed with error:\n%1").arg(errorMessage));
    }
}
