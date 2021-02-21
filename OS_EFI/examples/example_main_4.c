// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev
// The UEFI is a specifications that comes from this website
//          https://uefi.org/
// I am not part of, nor endorsed by the UEFI or it's partners.

#include "../efi/ErrorCodes.h"
#include "../efi/efi.h"
#include "../lib/lib.h" // This is for the NULL and Int / Uint to String functions

// This example enables PS/2 Mouse ( works with USB 1.1 and USB 2.0 )
// Even though this will show up in Virtualbox,
// it will not interact with the mouse without USB 3.0 support.

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

void PrintInt(long int n, UINTN Attribute)
{
    UINT16 GOPINFO[12];
    itoa(n, GOPINFO);
    Print(GOPINFO, Attribute);
}

void PrintUInt(unsigned long int n, int bn, UINTN Attribute)
{
    UINT16 GOPINFO[12];
    uitoa(n, GOPINFO, bn);
    if(bn == HEX)
    {
        Print(L"0x", Attribute);
    }
    Print(GOPINFO, Attribute);
}

typedef struct PIXELPOSITIONS
{
    UINT32 PixelxPos;
    UINT32 PixelyPos;
} PIXELPOSITIONS;

PIXELPOSITIONS* pixelpos;
PIXELPOSITIONS* GlobalMousePosition;
UINT32 GlobalMouseLastPosition[498];

void SetMouseCursor()
{
    UINT32 MouseCursor[166] = {1,1,1,3,1,2,2,1,3,1,2,2,2,1,3,1,2,2,2,2,1,3,1,2,2,2,2,2,1,3,1,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,2,2,2,2,1,3,1,2,2,2,2,2,2,2,2,2,2,2,2,1,3,1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,3,0,0,0,0,1,2,2,2,2,1,3,0,0,0,0,0,1,2,2,2,2,1,3,0,0,0,0,0,0,1,1,1,1,1,1,4};
    
    if(GlobalMousePosition->PixelxPos < 0){GlobalMousePosition->PixelxPos = 0;}
    if(GlobalMousePosition->PixelyPos < 0){GlobalMousePosition->PixelyPos = 0;}
    UINT32 mcX = GlobalMousePosition->PixelxPos * 4;
    UINT32 mcY = GlobalMousePosition->PixelyPos;
    UINT32 MouseIndex;
    UINT32 GlobalMouseIndexNumber = 0;
    UINT32 MouseSize = sizeof(MouseCursor)/sizeof(MouseCursor[0]);
    
    for(MouseIndex = 0; MouseIndex < MouseSize; MouseIndex++)
    {
        UINT32 MouseColor = MouseCursor[MouseIndex];
        switch(MouseColor)
        {
            case 0:
            {
                mcX+=4;
                break;
            }
            case 1:
            {
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = mcX;
                GlobalMouseIndexNumber++;
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = mcY;
                GlobalMouseIndexNumber++;
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = *(UINT32*)(mcX + (mcY * fBuffer.PixelsPerScanLine * 4) + fBuffer.BaseAddress);
                GlobalMouseIndexNumber++;
                *(UINT32*)(mcX + (mcY * fBuffer.PixelsPerScanLine * 4) + fBuffer.BaseAddress) = BLACK;
                mcX+=4;
                break;
            }
            case 2:
            {
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = mcX;
                GlobalMouseIndexNumber++;
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = mcY;
                GlobalMouseIndexNumber++;
                GlobalMouseLastPosition[GlobalMouseIndexNumber] = *(UINT32*)(mcX + (mcY * fBuffer.PixelsPerScanLine * 4) + fBuffer.BaseAddress);
                GlobalMouseIndexNumber++;
                *(UINT32*)(mcX + (mcY * fBuffer.PixelsPerScanLine * 4) + fBuffer.BaseAddress) = WHITE;
                mcX+=4;
                break;
            }
            case 3:
            {
                mcX = GlobalMousePosition->PixelxPos * 4;
                mcY++;
                break;
            }
            case 4:
            {
                break;
            }
        }
    }
}

void RestoreLastMousePosition()
{
    UINT32  GlobalMouseIndexNumber = 0;
    while(GlobalMouseIndexNumber < 498)
    {
        UINT32 mcX = GlobalMouseLastPosition[GlobalMouseIndexNumber];
        GlobalMouseIndexNumber++;
        UINT32 mcY = GlobalMouseLastPosition[GlobalMouseIndexNumber];
        GlobalMouseIndexNumber++;
        *(UINT32*)(mcX + (mcY * fBuffer.PixelsPerScanLine * 4) + fBuffer.BaseAddress) = GlobalMouseLastPosition[GlobalMouseIndexNumber];
        GlobalMouseIndexNumber++;
    }
}

