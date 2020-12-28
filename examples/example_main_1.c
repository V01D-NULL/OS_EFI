// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev
// The UEFI is a specifications that comes from this website
//          https://uefi.org/
// I am not part of, nor endorsed by the UEFI or it's partners.

#include "../efi/ErrorCodes.h"
#include "../efi/efi.h"

// This example shows Yellow Text and then waits for keyboard input

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    // This line clears the screen.
    SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
    
    // This line sets the text color to Yellow
    // EFI_YELLOW is listed in the efi.h file
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_YELLOW);
    
    // This line prints the text
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello World\r\n\r\nHit any key");
    
    // This line flushes the Keyboard
    SystemTable->ConIn->Reset(SystemTable->ConIn, 1);

    // This line setups up a variable to receive keyboard input.
    EFI_INPUT_KEY Key;
    
    // This line waits for a key on the keyboard to be pushed.
    // EFI_NOT_READY is listed in the ErrorCodes.h file
	while((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
    
	return EFI_SUCCESS;
}
