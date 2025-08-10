# PDF Extractor

This is a C++ Qt application designed to help users extract specific pages from PDF files based on various criteria. It's particularly useful for organizing study materials, such as those for the BCS exam, by allowing you to quickly pinpoint and extract relevant information.

## Features

*   **Extract pages by range:** Extract pages using specified ranges (e.g., "1-5", "1,3,5").
*   **Extract pages by keyword search:** Search for pages containing specific keywords.
    *   Supports multiple keywords (comma-separated).
    *   Flexible matching: "ANY" (OR logic) or "ALL" (AND logic) keywords.
    *   Minimum threshold for keyword occurrences.
    *   Search modes: "Whole Word" or "Substring".
*   **Extract pages by date search:** Find and extract pages based on dates found within the PDF content.
    *   Supports various date formats (e.g., "MM-DD-YYYY", "Month Day, Year").
    *   Automatically parses multiple dates.
*   **Display Matching Page Numbers:** A dedicated "Show Page Numbers" button allows users to find and display page numbers that match date or keyword criteria without extracting the PDF.
*   **Copy Page Numbers to Clipboard:** Easily copy the list of matching page numbers to your clipboard.
*   **Open PDF After Extraction:** After successfully extracting a PDF, a prompt will ask if you'd like to open the newly created file.
*   **Compress Output PDF:** Option to compress the size of the extracted PDF.
*   **Add Watermark:** Overlay custom text as a semi-transparent watermark on the output PDF.
*   **Intuitive GUI:** User-friendly interface for selecting input/output files and configuring extraction options.
*   **Customizable Default Settings:** A dedicated settings dialog allows you to set and save default values for various input fields, including:
    *   Default PDF path
    *   Default page range
    *   Default output directory (with a file selector)
    *   Default keywords and search options
    *   Default date
    *   Default compression and watermark settings
    These settings are persisted using `QSettings` and are loaded on application startup.
*   **Application Icon:** The application now features a custom icon, visible in the window title bar and as an executable icon on Linux desktop environments.
*   **About Dialog:** An "About" dialog, accessible via the "Help" menu, provides information about the application, its purpose (for BCS exam paper organization), version, dependencies, and developer credits.

## Dependencies

To run this application, you will need:

*   **Qt6:** A cross-platform application development framework.
*   **`pdftk`:** A command-line tool for PDF manipulation.
*   **`poppler-utils` (`pdftotext`):** Command-line utilities for PDF text extraction.

## Build Instructions

The project uses CMake for its build system. Follow these steps to build the application:

1.  **Create a build directory:**
    ```bash
    mkdir build
    ```
2.  **Navigate into the build directory:**
    ```bash
    cd build
    ```
3.  **Configure the project with CMake:**
    ```bash
    cmake ..
    ```
4.  **Build the application:**
    ```bash
    make
    ```

The executable will be located at `build/PdfExtractor`.

## How to Run

After building, you can run the application from the `build` directory:

```bash
./PdfExtractor
```

## Setting the Executable Icon (Linux)

For the application icon to appear on your Linux desktop environment (e.g., in your application launcher or file manager), you need to install the provided `.desktop` file.

1.  **Copy the `.desktop` file for the current user:**
    ```bash
    cp PdfExtractor.desktop ~/.local/share/applications/
    ```
    Then, update the desktop database (optional, but recommended):
    ```bash
    update-desktop-database ~/.local/share/applications/
    ```
2.  **Copy the `.desktop` file for all users (requires root privileges):**
    ```bash
    sudo cp PdfExtractor.desktop /usr/share/applications/
    ```
    Then, update the desktop database:
    ```bash
    sudo update-desktop-database
    ```

## About

**PDF Extractor**
Version 1.0

This project is all about BCS exam, to study paper more organized way.
This application allows you to extract pages from PDF files by page range, keywords, or dates.

Developed by: Gemini CLI Agent
Instructed by: Emran Hossain
Contact: emran.blue.120@gmail.com

&copy; 2025 All rights reserved.