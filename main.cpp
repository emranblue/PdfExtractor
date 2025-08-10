#include <QApplication>
#include <QIcon> // Added for application icon
#include <QDir>  // Add this line
#include <QSettings>
#include <QFont>
#include <QPalette>
#include <QStyleFactory>
#include "pdfextractiondialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    QSettings settings("MyCompany", "PdfExtractor");
    int fontSize = settings.value("appFontSize", 12).toInt();
    
    QFont font = app.font();
    font.setPointSize(fontSize);
    app.setFont(font);

    app.setWindowIcon(QIcon(":/icons/app_icon.png")); // Set application icon
    QDir::setCurrent("/media/emran/sdcard/bcs");
    PdfExtractionDialog dialog;
    dialog.exec();
    return 0;
}
