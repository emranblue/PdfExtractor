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
#include <QDir>  // Add this line
#include "pdfextractiondialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    QDir::setCurrent("/media/emran/sdcard/bcs");
    PdfExtractionDialog dialog;
    dialog.exec();
    return 0;
}
