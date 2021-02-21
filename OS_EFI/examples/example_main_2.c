// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev
// The UEFI is a specifications that comes from this website
//          https://uefi.org/
// I am not part of, nor endorsed by the UEFI or it's partners.

#include "../efi/ErrorCodes.h"
#include "../efi/efi.h"

// This example shows Text in a specified position
// This also simplifies functions that can be re-used.
// ClearScreen() | Print() | WaitForKey() | SetTextPosition()

// This sets the global variable that the whole project will use
EFI_SYSTEM_TABLE *SystemTable;

void ClearScreen()
{
    // This function clears the screen.
    SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
}

void Print(CHAR16* str, UINTN Attribute)
{
    // This function sets the text with the color
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, Attribute);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
}

void WaitForKey()
{
    // This function waits for keyboard input.
    EFI_INPUT_KEY Key;
	SystemTable->ConIn->Reset(SystemTable->ConIn, 1);
	while((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
}

void SetTextPosition(UINT32 Col, UINT32 Row)
{
    // This function sets the console text position
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST)
{
    SystemTable = ST;
    
    ClearScreen();
    
    Print(L"Hello World\r\n\r\n", EFI_CYAN);
    
    SetTextPosition(5, 8);
    
    Print(L"This Text Position has been set.\r\n", EFI_BACKGROUND_BLUE | EFI_WHITE);

    WaitForKey();
    
	return EFI_SUCCESS;
}
