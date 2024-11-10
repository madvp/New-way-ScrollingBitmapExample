// main.cpp
#include <windows.h>
#include "ScrollingBitmapExample.h"

HBITMAP hBitmap = NULL;
void* pBits = NULL;
const int WIDTH = 256;
const int HEIGHT = 300;
RECT dirtyRect = { 0, 0, WIDTH, HEIGHT };

// Initialize DIB section
void InitBitmap(HWND hwnd) {
    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(bmpInfo));

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = WIDTH;
    bmpInfo.bmiHeader.biHeight = HEIGHT; // Top-down DIB
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(hwnd);
    hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    ReleaseDC(hwnd, hdc);

    if (!hBitmap) {
        MessageBox(hwnd, L"Failed to create DIB section!", L"Error", MB_OK);
    }
}

#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

void UpdateBitmapData() {
    if (pBits == NULL) return;

    BYTE* pixelData = (BYTE*)pBits;
    int rowSize = WIDTH * 3;

    // Move all rows up by one row
    memmove(pixelData, pixelData + rowSize, (HEIGHT - 1) * rowSize);

    // Add new row of random color data at the bottom
    BYTE* newRow = pixelData + (HEIGHT - 1) * rowSize;

    // Seed the random number generator once
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(0));
        seeded = true;
    }

    for (int x = 0; x < WIDTH; ++x) {
        newRow[x * 3 + 0] = rand() % 256; // Blue
        newRow[x * 3 + 1] = rand() % 256; // Green
        newRow[x * 3 + 2] = rand() % 256; // Red
    }

    // Set the modified area (the entire bitmap)
    dirtyRect.left = 0;
    dirtyRect.right = WIDTH;
    dirtyRect.top = 0;
    dirtyRect.bottom = HEIGHT;
}


// Draw using SetDIBitsToDevice
void PaintBitmap(HWND hwnd, HDC hdc) {
    // Declare and initialize bmpInfo
    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(bmpInfo));

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = WIDTH;
    bmpInfo.bmiHeader.biHeight = HEIGHT;  // Top-down DIB
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // Create a memory DC (off-screen buffer)
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    HBITMAP hOldBuffer = (HBITMAP)SelectObject(hdcMem, hbmBuffer);

    // Draw the bitmap in the memory DC (off-screen buffer)
    SetDIBitsToDevice(hdcMem, 0, 0, WIDTH, HEIGHT, 0, 0, 0, HEIGHT, pBits, &bmpInfo, DIB_RGB_COLORS);

    // Copy the memory DC to the screen
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);

    // Clean up
    SelectObject(hdcMem, hOldBuffer);
    DeleteObject(hbmBuffer);
    DeleteDC(hdcMem);
}


