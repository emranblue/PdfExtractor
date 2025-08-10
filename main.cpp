#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QDir>
#include <QApplication>
#include <QIcon> // Added for application icon
#include <QDir>  // Add this line
#include "pdfextractiondialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    app.setWindowIcon(QIcon(":/icons/app_icon.png")); // Set application icon
    QDir::setCurrent("/media/emran/sdcard/bcs");
    PdfExtractionDialog dialog;
    dialog.exec();
    return 0;
}
