#pragma once

#include "resource.h"
// ScrollingBitmapExample.h
#pragma once
#include <windows.h>

// Declare bitmap functions
void InitBitmap(HWND hwnd);
void UpdateBitmapData();
void PaintBitmap(HWND hwnd, HDC hdc);

// Declare global variables as extern to share across files
extern HBITMAP hBitmap;
extern RECT dirtyRect;
