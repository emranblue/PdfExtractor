#include "pdfextractiondialog.h"
#include <QDate>
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
    setFixedSize(600, 450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Input group
    QGroupBox *inputGroup = new QGroupBox("Input Parameters", this);
    QFormLayout *inputLayout = new QFormLayout(inputGroup);

    // PDF file selection
    QPushButton *selectFileButton = new QPushButton("Browse...", this);
    pdfPathLabel = new QLabel("No file selected", this);
    pdfPath = "/media/emran/sdcard/bcs/total.pdf";
    pdfPathLabel->setText(QFileInfo(pdfPath).fileName());
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

    // Keyword input
    keywordEdit = new QLineEdit(this);
    keywordEdit->setPlaceholderText("Enter keywords, separated by commas");
    inputLayout->addRow("Keywords:", keywordEdit);

    // Keyword search options
    matchAnyKeywordRadio = new QRadioButton("Match ANY keyword (OR)", this);
    matchAllKeywordsRadio = new QRadioButton("Match ALL keywords (AND)", this);
    matchAnyKeywordRadio->setChecked(true);
    QHBoxLayout *keywordRadioLayout = new QHBoxLayout();
    keywordRadioLayout->addWidget(matchAnyKeywordRadio);
    keywordRadioLayout->addWidget(matchAllKeywordsRadio);
    inputLayout->addRow("Keyword Logic:", keywordRadioLayout);

    // Threshold input
    thresholdEdit = new QLineEdit(this);
    thresholdEdit->setPlaceholderText("e.g. 3");
    inputLayout->addRow("Minimum Keyword Matches:", thresholdEdit);

    // Keyword search mode
    wholeWordRadio = new QRadioButton("Whole Word", this);
    substringRadio = new QRadioButton("Substring", this);
    substringRadio->setChecked(true);
    QHBoxLayout *searchModeLayout = new QHBoxLayout();
    searchModeLayout->addWidget(wholeWordRadio);
    searchModeLayout->addWidget(substringRadio);
    inputLayout->addRow("Search Mode:", searchModeLayout);

    // Date input
    dateEdit = new QLineEdit(this);
    dateEdit->setPlaceholderText("e.g., MAY 20, 2025 or 5-20-2025");
    inputLayout->addRow("Date:", dateEdit);

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

#include <QDate>

QStringList PdfExtractionDialog::parseDateStrings(const QString &dateString) {
    QStringList formattedDates;
    QStringList dates = dateString.split(',');

    for (const QString &dateStr : dates) {
        QString trimmedDate = dateStr.trimmed();
        if (trimmedDate.isEmpty()) {
            continue;
        }

        QDate date;
        QStringList parts = trimmedDate.split(QRegularExpression("[-,\\s/]+"));
        int day = 0, month = 0, year = 0;

        if (parts.size() == 2) { // Month and Day
            bool dayOk, monthOk;
            month = parts[0].toInt(&monthOk);
            day = parts[1].toInt(&dayOk);
            if (!monthOk) { // Try month name
                month = QDate::fromString(parts[0], "MMM").month();
            }
            if (!dayOk) {
                 day = parts[1].toInt(&dayOk);
            }
            year = 2025; // Default year
        } else if (parts.size() == 3) { // Month, Day, Year
            bool dayOk, monthOk, yearOk;
            month = parts[0].toInt(&monthOk);
            day = parts[1].toInt(&dayOk);
            year = parts[2].toInt(&yearOk);
            if (!monthOk) { // Try month name
                month = QDate::fromString(parts[0], "MMM").month();
            }
            if (!dayOk) {
                 day = parts[1].toInt(&dayOk);
            }
            if (!yearOk) {
                year = parts[2].toInt(&yearOk);
            }
        } else {
            continue; // Invalid format
        }

        if (QDate::isValid(year, month, day)) {
            date.setDate(year, month, day);
            formattedDates.append(date.toString("MMMM d, yyyy").toUpper());
        }
    }

    return formattedDates;
}

QString PdfExtractionDialog::generateOutputFilename(ExtractionMethod method) {
    QFileInfo fileInfo(pdfPath);
    QString basePath = fileInfo.path() + "/" + fileInfo.completeBaseName();

    switch (method) {
        case ExtractionMethod::Date:
            {
                QStringList dates = dateEdit->text().trimmed().split(',');
                return basePath + "_date_" + dates[0].trimmed() + ".pdf";
            }
        case ExtractionMethod::Keyword:
            {
                QString sanitizedKeyword = keywordEdit->text().trimmed();
                sanitizedKeyword.replace(QRegularExpression("[^a-zA-Z0-9_]+"), "_");
                return basePath + "_keyword_" + sanitizedKeyword + ".pdf";
            }
        case ExtractionMethod::PageRange:
            {
                QString sanitizedRange = pageRangeEdit->text().trimmed();
                sanitizedRange.replace(QRegularExpression("[^a-zA-Z0-9_]+"), "_");
                return basePath + "_page_no_" + sanitizedRange + ".pdf";
            }
        default:
            return basePath + "_extracted.pdf";
    }
}

bool PdfExtractionDialog::createWatermarkPdf(const QString &watermarkText, const QString &outputPdfPath) {
    QPdfWriter pdfWriter(outputPdfPath);
    if (!pdfWriter.setPageSize(QPageSize(QPageSize::A4))) {
        qDebug() << "Failed to set page size.";
        return false;
    }

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        qDebug() << "Failed to activate painter.";
        return false;
    }

    // Set font and color for the watermark
    QFont font = painter.font();
    font.setPointSize(72); // Large font size
    painter.setFont(font);
    QColor textColor = Qt::red;
    textColor.setAlpha(50); // Semi-transparent
    painter.setPen(textColor);

    // Rotate and position the text
    painter.translate(pdfWriter.width() / 2, pdfWriter.height() / 2);
    painter.rotate(-45); // Rotate by -45 degrees

    // Draw the text
    painter.drawText(-pdfWriter.width() / 4, -pdfWriter.height() / 4, watermarkText);

    painter.end();
    return true;
}

