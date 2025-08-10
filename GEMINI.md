# Project: PDF Extractor

This is a C++ Qt application for extracting pages from PDF files.

## Features

*   **Extract pages by range:** (e.g., "1-5", "1,3,5").
*   **Extract pages by keyword search:**
    *   Supports multiple keywords separated by commas.
    *   Can match "ANY" (OR logic) or "ALL" (AND logic) keywords.
    *   Allows setting a minimum threshold for keyword matches.
    *   Supports both "Whole Word" and "Substring" search modes.
*   **Extract pages by date search:**
    *   Supports single or multiple dates separated by commas (e.g., "04-21-2025", "MAY 20, 2025").
    *   Parses various date formats (MM-DD-YYYY, Month Day, Year).
    *   Defaults year to 2025 if not provided.
    *   Searches for dates in "MONTH DAY, YEAR" format (e.g., "APRIL 21, 2025").
*   **Compress the output PDF.**
*   **Add watermark to the output PDF:** Overlays text as a semi-transparent watermark.
*   **GUI for selecting input/output files and options.**
*   **Default Input PDF:** Automatically selects `/media/emran/sdcard/bcs/total.pdf` as the default input file.

## Dependencies

*   Qt6
*   `pdftk` (for PDF manipulation)
*   `poppler-utils` (`pdftotext` for text extraction from PDFs)

## Workflow and Implementation Details

The application uses `pdftk` and `pdftotext` command-line tools to perform PDF operations. Commands are built based on user input from the GUI and executed using `system()`.

### Core Logic Prioritization

The application prioritizes extraction methods in the following order:
1.  **Date Search:** If the "Date" field is populated, it takes precedence.
2.  **Keyword Search:** If the "Date" field is empty but the "Keywords" field is populated, keyword search is performed.
3.  **Page Range Extraction:** If both "Date" and "Keywords" fields are empty, page range extraction is used.

### Text Extraction (`pdftotext`)

*   `pdftotext` is used to extract text content from each page of the input PDF.
*   The output of `pdftotext` for each page is written to a temporary file (`/tmp/pdftotext_temp.txt`) and then read back into the application. This approach was chosen to ensure compatibility with the `system()` command execution in the Qt environment.

### PDF Manipulation (`pdftk`)

*   `pdftk` is used for concatenating pages, compressing the output, and applying watermarks.
*   The `pdftk` command is constructed dynamically based on the selected options.
*   **Compression:** The `compress` keyword is added to the `pdftk` command after the `output` file specification.
*   **Watermarking:**
    *   A temporary PDF containing the watermark text is generated using Qt's `QPdfWriter` and `QPainter`. This PDF is semi-transparent and rotated.
    *   The `pdftk` command uses the `stamp` operation to overlay this temporary watermark PDF onto the extracted pages. The command structure is `pdftk A=input.pdf B=watermark.pdf cat A output output.pdf stamp B`.
*   Error messages from `pdftk` are redirected to a temporary file (`/tmp/pdftk_error.txt`) for display to the user.

### Date Parsing

*   The `parseDateStrings` function handles various input date formats (e.g., "MM-DD-YYYY", "Month Day, Year", "Day Month Year").
*   It supports multiple dates separated by commas.
*   If the year is not provided, it defaults to 2025.
*   All parsed dates are converted to "MONTH DAY, YEAR" (e.g., "APRIL 21, 2025") in uppercase for consistent searching within the PDF content.

### Keyword Search Logic

*   **Input:** Keywords are entered as a comma-separated list.
*   **Matching:**
    *   **"Match ANY keyword (OR)"**: If selected, a page is extracted if it contains at least one of the provided keywords.
    *   **"Match ALL keywords (AND)"**: If selected, a page is extracted only if it contains all of the provided keywords.
*   **Threshold:** A minimum number of keyword matches can be specified. If the total count of all keyword occurrences on a page is below this threshold, the page is not extracted (applies only to keyword search, not date search).
*   **Search Mode:**
    *   **"Whole Word"**: Uses regular expressions (`keyword`) to ensure only complete words are matched.
    *   **"Substring"**: Uses simple string containment (`QString::contains()`) to match any occurrence of the keyword.

### Output Filename Generation

The output filename is dynamically generated based on the extraction method and the original input PDF's name:
*   **Keyword:** `[input_filename]_keyword_[sanitized_keyword].pdf`
*   **Date:** `[input_filename]_date_[first_input_date].pdf`
*   **Page Range:** `[input_filename]_page_no_[sanitized_range].pdf`
*   **Default (if no specific method):** `[input_filename]_extracted.pdf`

Sanitization replaces non-alphanumeric characters with underscores.

## Build Instructions

The project can be built using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

The executable will be located at `build/PdfExtractor`.

## Known Issues/Limitations

*   Debug output from `pdftotext` and `pdftk` is redirected to temporary files or the terminal, making direct capture within the Qt application challenging.
*   The `system()` command is used for external tool execution, which might have environment-specific behaviors.
*   The watermarking feature currently applies a single, rotated, semi-transparent text string. More advanced watermarking options (e.g., image watermarks, custom positions, multiple watermarks) are not implemented.

## Version Control

This project is now under Git version control.

## Error Codes

This section defines numerical error codes for various scenarios within the PDF Extractor application. These codes can be used for internal error handling and external reporting.

*   **1000:** Success (No Error)
*   **2001:** `pdftk` Command Execution Failed
*   **2002:** `pdftotext` Command Execution Failed
*   **3001:** Input PDF File Not Found/Inaccessible
*   **3002:** Output PDF File Write Error (Permissions/Disk Full)
*   **3003:** Temporary File I/O Error
*   **4001:** Invalid Page Range Format
*   **4002:** Invalid Keyword Input
*   **4003:** Invalid Date Input Format
*   **5001:** No Pages Extracted (Criteria Mismatch)
*   **5002:** Watermark PDF Creation Failed