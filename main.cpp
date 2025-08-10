#include <QApplication>
#include <QIcon> // Added for application icon
#include <QDir>  // Add this line
#include <QSettings>
#include <QFont>
#include <QPalette>
#include "pdfextractiondialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    QSettings settings("MyCompany", "PdfExtractor");
    int fontSize = settings.value("appFontSize", 12).toInt();
    QColor themeColor = settings.value("themeColor", QColor(Qt::white)).value<QColor>();

    QFont font = app.font();
    font.setPointSize(fontSize);
    app.setFont(font);

    QPalette palette = app.palette();
    palette.setColor(QPalette::Window, themeColor);
    app.setPalette(palette);

    app.setWindowIcon(QIcon(":/icons/app_icon.png")); // Set application icon
    QDir::setCurrent("/media/emran/sdcard/bcs");
    PdfExtractionDialog dialog;
    dialog.exec();
    return 0;
}
