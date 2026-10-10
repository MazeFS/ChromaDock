#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <commoncontrols.h>
#include <commdlg.h>
#include <uxtheme.h>
#include <mmsystem.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ole32.lib")

#pragma comment(linker, "/subsystem:windows /ENTRY:wWinMainCRTStartup")

using namespace Gdiplus;

static const CLSID CLSID_PNG = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

enum class Language { EN = 0, RU = 1 };

enum StringId {
    STR_TOGGLE_ALL,
    STR_TOPMOST,
    STR_AUTOSTART,
    STR_EXIT_PROG,
    STR_ADD_APP,
    STR_ADD_FOLDER,
    STR_DIRECTION,
    STR_DIR_DOWN,
    STR_DIR_UP,
    STR_DIR_RIGHT,
    STR_DIR_LEFT,
    STR_ICON_SIZE,
    STR_SIZE_UP,
    STR_SIZE_DOWN,
    STR_DOCK_SCALE,
    STR_SCALE_UP,
    STR_SCALE_DOWN,
    STR_SCALE_RESET,
    STR_ALWAYS_VIS,
    STR_NEW_DOCK,
    STR_CLOSE_THIS,
    STR_CLOSE_ALL,
    STR_DELETE_ITEM,
    STR_SELECT_FOLD,
    STR_LANG_MENU,
    STR_COUNT
};

struct LocalizedString {
    const wchar_t* en;
    const char* ru_u8;
};

static const LocalizedString g_strTable[STR_COUNT] = {
    { L"Expand / Collapse All Docks", "\xD0\xA0\xD0\xB0\xD1\x81\xD0\xBA\xD1\x80\xD1\x8B\xD1\x82\xD1\x8C / \xD0\xA1\xD0\xB2\xD0\xB5\xD1\x80\xD0\xBD\xD1\x83\xD1\x82\xD1\x8C \xD0\xB2\xD1\x81\xD0\xB5 \xD0\xB4\xD0\xBE\xD0\xBA\xD0\xB8" },
    { L"Always on Top",               "\xD0\x9F\xD0\xBE\xD0\xB2\xD0\xB5\xD1\x80\xD1\x85 \xD0\xB2\xD1\x81\xD0\xB5\xD1\x85 \xD0\xBE\xD0\xBA\xD0\xBE\xD0\xBD" },
    { L"Launch on Windows Startup",   "\xD0\x90\xD0\xB2\xD1\x82\xD0\xBE\xD0\xB7\xD0\xB0\xD0\xBF\xD1\x83\xD1\x81\xD0\xBA \xD0\xBF\xD1\x80\xD0\xB8 \xD1\x81\xD1\x82\xD0\xB0\xD1\x80\xD1\x82\xD0\xB5 Windows" },
    { L"Exit Application",            "\xD0\x92\xD1\x8B\xD1\x85\xD0\xBE\xD0\xB4 \xD0\xB8\xD0\xB7 \xD0\xBF\xD1\x80\xD0\xBE\xD0\xB3\xD1\x80\xD0\xB0\xD0\xBC\xD0\xBC\xD1\x8B" },
    { L"Add Application...",          "\xD0\x94\xD0\xBE\xD0\xB1\xD0\xB0\xD0\xB2\xD0\xB8\xD1\x82\xD1\x8C \xD0\xBF\xD1\x80\xD0\xB8\xD0\xBB\xD0\xBE\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xB8\xD0\xB5..." },
    { L"Add Folder...",               "\xD0\x94\xD0\xBE\xD0\xB1\xD0\xB0\xD0\xB2\xD0\xB8\xD1\x82\xD1\x8C \xD0\xBF\xD0\xB0\xD0\xBF\xD0\xBA\xD1\x83..." },
    { L"Expand Direction",            "\xD0\x9D\xD0\xB0\xD0\xBF\xD1\x80\xD0\xB0\xD0\xB2\xD0\xBB\xD0\xB5\xD0\xBD\xD0\xB8\xD0\xB5 \xD1\x80\xD0\xB0\xD1\x81\xD0\xBA\xD1\x80\xD1\x8B\xD1\x82\xD0\xB8\xD1\x8F" },
    { L"Down (Default)",              "\xD0\x92\xD0\xBD\xD0\xB8\xD0\xB7 (\xD0\xBF\xD0\xBE \xD1\x83\xD0\xBC\xD0\xBE\xD0\xBB\xD1\x87\xD0\xB0\xD0\xBD\xD0\xB8\xD1\x8E)" },
    { L"Up",                          "\xD0\x92\xD0\xB2\xD0\xB5\xD1\x80\xD1\x85" },
    { L"Right",                       "\xD0\x92\xD0\xBF\xD1\x80\xD0\xB0\xD0\xB2\xD0\xBE" },
    { L"Left",                        "\xD0\x92\xD0\xBB\xD0\xB5\xD0\xB2\xD0\xBE" },
    { L"Icon Size",                   "\xD0\xA0\xD0\xB0\xD0\xB7\xD0\xBC\xD0\xB5\xD1\x80 \xD0\xB8\xD0\xBA\xD0\xBE\xD0\xBD\xD0\xBE\xD0\xBA" },
    { L"Larger (+4)",                 "\xD0\x9A\xD1\x80\xD1\x83\xD0\xBF\xD0\xBD\xD0\xB5\xD0\xB5 (+4)" },
    { L"Smaller (-4)",                "\xD0\x9C\xD0\xB5\xD0\xBB\xD1\x8C\xD1\x87\xD0\xB5 (-4)" },
    { L"Dock Scale",                  "\xD0\x9C\xD0\xB0\xD1\x81\xD1\x88\xD1\x82\xD0\xB0\xD0\xB1 \xD0\xB4\xD0\xBE\xD0\xBA\xD0\xB0" },
    { L"Larger (+10%)",               "\xD0\x9A\xD1\x80\xD1\x83\xD0\xBF\xD0\xBD\xD0\xB5\xD0\xB5 (+10%)" },
    { L"Smaller (-10%)",              "\xD0\x9C\xD0\xB5\xD0\xBB\xD1\x8C\xD1\x87\xD0\xB5 (-10%)" },
    { L"Reset (100%)",                "\xD0\xA1\xD0\xB1\xD1\x80\xD0\xBE\xD1\x81\xD0\xB8\xD1\x82\xD1\x8C (100%)" },
    { L"Always Show Handle",          "\xD0\x92\xD1\x81\xD0\xB5\xD0\xB3\xD0\xB4\xD0\xB0 \xD0\xBF\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xB7\xD1\x8B\xD0\xB2\xD0\xB0\xD1\x82\xD1\x8C \xD0\xB8\xD0\xBD\xD0\xB4\xD0\xB8\xD0\xBA\xD0\xB0\xD1\x82\xD0\xBE\xD1\x80" },
    { L"Create Another Dock",         "\xD0\xA1\xD0\xBE\xD0\xB7\xD0\xB4\xD0\xB0\xD1\x82\xD1\x8C \xD0\xB5\xD1\x89\xD1\x91 \xD0\xBE\xD0\xB4\xD0\xB8\xD0\xBD \xD0\xB4\xD0\xBE\xD0\xBA" },
    { L"Close This Dock",             "\xD0\x97\xD0\xB0\xD0\xBA\xD1\x80\xD1\x8B\xD1\x82\xD1\x8C \xD0\xB4\xD0\xB0\xD0\xBD\xD0\xBD\xD1\x8B\xD0\xB9 \xD0\xB2\xD0\xB8\xD0\xB4\xD0\xB6\xD0\xB5\xD1\x82" },
    { L"Close All",                   "\xD0\x97\xD0\xB0\xD0\xBA\xD1\x80\xD1\x8B\xD1\x82\xD1\x8C \xD0\xB2\xD1\x81\xD1\x91" },
    { L"Delete Application / Folder", "\xD0\xA3\xD0\xB4\xD0\xB0\xD0\xBB\xD0\xB8\xD1\x82\xD1\x8C \xD0\xBF\xD1\x80\xD0\xB8\xD0\xBB\xD0\xBE\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xB8\xD0\xB5 / \xD0\xBF\xD0\xB0\xD0\xBF\xD0\xBA\xD1\x83" },
    { L"Select folder for dock",      "\xD0\x92\xD1\x8B\xD0\xB1\xD0\xB5\xD1\x80\xD0\xB8\xD1\x82\xD0\xB5 \xD0\xBF\xD0\xB0\xD0\xBF\xD0\xBA\xD1\x83 \xD0\xB4\xD0\xBB\xD1\x8F \xD0\xB4\xD0\xBE\xD0\xBA\xD0\xB0" },
    { L"Language / \xD0\xAF\xD0\xB7\xD1\x8B\xD0\xBA", "Language / \xD0\xAF\xD0\xB7\xD1\x8B\xD0\xBA" }
};

