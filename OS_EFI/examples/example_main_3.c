// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev
// The UEFI is a specifications that comes from this website
//          https://uefi.org/
// I am not part of, nor endorsed by the UEFI or it's partners.

#include "../efi/ErrorCodes.h"
#include "../efi/efi.h"
#include "../lib/lib.h" // This is for the NULL

// This example demonstrates graphics

             // ARGB             
#define ORANGE 0xffffa500
#define CYAN   0xff00ffff
#define RED    0xffff0000
#define GREEN  0xff00ff00
#define BLUE   0xff0000ff
#define GRAY   0xff888888
#define WHITE  0xffffffff
#define BLACK  0xff000000

typedef struct FRAMEBUFFER
{
    void*  BaseAddress;
    UINT64 BufferSize;
    UINT32 Width;
    UINT32 Height;
    UINT32 PixelsPerScanLine;
} FRAMEBUFFER;

FRAMEBUFFER         fBuffer;
EFI_SYSTEM_TABLE    *SystemTable;

// This function idea is from the Poncho Series
FRAMEBUFFER* InitializeGraphicsOutputProtocol()
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS Status = SystemTable->BootServices->LocateProtocol(&EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, NULL, (void**)&gop);
    if(Status != EFI_SUCCESS)
    {
        return NULL;
    }

    fBuffer.BaseAddress        = (void*)gop->Mode->FrameBufferBase;
    fBuffer.BufferSize         = gop->Mode->FrameBufferSize;
    fBuffer.Width              = gop->Mode->Info->HorizontalResolution;
    fBuffer.Height             = gop->Mode->Info->VerticalResolution;
    fBuffer.PixelsPerScanLine  = gop->Mode->Info->PixelsPerScanLine;
    return &fBuffer;
}

void ClearScreen()
{
    // This function clears the screen.
    SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
}

void WaitForKey()
{
    // This function waits for keyboard input.
    EFI_INPUT_KEY Key;
	SystemTable->ConIn->Reset(SystemTable->ConIn, 1);
	while((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
}

void CreateFilledBox(UINT32 xPos, UINT32 yPos, UINT32 w, UINT32 h, UINT32 color)
{
    // This functions puts a color filled box on the screen
    UINT32 BBP = 4;
    if(xPos < 0){xPos = 0;}
    if(yPos < 0){yPos = 0;}
    if(w < 1){w = 1;}
    if(h < 1){h = 1;}
    UINT32 x;
    UINT32 y      = yPos;
    UINT32 width  = (xPos + w) * BBP;
    UINT32 height = yPos + h;

    for(y = yPos; y <= height; y++)
    {
        for(x = xPos * BBP; x <= width; x+=BBP)
        {
            *(UINT32*)(x + (y * fBuffer.PixelsPerScanLine * BBP) + fBuffer.BaseAddress) = color;
        }
    }
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST)
{
    SystemTable = ST;
    
    ClearScreen();
    
    if(InitializeGraphicsOutputProtocol() != NULL)
    {
        CreateFilledBox(50, 50, fBuffer.Width - 100, fBuffer.Height - 100, ORANGE);
        CreateFilledBox(200, 200, 200, 200, BLUE);
        CreateFilledBox(300, 300, 100, 50, CYAN);
    }

    WaitForKey();
    
	return EFI_SUCCESS;
}
