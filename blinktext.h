//blinktext.h
#define WINVER 0x0501
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <process.h>
#include <string>
#include <iostream>
using namespace std;
struct Blink
{
    string Text;
    int x;
    int y;
    COLORREF TextColor;
    COLORREF BackColor;
};
unsigned __stdcall BlinkLoop(void *params)
{
    Blink *p = static_cast<Blink *>(params);
    int tlen = (int)p->Text.length();
    HDC hDC = GetDC(GetConsoleWindow());
    HBRUSH hb = CreateSolidBrush(p->BackColor);
    LOGFONT lf;
    GetObject(GetStockObject(OEM_FIXED_FONT), sizeof(LOGFONT), &lf);
    lf.lfWeight = FW_REGULAR;
    HFONT hf = CreateFont(lf.lfHeight, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
    SelectObject(hDC, (HGDIOBJ)(HFONT)hf);
    SetTextColor(hDC, p->TextColor);
    SetBkMode(hDC, TRANSPARENT);
    TEXTMETRIC tm;
    GetTextMetrics(hDC, &tm);
    RECT rt;
    rt.left = p->x * tm.tmAveCharWidth;
    rt.top = p->y * tm.tmHeight;
    rt.right = rt.left + tlen * tm.tmAveCharWidth;
    rt.bottom = rt.top + tm.tmHeight;
    while (true)
    {
        TextOut(hDC, rt.left, rt.top, p->Text.c_str(), tlen+1);//readers if the text isn't showed take of the '+1'
                Sleep(500);
        FillRect(hDC, &rt, hb);
                Sleep(500);
    }
    return 0;
}
void TextBlink(const string& Text, int x, int y, COLORREF TextColor, COLORREF BackColor)
{
    Blink *b = new Blink;
    b->Text = Text;
    b->BackColor = BackColor;
    b->TextColor = TextColor;
    b->x=x;
    b->y =y;
    _beginthreadex(NULL, 0, BlinkLoop, b, 0, NULL);
}