void PdfExtractionDialog::processPdf() {
    qDebug() << "PDF Path:" << pdfPath;
    qDebug() << "Page Range Edit:" << pageRangeEdit->text();
    qDebug() << "Keyword Edit:" << keywordEdit->text();
    qDebug() << "Date Edit:" << dateEdit->text();

    if (pdfPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a PDF file first.");
        return;
    }

    QString pageRange = pageRangeEdit->text().trimmed();
    QString keywords = keywordEdit->text().trimmed(); // Now multiple keywords
    QString date = dateEdit->text().trimmed();

    ExtractionMethod method = ExtractionMethod::None;
    if (!date.isEmpty()) {
        method = ExtractionMethod::Date;
    } else if (!keywords.isEmpty()) {
        method = ExtractionMethod::Keyword;
    } else if (!pageRange.isEmpty()) {
        method = ExtractionMethod::PageRange;
    }

    if (method == ExtractionMethod::None) {
        QMessageBox::warning(this, "Error", "Please enter a page range, keywords, or a date.");
        return;
    }

    QStringList pagesToExtract;
    QStringList keywordList;

    if (method == ExtractionMethod::Date) {
        keywordList = parseDateStrings(date);
        if (keywordList.isEmpty()) {
            QMessageBox::warning(this, "Error", "Invalid date format. Please use MM-DD-YYYY or similar, separated by commas.");
            return;
        }
    } else if (method == ExtractionMethod::Keyword) {
        keywordList = keywords.split(',', Qt::SkipEmptyParts);
        for(auto& kw : keywordList) {
            kw = kw.trimmed();
        }
    } else if (method == ExtractionMethod::PageRange) {
        QRegularExpression pageRangePattern("^(?:[0-9]+(?:-(?:[0-9]+|end))?)(?:,[0-9]+(?:-(?:[0-9]+|end))?)*$");
        if (!pageRangePattern.match(pageRange).hasMatch()) {
            QMessageBox::warning(this, "Error", "Invalid page range format. Use formats like 1-5, 250,687,1000, 250-687, or 1-end.");
            return;
        }
        pagesToExtract = pageRange.split(',', Qt::SkipEmptyParts);
    }

    if (method == ExtractionMethod::Date || method == ExtractionMethod::Keyword) {
        if (system("pdftotext -v > /dev/null 2>&1") != 0) {
            QMessageBox::critical(this, "Error",
                "pdftotext is not installed. Please install it first:\nsudo apt install poppler-utils");
            return;
        }

        qDebug() << "Parsed Keywords:" << keywordList;

        int numPages = 0;

        // Get the number of pages
        QProcess countProcess;
        countProcess.start("pdftk", QStringList() << pdfPath << "dump_data");
        countProcess.waitForFinished();
        QString countOutput = countProcess.readAllStandardOutput();
        QRegularExpression re("NumberOfPages: (\\d+)");
        QRegularExpressionMatch match = re.match(countOutput);
        if (match.hasMatch()) {
            numPages = match.captured(1).toInt();
        }
        qDebug() << "Number of Pages:" << numPages;

        for (int i = 1; i <= numPages; ++i) {
            QString tempTextFile = QDir::tempPath() + "/pdftotext_temp.txt";
            QString command = QString("pdftotext -f %1 -l %1 \"%2\" \"%3\"").arg(i).arg(pdfPath).arg(tempTextFile);
            qDebug() << "Executing pdftotext command:" << command;
            int ret = system(command.toStdString().c_str());

            QString pageText;
            if (ret == 0) {
                QFile file(tempTextFile);
                if (file.open(QIODevice::ReadOnly)) {
                    pageText = file.readAll();
                    file.close();
                    file.remove();
                }
            }
            qDebug() << "Page" << i << "Text:\n" << pageText; // Print full page text

            bool pageMatches = false;
            if (method == ExtractionMethod::Date || matchAnyKeywordRadio->isChecked()) {
                // Match ANY keyword (OR logic) or Date (always OR logic)
                for (const QString& key : keywordList) {
                    if (substringRadio->isChecked()) {
                        if (pageText.contains(key, Qt::CaseInsensitive)) {
                            pageMatches = true;
                            break;
                        }
                    } else { // Whole word search
                        QRegularExpression re("\\b" + QRegularExpression::escape(key) + "\\b", QRegularExpression::CaseInsensitiveOption);
                        if (pageText.contains(re)) {
                            pageMatches = true;
                            break;
                        }
                    }
                }
            } else if (matchAllKeywordsRadio->isChecked()) {
                // Match ALL keywords (AND logic)
                bool allMatch = true;
                for (const QString& key : keywordList) {
                    if (substringRadio->isChecked()) {
                        if (!pageText.contains(key, Qt::CaseInsensitive)) {
                            allMatch = false;
                            break;
                        }
                    } else { // Whole word search
                        QRegularExpression re("\\b" + QRegularExpression::escape(key) + "\\b", QRegularExpression::CaseInsensitiveOption);
                        if (!pageText.contains(re)) {
                            allMatch = false;
                            break;
                        }
                    }
                }
                pageMatches = allMatch;
            } else { // Default to match ANY if no radio button is selected (shouldn't happen with UI)
                for (const QString& key : keywordList) {
                    if (substringRadio->isChecked()) {
                        if (pageText.contains(key, Qt::CaseInsensitive)) {
                            pageMatches = true;
                            break;
                        }
                    }
                    else { // Whole word search
                        QRegularExpression re("\\b" + QRegularExpression::escape(key) + "\\b", QRegularExpression::CaseInsensitiveOption);
                        if (pageText.contains(re)) {
                            pageMatches = true;
                            break;
                        }
                    }
                }
            }

            // Apply threshold for keyword search only (not for date search)
            if (method == ExtractionMethod::Keyword && thresholdEdit->text().toInt() > 0) {
                int totalMatchCount = 0;
                for (const QString& key : keywordList) {
                    totalMatchCount += pageText.count(key, Qt::CaseInsensitive);
                }
                if (totalMatchCount < thresholdEdit->text().toInt()) {
                    pageMatches = false; // Override if threshold not met
                }
            }

            if(pageMatches) {
                pagesToExtract << QString::number(i);
            }
        }

        if (pagesToExtract.isEmpty()) {
            QMessageBox::information(this, "No Match", "No pages were found containing the specified keyword(s).");
            return;
        }
    }

    QString outputFile = generateOutputFilename(method);
    outputEdit->setText(outputFile);

    QString pdftkPageRange = pagesToExtract.join(" ");
    QString escapedPdfPath = QString(pdfPath).replace("\"", "\\\"").replace(" ", "\\ ");
    QString escapedOutputFile = QString(outputFile).replace("\"", "\\\"").replace(" ", "\\ ");
    QString escapedPageRange = QString(pdftkPageRange).replace("\"", "\\\"");

    if (system("pdftk --version > /dev/null 2>&1") != 0) {
        QMessageBox::critical(this, "Error",
            "pdftk is not installed. Please install it first:\nsudo apt install pdftk");
        return;
    }

    QString command = QString("pdftk A=\"%1\" cat A%2 output \"%3\"")
                     .arg(escapedPdfPath, escapedPageRange, escapedOutputFile);

    if (compressCheck->isChecked()) {
        command += " compress";
    }

    QString watermarkPdfPath;
    if (watermarkCheck->isChecked() && !watermarkText->text().isEmpty()) {
        watermarkPdfPath = QDir::tempPath() + "/watermark_temp.pdf";
        if (!createWatermarkPdf(watermarkText->text(), watermarkPdfPath)) {
            QMessageBox::critical(this, "Error", "Failed to create watermark PDF.");
            return;
        }
        command = QString("pdftk A=\"%1\" B=\"%2\" cat A%3 output \"%4\" stamp B").arg(escapedPdfPath, watermarkPdfPath, escapedPageRange, escapedOutputFile);
        if (compressCheck->isChecked()) {
            command += " compress";
        }
    }

    QString errorFile = QDir::tempPath() + "/pdftk_error.txt";
    command += QString(" 2> \"%1\"").arg(errorFile);

    qDebug() << "Executing pdftk command:" << command;

    // Execute command
    int exitCode = system(command.toStdString().c_str());

    // Clean up temporary watermark PDF
    if (!watermarkPdfPath.isEmpty()) {
        QFile::remove(watermarkPdfPath);
    }

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