void MoveMouseCursor(UINT32 xPos, UINT32 yPos)
{
    GlobalMousePosition->PixelxPos = xPos;
    GlobalMousePosition->PixelyPos = yPos;
    RestoreLastMousePosition();
    SetMouseCursor();
}

EFI_SIMPLE_POINTER_PROTOCOL     *Mouse;
EFI_SIMPLE_POINTER_STATE	    CurrentMouseState;
void PreInitializeMouse()
{
    Print(L"Initializing Mouse Pointer.....\r\n", EFI_YELLOW);
	EFI_STATUS	Status;

 	Status = SystemTable->BootServices->LocateProtocol(&EFI_SIMPLE_POINTER_PROTOCOL_GUID, NULL, (void**)&Mouse);
    Print(L"EFI_SIMPLE_POINTER_PROTOCOL_GUID", EFI_YELLOW);
    Print(CheckStandardEFIError(Status), EFI_WHITE);
    
	Status = Mouse->Reset(Mouse, 1);
    Print(L"Mouse Reset", EFI_YELLOW);
    Print(CheckStandardEFIError(Status), EFI_WHITE);
    Print(L"\r\n", EFI_WHITE);
}

void InitializeMouse()
{
    GlobalMousePosition->PixelxPos = 0;
    GlobalMousePosition->PixelyPos = 0;
    SetMouseCursor();
    MoveMouseCursor(200, 50);
}

void UpdateMouseCursor(INT32 xPos, INT32 yPos)
{
    INT32 tempX = GlobalMousePosition->PixelxPos + xPos;
    INT32 tempY = GlobalMousePosition->PixelyPos + yPos;
    
    if(tempX < 0)
    {
        GlobalMousePosition->PixelxPos = 0;
    } else if(GlobalMousePosition->PixelxPos >= fBuffer.Width){
        GlobalMousePosition->PixelxPos = fBuffer.Width - 4;
    } else {
        GlobalMousePosition->PixelxPos = GlobalMousePosition->PixelxPos + xPos;
    }
    
    if(tempY < 0)
    {
        GlobalMousePosition->PixelyPos = 0;
    } else if(GlobalMousePosition->PixelyPos >= fBuffer.Height){
        GlobalMousePosition->PixelyPos = fBuffer.Height - 4;
    } else {
        GlobalMousePosition->PixelyPos = GlobalMousePosition->PixelyPos + yPos;
    }
    
    MoveMouseCursor(GlobalMousePosition->PixelxPos, GlobalMousePosition->PixelyPos);
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST)
{
    SystemTable = ST;
    
    ClearScreen();
    
    if(InitializeGraphicsOutputProtocol() != NULL)
    {
        PreInitializeMouse();
        InitializeMouse();
        
        EFI_STATUS Status;
        INT32		                mX,mY;

        while (1)
        {
            Status = SystemTable->BootServices->CheckEvent(Mouse->WaitForInput);
            if(Status != EFI_NOT_READY)
            {
                mX = (INT32) (( CurrentMouseState.RelativeMovementX / Mouse->Mode->ResolutionX) * 3);
                mY = (INT32) ((CurrentMouseState.RelativeMovementY / Mouse->Mode->ResolutionY) * 3);

                UpdateMouseCursor(mX, mY);
                SetTextPosition(8, 7);
                PrintUInt(GlobalMousePosition->PixelxPos, DECIMAL, EFI_GREEN);
                Print(L" : ", EFI_WHITE);
                PrintUInt(GlobalMousePosition->PixelyPos, DECIMAL, EFI_GREEN);
                
                Mouse->GetState(Mouse, &CurrentMouseState);
                if(CurrentMouseState.LeftButton == 1)
                {
                    SetTextPosition(8, 8);
                    Print(L"Left Button Down ", EFI_WHITE);
                }
                if(CurrentMouseState.LeftButton == 0)
                {
                    SetTextPosition(8, 8);
                    Print(L"Left Button Up   ", EFI_WHITE);
                }
                
                if(CurrentMouseState.RightButton == 1)
                {
                    SetTextPosition(8, 9);
                    Print(L"Right Button Down", EFI_WHITE);
                }
                if(CurrentMouseState.RightButton == 0)
                {
                    SetTextPosition(8, 9);
                    Print(L"Right Button Up  ", EFI_WHITE);
                }
            }
        }
    }

    WaitForKey();
    
	return EFI_SUCCESS;
}
