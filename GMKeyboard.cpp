#include <Windows.h>
#include <ctype.h>
#include "GMKeyboard.h"

static const BYTE g_abLetterScanCode[26] = {
    0x1E, // A
    0x30, // B
    0x2E, // C
    0x20, // D
    0x12, // E
    0x21, // F
    0x22, // G
    0x23, // H
    0x17, // I
    0x24, // J
    0x25, // K
    0x26, // L
    0x32, // M
    0x31, // N
    0x18, // O
    0x19, // P
    0x10, // Q
    0x13, // R
    0x1F, // S
    0x14, // T
    0x16, // U
    0x2F, // V
    0x11, // W
    0x2D, // X
    0x15, // Y
    0x2C, // Z
};

CHAR ScanCodeToUSLetter(UINT uScanCode)
{
    for (int i = 0; i < 26; ++i)
    {
        if (g_abLetterScanCode[i] == uScanCode)
        {
            return (CHAR)('A' + i);
        }
    }

    return 0;
}

extern "C" GMKEYBOARD_API double GMKeyboard_ToUserLayout(double dInput)
{
    INT iUpper = toupper((INT)dInput);
    if (iUpper < 'A' || iUpper > 'Z')
    {
        // Not a letter, pass unchanged.
        return dInput;
    }

    UINT uScanCode = g_abLetterScanCode[iUpper - 'A'];
    HKL hLayout = GetKeyboardLayout(NULL);

    // Map the scancode to the virtual key.
    UINT uVk = MapVirtualKeyEx(uScanCode, MAPVK_VSC_TO_VK, hLayout);
    if (uVk == 0)
    {
        return dInput;
    }

    // Turn that virtual key into ASCII character.
    BYTE abKeyState[256] = { 0 };
    WCHAR awszBuffer[8] = { 0 };

    if (ToUnicodeEx(uVk, uScanCode, abKeyState, awszBuffer, 8, 0, hLayout) <= 0)
    {
        // This is a dead key (no mapping on the user's layout) or error.
        return dInput;
    }

    // Translate the wide char to narrow.
    CHAR acOutput[1] = { 0 };
    if (WideCharToMultiByte(CP_UTF8, 0, awszBuffer, 8, acOutput, 1, NULL, NULL) == 1)
    {
        // Good conversion!
        return (double)toupper(acOutput[0]);
    }
    else
    {
        return dInput;
    }
}

extern "C" GMKEYBOARD_API double GMKeyboard_ToPhysicalLayout(double dInput)
{
    CHAR acUpper[1] = { toupper((int)dInput) };
    HKL hLayout = GetKeyboardLayout(NULL);

    WCHAR awcUpper[1] = { 0 };
    if (MultiByteToWideChar(CP_UTF8, 0, acUpper, 1, awcUpper, 1) != 1)
    {
        // Conversion failed, probably invalid character.
        return dInput;
    }

    SHORT sVkAndMask = VkKeyScanEx(awcUpper[0], hLayout);
    if (sVkAndMask == -1)
    {
        // Not a key this layout can do.
        return dInput;
    }

    UINT uVk = LOBYTE(sVkAndMask);
    UINT uScanCode = MapVirtualKeyEx(uVk, MAPVK_VK_TO_VSC, hLayout);
    if (uScanCode == 0)
    {
        return dInput;
    }

    CHAR cOutput = ScanCodeToUSLetter(uScanCode);
    if (cOutput == 0)
    {
        // Did not find in the array.
        return dInput;
    }

    return (double)cOutput;
}
