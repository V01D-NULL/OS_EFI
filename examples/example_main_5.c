// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev
// The UEFI is a specifications that comes from this website
//          https://uefi.org/
// I am not part of, nor endorsed by the UEFI or it's partners.

#include "../efi/ErrorCodes.h"
#include "../efi/efi.h"
#include "../lib/lib.h" // This is for the NULL and Int / Uint to String functions

// This example shows how to load a file into memory and get a return number.

EFI_SYSTEM_TABLE    *SystemTable;

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

void PrintInt(long int n, UINTN Attribute)
{
    UINT16 GOPINFO[12];
    itoa(n, GOPINFO);
    Print(GOPINFO, Attribute);
}

// Original Idea from Poncho Series
EFI_FILE_PROTOCOL* LoadFile(CHAR16* Path, EFI_HANDLE ImageHandle)
{
    // Beyond BIO Page 57
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_STATUS Status = SystemTable->BootServices->HandleProtocol(ImageHandle, &EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&LoadedImage);
    Print(L"LoadedImage", EFI_WHITE);
    Print(CheckStandardEFIError(Status), EFI_CYAN);

    // Beyond BIO Page 57
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
    Status = SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &EFI_DEVICE_PATH_PROTOCOL_GUID, (void**)&DevicePath);
    Print(L"DevicePath", EFI_WHITE);
    Print(CheckStandardEFIError(Status), EFI_CYAN);

    // Beyond BIO Page 58
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume;
    Status = SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (void**)&Volume);
    Print(L"Volume", EFI_WHITE);
    Print(CheckStandardEFIError(Status), EFI_CYAN);
    
    // Beyond BIO Page 58
    EFI_FILE_PROTOCOL* RootFS;
    Status = Volume->OpenVolume(Volume, &RootFS);
    Print(L"RootFS", EFI_WHITE);
    Print(CheckStandardEFIError(Status), EFI_CYAN);
    
    EFI_FILE_PROTOCOL* FileHandle;
    Status = RootFS->Open(RootFS, &FileHandle, Path, EFI_FILE_MODE_READ, 0);
    Print(L"kernel.bin", EFI_WHITE);
    Print(CheckStandardEFIError(Status), EFI_CYAN);
	if (Status != EFI_SUCCESS)
    {
		return NULL;
	}
	return FileHandle;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST)
{
    SystemTable = ST;
    
    ClearScreen();
   
    EFI_FILE_PROTOCOL* kernel = LoadFile(L"ourOS\\kernel.bin", ImageHandle);
    
    UINT64 fsize           = 0x00100000;
    void*                  OSKernelBuffer;
    
    SystemTable->BootServices->AllocatePool(EfiLoaderData, fsize, (void**)&OSKernelBuffer);
    Print(L"AllocatePool OSKernelBuffer\r\n", EFI_YELLOW);
    
    kernel->Read(kernel, &fsize, OSKernelBuffer);
    Print(L"Read OSKernelBuffer\r\n", EFI_YELLOW);
    
    kernel->Close(kernel);
    Print(L"Close OSKernelBuffer\r\n\r\n", EFI_YELLOW);
    
    UINT8* test = (UINT8*)OSKernelBuffer;

    int (*KernelBinFile)() = ((__attribute__((ms_abi)) int (*)() ) &test[256]);

    Print(L"The returned number is : ", EFI_YELLOW);
    
    PrintInt(KernelBinFile(), EFI_WHITE);
    
    Print(L"\r\n\r\nHit Any Key", EFI_LIGHTGRAY);
    
    WaitForKey();
    
	return EFI_SUCCESS;
}