std::wstring U8(const char* utf8Str);

std::wstring GetStr(StringId id, Language lang) {
    if (id < 0 || id >= STR_COUNT) return L"";
    if (lang == Language::EN) {
        return g_strTable[id].en;
    }
    return U8(g_strTable[id].ru_u8);
}

std::wstring U8(const char* utf8Str) {
    if (!utf8Str) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
    if (len <= 0) return L"";
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, &wstr[0], len);
    return wstr;
}

std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &str[0], size, NULL, NULL);
    return str;
}

void AddMenuItem(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const std::wstring& text, bool checked = false) {
    UINT flags = uFlags | (checked ? MF_CHECKED : 0);
    AppendMenuW(hMenu, flags, uIDNewItem, text.c_str());
}

void AddPopupMenuItem(HMENU hMenu, HMENU hSubMenu, const std::wstring& text) {
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, text.c_str());
}

bool IsAutostartEnabled() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD len = 0;
        LSTATUS st = RegQueryValueExW(hKey, L"WidgetLauncherCpp", NULL, NULL, NULL, &len);
        RegCloseKey(hKey);
        return (st == ERROR_SUCCESS);
    }
    return false;
}

void SetAutostart(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            wchar_t exePath[MAX_PATH];
            GetModuleFileNameW(NULL, exePath, MAX_PATH);
            std::wstring cmd = L"\"" + std::wstring(exePath) + L"\"";
            RegSetValueExW(hKey, L"WidgetLauncherCpp", 0, REG_SZ, (const BYTE*)cmd.c_str(), (DWORD)(cmd.size() + 1) * sizeof(wchar_t));
        }
        else {
            RegDeleteValueW(hKey, L"WidgetLauncherCpp");
        }
        RegCloseKey(hKey);
    }
}

enum PreferredAppMode { Default, AllowDark, ForceDark, ForceLight, Max };
using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode);

void EnableWindowDarkMenu() {
    HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hUxtheme) {
        auto SetPreferredAppMode = (fnSetPreferredAppMode)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
        if (SetPreferredAppMode) {
            SetPreferredAppMode(ForceDark);
        }
        FreeLibrary(hUxtheme);
    }
}

enum class Direction { Down, Up, Right, Left };

struct AppItem {
    std::wstring path;
    std::wstring displayName;
    std::wstring cachedIconPath;
    HICON hIcon = nullptr;
    Gdiplus::Bitmap* pBmp = nullptr;
    float hoverProgress = 0.0f;
};

const int PILL_LONG = 52;
const int PILL_THICK = 16;
const int PAD = 8;
const int MARGIN_BUF = 70;

class DockWindow;
std::vector<DockWindow*> g_docks;
HWND g_hTooltipWnd = NULL;
NOTIFYICONDATAW g_nid = { 0 };
ULONG_PTR g_gdiplusToken = 0;
HICON g_hAppIcon = NULL;

Language g_currentLang = Language::EN;

std::wstring GetBaseDir() {
    wchar_t appData[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appData);
    std::wstring dir = std::wstring(appData) + L"\\WidgetLauncher";
    CreateDirectoryW(dir.c_str(), NULL);
    CreateDirectoryW((dir + L"\\icons").c_str(), NULL);
    return dir;
}

std::wstring GetConfigPath() {
    return GetBaseDir() + L"\\config_cpp.txt";
}

HICON CreateProceduralChromeIcon() {
    Bitmap bmp(32, 32, PixelFormat32bppARGB);
    Graphics g(&bmp);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    GraphicsPath path;
    path.AddEllipse(2, 2, 27, 27);

    LinearGradientBrush brush(Point(2, 2), Point(29, 29), Color(255, 255, 255), Color(45, 52, 62));
    Color colors[] = { Color(255, 255, 255), Color(210, 218, 226), Color(85, 95, 108), Color(35, 40, 48), Color(130, 140, 150) };
    float pos[] = { 0.0f, 0.25f, 0.55f, 0.85f, 1.0f };
    brush.SetInterpolationColors(colors, pos, 5);
    g.FillPath(&brush, &path);

    Pen pen(Color(200, 255, 255, 255), 1.2f);
    g.DrawPath(&pen, &path);

    SolidBrush shine(Color(230, 255, 255, 255));
    g.FillEllipse(&shine, 7, 5, 12, 6);

    HICON hIcon = nullptr;
    bmp.GetHICON(&hIcon);
    return hIcon;
}

HICON LoadAppIcon(HINSTANCE hInstance) {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    PathRemoveFileSpecW(exePath);

    std::vector<std::wstring> paths = {
        std::wstring(exePath) + L"\\app.ico",
        std::wstring(exePath) + L"\\..\\app.ico",
        std::wstring(exePath) + L"\\..\\..\\app.ico",
        L"app.ico"
    };

    for (const auto& p : paths) {
        if (PathFileExistsW(p.c_str())) {
            HICON h = (HICON)LoadImageW(NULL, p.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            if (h) return h;
        }
    }

    HICON hRes = (HICON)LoadImageW(hInstance, MAKEINTRESOURCEW(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
    if (hRes) return hRes;

    return CreateProceduralChromeIcon();
}

typedef HRESULT(WINAPI* PFN_SHGetImageList)(int iImageList, REFIID riid, void** ppv);

HICON ExtractHighResIcon(const std::wstring& path) {
    SHFILEINFOW sfi = { 0 };
    DWORD flags = SHGFI_SYSICONINDEX;
    SHGetFileInfoW(path.c_str(), 0, &sfi, sizeof(sfi), flags);

    HMODULE hShell32 = GetModuleHandleW(L"shell32.dll");
    if (hShell32) {
        PFN_SHGetImageList pfn = (PFN_SHGetImageList)GetProcAddress(hShell32, "SHGetImageList");
        if (pfn) {
            IImageList* piml = nullptr;
            if (SUCCEEDED(pfn(0x4, IID_IImageList, (void**)&piml)) && piml) {
                HICON hIcon = nullptr;
                piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hIcon);
                piml->Release();
                if (hIcon) return hIcon;
            }
        }
    }

    SHFILEINFOW sfi2 = { 0 };
    SHGetFileInfoW(path.c_str(), 0, &sfi2, sizeof(sfi2), SHGFI_ICON | SHGFI_LARGEICON);
    return sfi2.hIcon;
}

Gdiplus::Bitmap* CreateMonochromeIcon(HICON hIcon, int targetSize) {
    if (!hIcon) return nullptr;

    Bitmap rawBmp(hIcon);
    int w = rawBmp.GetWidth();
    int h = rawBmp.GetHeight();
    if (w <= 0 || h <= 0) return nullptr;

    int minX = w, maxX = 0, minY = h, maxY = 0;
    BitmapData data;
    Rect lockRc(0, 0, w, h);
    if (rawBmp.LockBits(&lockRc, ImageLockModeRead, PixelFormat32bppARGB, &data) == Ok) {
        BYTE* pixels = (BYTE*)data.Scan0;
        int stride = data.Stride;

        for (int y = 0; y < h; y++) {
            BYTE* row = pixels + y * stride;
            for (int x = 0; x < w; x++) {
                if (row[x * 4 + 3] > 15) {
                    if (x < minX) minX = x;
                    if (x > maxX) maxX = x;
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }
        rawBmp.UnlockBits(&data);
    }

    if (maxX < minX || maxY < minY) {
        minX = 0; maxX = w - 1;
        minY = 0; maxY = h - 1;
    }

    minX = (std::max)(0, minX - 1);
    minY = (std::max)(0, minY - 1);
    maxX = (std::min)(w - 1, maxX + 1);
    maxY = (std::min)(h - 1, maxY + 1);

    int cropW = maxX - minX + 1;
    int cropH = maxY - minY + 1;

    float scale = (std::min)((float)targetSize / (float)cropW, (float)targetSize / (float)cropH);
    int drawW = (int)(cropW * scale);
    int drawH = (int)(cropH * scale);
    int drawX = (targetSize - drawW) / 2;
    int drawY = (targetSize - drawH) / 2;

    Bitmap* pBmp = new Bitmap(targetSize, targetSize, PixelFormat32bppARGB);
    Graphics g(pBmp);
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    g.SetSmoothingMode(SmoothingModeAntiAlias);

    ColorMatrix cm = {
        0.2126f, 0.2126f, 0.2126f, 0.0f, 0.0f,
        0.7152f, 0.7152f, 0.7152f, 0.0f, 0.0f,
        0.0722f, 0.0722f, 0.0722f, 0.0f, 0.0f,
        0.0000f, 0.0000f, 0.0000f, 1.0f, 0.0f,
        0.1600f, 0.1600f, 0.1600f, 0.0f, 1.0f
    };

    ImageAttributes attr;
    attr.SetColorMatrix(&cm, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    g.DrawImage(&rawBmp, Rect(drawX, drawY, drawW, drawH), minX, minY, cropW, cropH, UnitPixel, &attr);
    return pBmp;
}

void AddRoundedRect(GraphicsPath& path, const Rect& rc, int r) {
    REAL dia = (REAL)r * 2.0f;
    REAL right = (REAL)(rc.X + rc.Width);
    REAL bottom = (REAL)(rc.Y + rc.Height);

    path.AddArc((REAL)rc.X, (REAL)rc.Y, dia, dia, 180.0f, 90.0f);
    path.AddArc(right - dia, (REAL)rc.Y, dia, dia, 270.0f, 90.0f);
    path.AddArc(right - dia, bottom - dia, dia, dia, 0.0f, 90.0f);
    path.AddArc((REAL)rc.X, bottom - dia, dia, dia, 90.0f, 90.0f);
    path.CloseFigure();
}

void RenderTooltip(const std::wstring& text, int screenTargetX, int screenTargetY, int slotH) {
    if (text.empty()) {
        ShowWindow(g_hTooltipWnd, SW_HIDE);
        return;
    }

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    Graphics gMeasure(hdcMem);
    FontFamily ff(L"Segoe UI");
    Gdiplus::Font font(&ff, 9.5f, FontStyleBold, UnitPoint);

    StringFormat sf;
    sf.SetAlignment(StringAlignmentCenter);
    sf.SetLineAlignment(StringAlignmentCenter);

    RectF bounds;
    gMeasure.MeasureString(text.c_str(), (int)text.size(), &font, PointF(0, 0), &sf, &bounds);

    int tipW = (int)bounds.Width + 24;
    int tipH = (int)bounds.Height + 12;

    int tipScreenX = screenTargetX - tipW / 2;
    int tipScreenY = screenTargetY - tipH - 8;

    if (tipScreenY < 8) {
        tipScreenY = screenTargetY + slotH + 8;
    }

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = tipW;
    bmi.bmiHeader.biHeight = -tipH;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    HBITMAP hBmp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (!hBmp) {
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return;
    }

    HGDIOBJ hOld = SelectObject(hdcMem, hBmp);

    Graphics g(hdcMem);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    GraphicsPath path;
    Rect rc(0, 0, tipW, tipH);
    AddRoundedRect(path, rc, 8);

    SolidBrush bg(Color(140, 16, 18, 22));
    g.FillPath(&bg, &path);

    Pen border(Color(65, 255, 255, 255), 1.0f);
    g.DrawPath(&border, &path);

    SolidBrush textBrush(Color(255, 250, 250, 250));
    g.DrawString(text.c_str(), (int)text.size(), &font, RectF(0, 0, (REAL)tipW, (REAL)tipH), &sf, &textBrush);

    POINT ptSrc = { 0, 0 };
    POINT ptDst = { tipScreenX, tipScreenY };
    SIZE sz = { tipW, tipH };
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    UpdateLayeredWindow(g_hTooltipWnd, hdcScreen, &ptDst, &sz, hdcMem, &ptSrc, 0, &bf, ULW_ALPHA);
    ShowWindow(g_hTooltipWnd, SW_SHOWNOACTIVATE);

    SelectObject(hdcMem, hOld);
    DeleteObject(hBmp);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}

void SaveAllConfigs();

class DockWindow {
public:
    HWND hWnd = NULL;
    int x = 140;
    int y = 140;
    Direction dir = Direction::Down;
    int iconSize = 46;
    float scale = 1.10f;
    bool topmost = false;
    bool alwaysVisible = false;
    std::vector<AppItem> apps;

    bool isExpanded = false;
    float animProgress = 0.0f;
    float animStartProgress = 0.0f;
    float animTargetProgress = 0.0f;
    ULONGLONG animStartTime = 0;
    bool isAnimating = false;
    float chevronAngle = 0.0f;

    float pillOpacity = 0.0f;
    bool isNear = false;
    int hoveredAppIdx = -1;
    bool isPillHovered = false;
    bool pinnedByTray = false;
    bool isDragging = false;
    POINT dragStartMouse = { 0 };
    POINT dragStartWin = { 0 };

    RECT pillScreenRect = { 0 };
    RECT capScreenRect = { 0 };
    std::vector<RECT> appScreenRects;

    DockWindow(int px, int py, Direction d) : x(px), y(py), dir(d) {}

    void CacheIcon(AppItem& item, int index) {
        if (!item.pBmp) return;
        std::wstring file = GetBaseDir() + L"\\icons\\dock_" + std::to_wstring((uintptr_t)this) + L"_" + std::to_wstring(index) + L".png";
        item.pBmp->Save(file.c_str(), &CLSID_PNG, NULL);
        item.cachedIconPath = file;
    }

    void LoadIcon(AppItem& item, int index) {
        std::wstring file = GetBaseDir() + L"\\icons\\dock_" + std::to_wstring((uintptr_t)this) + L"_" + std::to_wstring(index) + L".png";
        if (PathFileExistsW(file.c_str())) {
            item.pBmp = Bitmap::FromFile(file.c_str());
            item.cachedIconPath = file;
            return;
        }

        item.hIcon = ExtractHighResIcon(item.path);
        if (item.hIcon) {
            int iconPx = (int)(iconSize * scale);
            item.pBmp = CreateMonochromeIcon(item.hIcon, iconPx);
            CacheIcon(item, index);
        }
    }

    void ReloadBitmaps() {
        for (int i = 0; i < (int)apps.size(); i++) {
            if (apps[i].pBmp) { delete apps[i].pBmp; apps[i].pBmp = nullptr; }
            HICON h = ExtractHighResIcon(apps[i].path);
            if (h) {
                apps[i].pBmp = CreateMonochromeIcon(h, iconSize);
                CacheIcon(apps[i], i);
                DestroyIcon(h);
            }
        }
    }

    void AddAppPath(const std::wstring& path) {
        AppItem item;
        item.path = path;
        wchar_t fname[MAX_PATH];
        wcscpy_s(fname, path.c_str());
        PathRemoveExtensionW(fname);
        item.displayName = PathFindFileNameW(fname);

        int newIdx = (int)apps.size();
        item.hIcon = ExtractHighResIcon(item.path);
        if (item.hIcon) {
            int iconPx = (int)(iconSize * scale);
            item.pBmp = CreateMonochromeIcon(item.hIcon, iconPx);
            CacheIcon(item, newIdx);
        }

        apps.push_back(item);
        SaveAllConfigs();
    }

    void Toggle() {
        isExpanded = !isExpanded;
        animStartTime = GetTickCount64();
        animStartProgress = animProgress;
        animTargetProgress = isExpanded ? 1.0f : 0.0f;
        isAnimating = true;
    }

    void GetMetrics(int& winW, int& winH, int& capsuleSize, int slotSize, int pillW, int pillH, int pad) {
        int count = (int)apps.size();
        capsuleSize = (count > 0) ? (count * slotSize + pad * 2) : (slotSize + pad * 2);

        float eased = animProgress;
        int curCap = (int)(capsuleSize * eased);

        if (dir == Direction::Down || dir == Direction::Up) {
            winW = slotSize + pad * 2 + MARGIN_BUF * 2;
            winH = pillH + pad + curCap + MARGIN_BUF * 2;
        }
        else {
            winW = pillH + pad + curCap + MARGIN_BUF * 2;
            winH = slotSize + pad * 2 + MARGIN_BUF * 2;
        }
    }

    void Render() {
        float sc = scale;
        int slotSize = (int)(58 * sc);
        int pad = (int)(PAD * sc);
        int pillW = (int)(PILL_LONG * sc);
        int pillH = (int)(PILL_THICK * sc);
        int cornerR = (int)(18 * sc);

        int winW, winH, capsuleSize;
        GetMetrics(winW, winH, capsuleSize, slotSize, pillW, pillH, pad);

        int curCap = (int)(capsuleSize * animProgress);

        int drawX = x;
        int drawY = y;

        if (dir == Direction::Up) {
            drawY = y - (winH - (pillH + MARGIN_BUF * 2));
        }
        else if (dir == Direction::Left) {
            drawX = x - (winW - (pillH + MARGIN_BUF * 2));
        }

        SetWindowPos(hWnd, topmost ? HWND_TOPMOST : HWND_NOTOPMOST, drawX, drawY, winW, winH, SWP_NOACTIVATE);

        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = winW;
        bmi.bmiHeader.biHeight = -winH;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HBITMAP hBmp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        if (!hBmp) {
            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdcScreen);
            return;
        }

        HGDIOBJ hOld = SelectObject(hdcMem, hBmp);

        Graphics g(hdcMem);
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

        Rect pillRc, capRc;
        bool isHorizPill = (dir == Direction::Down || dir == Direction::Up);

        if (dir == Direction::Down) {
            pillRc = Rect(MARGIN_BUF + (winW - MARGIN_BUF * 2 - pillW) / 2, MARGIN_BUF, pillW, pillH);
            capRc = Rect(MARGIN_BUF, MARGIN_BUF + pillH + pad, winW - MARGIN_BUF * 2, curCap);
        }
        else if (dir == Direction::Up) {
            pillRc = Rect(MARGIN_BUF + (winW - MARGIN_BUF * 2 - pillW) / 2, MARGIN_BUF + curCap + pad, pillW, pillH);
            capRc = Rect(MARGIN_BUF, MARGIN_BUF, winW - MARGIN_BUF * 2, curCap);
        }
        else if (dir == Direction::Right) {
            pillRc = Rect(MARGIN_BUF, MARGIN_BUF + (winH - MARGIN_BUF * 2 - pillW) / 2, pillH, pillW);
            capRc = Rect(MARGIN_BUF + pillH + pad, MARGIN_BUF, curCap, winH - MARGIN_BUF * 2);
        }
        else {
            pillRc = Rect(MARGIN_BUF + curCap + pad, MARGIN_BUF + (winH - MARGIN_BUF * 2 - pillW) / 2, pillH, pillW);
            capRc = Rect(MARGIN_BUF, MARGIN_BUF, curCap, winH - MARGIN_BUF * 2);
        }

        pillScreenRect = { drawX + pillRc.X, drawY + pillRc.Y, drawX + pillRc.X + pillRc.Width, drawY + pillRc.Y + pillRc.Height };
        capScreenRect = { drawX + capRc.X, drawY + capRc.Y, drawX + capRc.X + capRc.Width, drawY + capRc.Y + capRc.Height };

        appScreenRects.clear();

        if (animProgress > 0.01f) {
            if (capRc.Width > 0 && capRc.Height > 0) {
                GraphicsPath path;
                AddRoundedRect(path, capRc, cornerR);
                SolidBrush bg(Color((BYTE)(200 * animProgress), 16, 18, 22));
                g.FillPath(&bg, &path);
                Pen border(Color((BYTE)(110 * animProgress), 255, 255, 255), 1.0f);
                g.DrawPath(&border, &path);
            }

            GraphicsState capState = g.Save();
            g.SetClip(capRc);

            bool isHorizLayout = (dir == Direction::Right || dir == Direction::Left);
            int tileSize = slotSize - 6;
            int iconVisualSize = iconSize;

            for (int i = 0; i < (int)apps.size(); i++) {
                int slotX = isHorizLayout ? (capRc.X + pad + i * slotSize) : (capRc.X + (capRc.Width - slotSize) / 2);
                int slotY = isHorizLayout ? (capRc.Y + (capRc.Height - slotSize) / 2) : (capRc.Y + pad + i * slotSize);

                RECT appRc = { drawX + slotX, drawY + slotY, drawX + slotX + slotSize, drawY + slotY + slotSize };
                appScreenRects.push_back(appRc);

                float hProg = apps[i].hoverProgress;

                int tileX = slotX + (slotSize - tileSize) / 2;
                int tileY = slotY + (slotSize - tileSize) / 2;
                Rect tileRc(tileX, tileY, tileSize, tileSize);

                if (hProg > 0.01f) {
                    GraphicsPath hoverPath;
                    AddRoundedRect(hoverPath, tileRc, 10);
                    SolidBrush hoverBg(Color((BYTE)(48 * hProg * animProgress), 255, 255, 255));
                    g.FillPath(&hoverBg, &hoverPath);
                }

                int iconX = slotX + (slotSize - iconVisualSize) / 2;
                int iconY = slotY + (slotSize - iconVisualSize) / 2;
                int liftY = (int)(-4.0f * hProg);

                if (apps[i].pBmp) {
                    g.DrawImage(apps[i].pBmp, iconX, iconY + liftY, iconVisualSize, iconVisualSize);
                }

                if (i == hoveredAppIdx) {
                    RenderTooltip(apps[i].displayName, drawX + slotX + slotSize / 2, drawY + slotY + liftY, slotSize);
                }
            }
            g.Restore(capState);
        }
        else {
            if (hoveredAppIdx < 0) ShowWindow(g_hTooltipWnd, SW_HIDE);
        }

        if (pillOpacity > 0.01f || isExpanded) {
            GraphicsPath pPath;
            int r = (isHorizPill ? pillRc.Height : pillRc.Width) / 2;
            AddRoundedRect(pPath, pillRc, r);

            LinearGradientBrush pBrush(pillRc, Color(255, 255, 255), Color(40, 48, 56), isHorizPill ? LinearGradientModeVertical : LinearGradientModeHorizontal);
            Color pColors[] = { Color(255, 255, 255), Color(215, 222, 230), Color(98, 108, 122), Color(40, 48, 56), Color(130, 140, 152) };
            float pPos[] = { 0.0f, 0.25f, 0.55f, 0.85f, 1.0f };
            pBrush.SetInterpolationColors(pColors, pPos, 5);

            g.FillPath(&pBrush, &pPath);
            Pen pPen(Color(200, 255, 255, 255), 1.2f);
            g.DrawPath(&pPen, &pPath);

            GraphicsState pState = g.Save();
            float pcx = (float)pillRc.X + (float)pillRc.Width / 2.0f;
            float pcy = (float)pillRc.Y + (float)pillRc.Height / 2.0f;
            g.TranslateTransform(pcx, pcy);
            g.RotateTransform(chevronAngle);

            PointF pts[3] = { PointF(-4.5f, -2.0f), PointF(0.0f, 2.5f), PointF(4.5f, -2.0f) };
            Pen arrowPen(Color(235, 245, 255), 1.8f);
            arrowPen.SetStartCap(LineCapRound);
            arrowPen.SetEndCap(LineCapRound);
            g.DrawLines(&arrowPen, pts, 3);
            g.Restore(pState);
        }

        POINT ptSrc = { 0, 0 };
        POINT ptDst = { drawX, drawY };
        SIZE sz = { winW, winH };
        BYTE finalAlpha = (BYTE)(255 * (isExpanded ? 1.0f : pillOpacity));
        BLENDFUNCTION bf = { AC_SRC_OVER, 0, finalAlpha, AC_SRC_ALPHA };

        UpdateLayeredWindow(hWnd, hdcScreen, &ptDst, &sz, hdcMem, &ptSrc, 0, &bf, ULW_ALPHA);

        SelectObject(hdcMem, hOld);
        DeleteObject(hBmp);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
    }

    void ShowContextMenu() {
        HMENU hMenu = CreatePopupMenu();
        AddMenuItem(hMenu, MF_STRING, 201, GetStr(STR_ADD_APP, g_currentLang));
        AddMenuItem(hMenu, MF_STRING, 202, GetStr(STR_ADD_FOLDER, g_currentLang));
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

        HMENU hSubDir = CreatePopupMenu();
        AddMenuItem(hSubDir, MF_STRING, 301, GetStr(STR_DIR_DOWN, g_currentLang), dir == Direction::Down);
        AddMenuItem(hSubDir, MF_STRING, 302, GetStr(STR_DIR_UP, g_currentLang), dir == Direction::Up);
        AddMenuItem(hSubDir, MF_STRING, 303, GetStr(STR_DIR_RIGHT, g_currentLang), dir == Direction::Right);
        AddMenuItem(hSubDir, MF_STRING, 304, GetStr(STR_DIR_LEFT, g_currentLang), dir == Direction::Left);
        AddPopupMenuItem(hMenu, hSubDir, GetStr(STR_DIRECTION, g_currentLang));

        HMENU hSubIcon = CreatePopupMenu();
        AddMenuItem(hSubIcon, MF_STRING, 411, GetStr(STR_SIZE_UP, g_currentLang));
        AddMenuItem(hSubIcon, MF_STRING, 412, GetStr(STR_SIZE_DOWN, g_currentLang));
        AddPopupMenuItem(hMenu, hSubIcon, GetStr(STR_ICON_SIZE, g_currentLang));

        HMENU hSubScale = CreatePopupMenu();
        AddMenuItem(hSubScale, MF_STRING, 401, GetStr(STR_SCALE_UP, g_currentLang));
        AddMenuItem(hSubScale, MF_STRING, 402, GetStr(STR_SCALE_DOWN, g_currentLang));
        AddMenuItem(hSubScale, MF_STRING, 403, GetStr(STR_SCALE_RESET, g_currentLang));
        AddPopupMenuItem(hMenu, hSubScale, GetStr(STR_DOCK_SCALE, g_currentLang));

        AddMenuItem(hMenu, MF_STRING, 203, GetStr(STR_TOPMOST, g_currentLang), topmost);
        AddMenuItem(hMenu, MF_STRING, 204, GetStr(STR_ALWAYS_VIS, g_currentLang), alwaysVisible);
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

        HMENU hSubLang = CreatePopupMenu();
        AddMenuItem(hSubLang, MF_STRING, 601, L"English", g_currentLang == Language::EN);
        AddMenuItem(hSubLang, MF_STRING, 602, L"\x0420\x0443\x0441\x0441\x043A\x0438\x0439", g_currentLang == Language::RU);
        AddPopupMenuItem(hMenu, hSubLang, GetStr(STR_LANG_MENU, g_currentLang));

        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AddMenuItem(hMenu, MF_STRING, 501, GetStr(STR_NEW_DOCK, g_currentLang));
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AddMenuItem(hMenu, MF_STRING, 502, GetStr(STR_CLOSE_THIS, g_currentLang));
        AddMenuItem(hMenu, MF_STRING, 503, GetStr(STR_CLOSE_ALL, g_currentLang));

        POINT pt;
        GetCursorPos(&pt);
        SetForegroundWindow(hWnd);
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
    }
};

void SaveAllConfigs() {
    std::string path = WideToUtf8(GetConfigPath());
    std::ofstream out(path);
    if (!out.is_open()) return;

    out << "GlobalLang=" << (int)g_currentLang << "\n";

    for (auto* dock : g_docks) {
        out << "[Dock]\n";
        out << "X=" << dock->x << "\n";
        out << "Y=" << dock->y << "\n";
        out << "Dir=" << (int)dock->dir << "\n";
        out << "IconSize=" << dock->iconSize << "\n";
        out << "Scale=" << dock->scale << "\n";
        out << "Topmost=" << (dock->topmost ? 1 : 0) << "\n";
        out << "Always=" << (dock->alwaysVisible ? 1 : 0) << "\n";

        for (const auto& app : dock->apps) {
            out << "App=" << WideToUtf8(app.path) << "|" << WideToUtf8(app.displayName) << "\n";
        }
    }
}

void CreateNewDock(int px, int py, Direction d);

void ShowTrayMenu() {
    HMENU hMenu = CreatePopupMenu();
    AddMenuItem(hMenu, MF_STRING, 100, GetStr(STR_TOGGLE_ALL, g_currentLang));
    AddMenuItem(hMenu, MF_STRING, 101, GetStr(STR_NEW_DOCK, g_currentLang));
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);

    HMENU hSubLang = CreatePopupMenu();
    AddMenuItem(hSubLang, MF_STRING, 601, L"English", g_currentLang == Language::EN);
    AddMenuItem(hSubLang, MF_STRING, 602, L"\x0420\x0443\x0441\x0441\x043A\x0438\x0439", g_currentLang == Language::RU);
    AddPopupMenuItem(hMenu, hSubLang, GetStr(STR_LANG_MENU, g_currentLang));

    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AddMenuItem(hMenu, MF_STRING, 102, GetStr(STR_AUTOSTART, g_currentLang), IsAutostartEnabled());
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AddMenuItem(hMenu, MF_STRING, 103, GetStr(STR_EXIT_PROG, g_currentLang));

    POINT pt;
    GetCursorPos(&pt);
    HWND hTarget = g_docks.empty() ? NULL : g_docks[0]->hWnd;
    if (hTarget) {
        SetForegroundWindow(hTarget);
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hTarget, NULL);
    }
    DestroyMenu(hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    DockWindow* pDock = (DockWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (msg) {
    case WM_CREATE: {
        DragAcceptFiles(hWnd, TRUE);
        timeBeginPeriod(1);
        SetTimer(hWnd, 1, 5, NULL);
        SetTimer(hWnd, 2, 70, NULL);
        break;
    }
    case WM_TIMER: {
        if (!pDock) break;

        if (wParam == 1) {
            bool animating = false;

            if (pDock->isAnimating) {
                ULONGLONG now = GetTickCount64();
                float elapsed = (float)(now - pDock->animStartTime) / 220.0f;
                if (elapsed >= 1.0f) {
                    elapsed = 1.0f;
                    pDock->animProgress = pDock->animTargetProgress;
                    pDock->isAnimating = false;
                }
                else {
                    float t = 1.0f - std::pow(1.0f - elapsed, 3.0f);
                    pDock->animProgress = pDock->animStartProgress + (pDock->animTargetProgress - pDock->animStartProgress) * t;
                    animating = true;
                }
            }

            float targetAngle = 0.0f;
            if (pDock->dir == Direction::Down) targetAngle = pDock->isExpanded ? 180.0f : 0.0f;
            else if (pDock->dir == Direction::Up) targetAngle = pDock->isExpanded ? 0.0f : 180.0f;
            else if (pDock->dir == Direction::Right) targetAngle = pDock->isExpanded ? 90.0f : 270.0f;
            else if (pDock->dir == Direction::Left) targetAngle = pDock->isExpanded ? 270.0f : 90.0f;

            float diffA = targetAngle - pDock->chevronAngle;
            if (std::abs(diffA) > 0.5f) {
                pDock->chevronAngle += diffA * 0.28f;
                animating = true;
            }
            else {
                pDock->chevronAngle = targetAngle;
            }

            for (int i = 0; i < (int)pDock->apps.size(); i++) {
                float targetH = (i == pDock->hoveredAppIdx) ? 1.0f : 0.0f;
                float diffH = targetH - pDock->apps[i].hoverProgress;
                if (std::abs(diffH) > 0.01f) {
                    pDock->apps[i].hoverProgress += diffH * 0.28f;
                    animating = true;
                }
                else {
                    pDock->apps[i].hoverProgress = targetH;
                }
            }

            float targetPillOpacity = pDock->isNear ? 1.0f : 0.0f;
            float diffO = targetPillOpacity - pDock->pillOpacity;
            if (std::abs(diffO) > 0.01f) {
                pDock->pillOpacity += diffO * 0.20f;
                animating = true;
            }
            else {
                pDock->pillOpacity = targetPillOpacity;
            }

            if (animating || pDock->isAnimating) {
                pDock->Render();
            }
        }
        else if (wParam == 2) {
            if (pDock->isExpanded || pDock->pinnedByTray || pDock->isDragging) return 0;

            POINT pt;
            GetCursorPos(&pt);

            int cx = (pDock->pillScreenRect.left + pDock->pillScreenRect.right) / 2;
            int cy = (pDock->pillScreenRect.top + pDock->pillScreenRect.bottom) / 2;

            if (cx == 0 && cy == 0) {
                cx = pDock->x + MARGIN_BUF + PILL_LONG / 2;
                cy = pDock->y + MARGIN_BUF + PILL_THICK / 2;
            }

            double dist = std::sqrt((pt.x - cx) * (pt.x - cx) + (pt.y - cy) * (pt.y - cy));
            pDock->isNear = (dist <= 200.0) || pDock->alwaysVisible;
        }
        break;
    }
    case WM_MOUSEMOVE: {
        if (!pDock) break;

        POINT pt;
        GetCursorPos(&pt);

        if (pDock->isDragging) {
            pDock->x = pDock->dragStartWin.x + (pt.x - pDock->dragStartMouse.x);
            pDock->y = pDock->dragStartWin.y + (pt.y - pDock->dragStartMouse.y);
            pDock->Render();
            return 0;
        }

        pDock->isPillHovered = PtInRect(&pDock->pillScreenRect, pt);
        int prevHover = pDock->hoveredAppIdx;
        pDock->hoveredAppIdx = -1;

        if (pDock->isExpanded) {
            for (int i = 0; i < (int)pDock->appScreenRects.size(); i++) {
                if (PtInRect(&pDock->appScreenRects[i], pt)) {
                    pDock->hoveredAppIdx = i;
                    break;
                }
            }
        }

        if (prevHover != pDock->hoveredAppIdx) {
            if (pDock->hoveredAppIdx < 0) {
                ShowWindow(g_hTooltipWnd, SW_HIDE);
            }
            pDock->Render();
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        if (!pDock) break;

        POINT pt;
        GetCursorPos(&pt);

        if (pDock->isExpanded) {
            for (int i = 0; i < (int)pDock->appScreenRects.size(); i++) {
                if (PtInRect(&pDock->appScreenRects[i], pt)) {
                    ShellExecuteW(NULL, L"open", pDock->apps[i].path.c_str(), NULL, NULL, SW_SHOWNORMAL);
                    return 0;
                }
            }
        }

        if (PtInRect(&pDock->pillScreenRect, pt)) {
            pDock->pinnedByTray = false;
            pDock->Toggle();
            return 0;
        }

        if (pDock->isExpanded && PtInRect(&pDock->capScreenRect, pt)) {
            pDock->isDragging = true;
            pDock->dragStartMouse = pt;
            pDock->dragStartWin = { pDock->x, pDock->y };
            SetCapture(hWnd);
            return 0;
        }
        break;
    }
    case WM_LBUTTONUP: {
        if (pDock && pDock->isDragging) {
            pDock->isDragging = false;
            ReleaseCapture();
            SaveAllConfigs();
        }
        break;
    }
    case WM_RBUTTONUP: {
        if (!pDock) break;

        POINT pt;
        GetCursorPos(&pt);

        if (pDock->isExpanded) {
            for (int i = 0; i < (int)pDock->appScreenRects.size(); i++) {
                if (PtInRect(&pDock->appScreenRects[i], pt)) {
                    HMENU hMenu = CreatePopupMenu();
                    AddMenuItem(hMenu, MF_STRING, 999, GetStr(STR_DELETE_ITEM, g_currentLang));
                    SetForegroundWindow(hWnd);
                    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
                    DestroyMenu(hMenu);

                    if (cmd == 999) {
                        if (pDock->apps[i].pBmp) delete pDock->apps[i].pBmp;
                        if (pDock->apps[i].hIcon) DestroyIcon(pDock->apps[i].hIcon);
                        if (!pDock->apps[i].cachedIconPath.empty()) DeleteFileW(pDock->apps[i].cachedIconPath.c_str());
                        pDock->apps.erase(pDock->apps.begin() + i);
                        SaveAllConfigs();
                        pDock->Render();
                    }
                    return 0;
                }
            }
        }

        pDock->ShowContextMenu();
        break;
    }
    case WM_DROPFILES: {
        if (!pDock) break;

        HDROP hDrop = (HDROP)wParam;
        UINT count = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
        for (UINT i = 0; i < count; i++) {
            wchar_t path[MAX_PATH];
            DragQueryFileW(hDrop, i, path, MAX_PATH);
            pDock->AddAppPath(path);
        }
        DragFinish(hDrop);
        if (!pDock->isExpanded) pDock->Toggle();
        pDock->Render();
        break;
    }
    case WM_USER + 1: {
        if (lParam == WM_RBUTTONUP) ShowTrayMenu();
        else if (lParam == WM_LBUTTONUP) {
            for (auto* d : g_docks) {
                d->pinnedByTray = true;
                d->Toggle();
            }
        }
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 100) {
            for (auto* d : g_docks) d->Toggle();
        }
        else if (id == 101 || id == 203) {
            if (pDock) { pDock->topmost = !pDock->topmost; SaveAllConfigs(); pDock->Render(); }
        }
        else if (id == 102) {
            SetAutostart(!IsAutostartEnabled());
        }
        else if (id == 103 || id == 503) {
            SaveAllConfigs();
            for (auto* d : g_docks) DestroyWindow(d->hWnd);
            PostQuitMessage(0);
        }
        else if (id == 601) {
            g_currentLang = Language::EN;
            SaveAllConfigs();
        }
        else if (id == 602) {
            g_currentLang = Language::RU;
            SaveAllConfigs();
        }
        else if (pDock) {
            if (id == 501) {
                CreateNewDock(pDock->x + 50, pDock->y + 50, Direction::Down);
            }
            else if (id == 502) {
                auto it = std::find(g_docks.begin(), g_docks.end(), pDock);
                if (it != g_docks.end()) g_docks.erase(it);
                DestroyWindow(pDock->hWnd);
                delete pDock;
                SaveAllConfigs();
                if (g_docks.empty()) PostQuitMessage(0);
            }
            else if (id == 201) {
                OPENFILENAMEW ofn = { 0 };
                ofn.lStructSize = sizeof(ofn);
                wchar_t f[MAX_PATH] = { 0 };
                ofn.lpstrFile = f; ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFilter = L"Applications & Shortcuts\0*.exe;*.lnk;*.url\0All Files\0*.*\0";
                if (GetOpenFileNameW(&ofn)) { pDock->AddAppPath(f); if (!pDock->isExpanded) pDock->Toggle(); pDock->Render(); }
            }
            else if (id == 202) {
                BROWSEINFOW bi = { 0 };
                std::wstring title = GetStr(STR_SELECT_FOLD, g_currentLang);
                bi.lpszTitle = title.c_str();
                LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
                if (pidl) {
                    wchar_t path[MAX_PATH];
                    SHGetPathFromIDListW(pidl, path);
                    pDock->AddAppPath(path);
                    CoTaskMemFree(pidl);
                    if (!pDock->isExpanded) pDock->Toggle();
                    pDock->Render();
                }
            }
            else if (id == 204) {
                pDock->alwaysVisible = !pDock->alwaysVisible;
                SaveAllConfigs();
                pDock->Render();
            }
            else if (id >= 301 && id <= 304) {
                Direction dirs[] = { Direction::Down, Direction::Up, Direction::Right, Direction::Left };
                pDock->dir = dirs[id - 301];
                SaveAllConfigs();
                pDock->Render();
            }
            else if (id == 401) {
                if (pDock->scale < 1.75f) { pDock->scale += 0.10f; pDock->ReloadBitmaps(); SaveAllConfigs(); pDock->Render(); }
            }
            else if (id == 402) {
                if (pDock->scale > 0.85f) { pDock->scale -= 0.10f; pDock->ReloadBitmaps(); SaveAllConfigs(); pDock->Render(); }
            }
            else if (id == 403) {
                pDock->scale = 1.0f; pDock->ReloadBitmaps(); SaveAllConfigs(); pDock->Render();
            }
            else if (id == 411) {
                if (pDock->iconSize < 54) { pDock->iconSize += 4; pDock->ReloadBitmaps(); SaveAllConfigs(); pDock->Render(); }
            }
            else if (id == 412) {
                if (pDock->iconSize > 28) { pDock->iconSize -= 4; pDock->ReloadBitmaps(); SaveAllConfigs(); pDock->Render(); }
            }
        }
        break;
    }
    case WM_DESTROY: {
        break;
    }
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateNewDock(int px, int py, Direction d) {
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    DockWindow* dock = new DockWindow(px, py, d);

    dock->hWnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        L"WidgetLauncherCppClass", L"WidgetLauncher",
        WS_POPUP,
        dock->x, dock->y, 100, 100,
        NULL, NULL, hInstance, NULL
    );

    SetWindowLongPtr(dock->hWnd, GWLP_USERDATA, (LONG_PTR)dock);
    g_docks.push_back(dock);

    ShowWindow(dock->hWnd, SW_SHOWNOACTIVATE);
    dock->Render();
    SaveAllConfigs();
}

void LoadAllConfigs() {
    std::string path = WideToUtf8(GetConfigPath());
    std::ifstream in(path);
    if (!in.is_open()) return;

    std::string line;
    DockWindow* curDock = nullptr;
    int appIdx = 0;

    while (std::getline(in, line)) {
        if (line.rfind("GlobalLang=", 0) == 0) {
            g_currentLang = (Language)std::stoi(line.substr(11));
        }
        else if (line.rfind("[Dock]", 0) == 0) {
            curDock = new DockWindow(140, 140, Direction::Down);
            g_docks.push_back(curDock);
            appIdx = 0;
        }
        else if (curDock) {
            if (line.rfind("X=", 0) == 0) curDock->x = std::stoi(line.substr(2));
            else if (line.rfind("Y=", 0) == 0) curDock->y = std::stoi(line.substr(2));
            else if (line.rfind("Dir=", 0) == 0) curDock->dir = (Direction)std::stoi(line.substr(4));
            else if (line.rfind("IconSize=", 0) == 0) curDock->iconSize = std::stoi(line.substr(9));
            else if (line.rfind("Scale=", 0) == 0) curDock->scale = std::stof(line.substr(6));
            else if (line.rfind("Topmost=", 0) == 0) curDock->topmost = (std::stoi(line.substr(8)) == 1);
            else if (line.rfind("Always=", 0) == 0) curDock->alwaysVisible = (std::stoi(line.substr(7)) == 1);
            else if (line.rfind("App=", 0) == 0) {
                std::string data = line.substr(4);
                size_t sep = data.find('|');
                if (sep != std::string::npos) {
                    AppItem item;
                    item.path = U8(data.substr(0, sep).c_str());
                    item.displayName = U8(data.substr(sep + 1).c_str());
                    curDock->LoadIcon(item, appIdx++);
                    curDock->apps.push_back(item);
                }
            }
        }
    }
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    FreeConsole();

    HRESULT hrOle = OleInitialize(NULL);
    if (FAILED(hrOle)) {
        return 0;
    }

    EnableWindowDarkMenu();

    GdiplusStartupInput gdiInput;
    GdiplusStartup(&g_gdiplusToken, &gdiInput, NULL);

    g_hAppIcon = LoadAppIcon(hInstance);

    WNDCLASSEXW wc = { sizeof(wc) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"WidgetLauncherCppClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = g_hAppIcon;
    wc.hIconSm = g_hAppIcon;
    RegisterClassExW(&wc);

    WNDCLASSEXW wcTip = { sizeof(wcTip) };
    wcTip.lpfnWndProc = DefWindowProcW;
    wcTip.hInstance = hInstance;
    wcTip.lpszClassName = L"WidgetLauncherTooltipClass";
    RegisterClassExW(&wcTip);

    g_hTooltipWnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        wcTip.lpszClassName, L"WidgetTooltip",
        WS_POPUP, 0, 0, 10, 10, NULL, NULL, hInstance, NULL
    );

    LoadAllConfigs();
    if (g_docks.empty()) {
        g_docks.push_back(new DockWindow(140, 140, Direction::Down));
    }

    for (auto* dock : g_docks) {
        dock->hWnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TOOLWINDOW,
            L"WidgetLauncherCppClass", L"WidgetLauncher",
            WS_POPUP,
            dock->x, dock->y, 100, 100,
            NULL, NULL, hInstance, NULL
        );
        SetWindowLongPtr(dock->hWnd, GWLP_USERDATA, (LONG_PTR)dock);
        ShowWindow(dock->hWnd, SW_SHOWNOACTIVATE);
        dock->Render();
    }

    g_nid.cbSize = sizeof(g_nid);
    g_nid.hWnd = g_docks[0]->hWnd;
    g_nid.uID = 2026;
    g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_nid.uCallbackMessage = WM_USER + 1;
    g_nid.hIcon = g_hAppIcon;
    wcscpy_s(g_nid.szTip, L"Widget Launcher");
    Shell_NotifyIconW(NIM_ADD, &g_nid);

    timeBeginPeriod(1);
    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    timeEndPeriod(1);
    Shell_NotifyIconW(NIM_DELETE, &g_nid);
    if (g_hAppIcon) DestroyIcon(g_hAppIcon);
    if (g_hTooltipWnd) DestroyWindow(g_hTooltipWnd);

    GdiplusShutdown(g_gdiplusToken);
    OleUninitialize();
    return 0;
}

int main() {
    return wWinMain(GetModuleHandleW(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}