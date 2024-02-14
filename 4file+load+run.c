#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <Library/FileHandleLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>  // Added header for StrnCpyS

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume;
    EFI_FILE_PROTOCOL *RootFs;
    EFI_FILE_PROTOCOL *File;
    UINTN FileSize;
    VOID* FileBuffer;

    EFI_HANDLE NewImageHandle1;
    EFI_HANDLE NewImageHandle2;
    EFI_HANDLE NewImageHandle3;
    EFI_HANDLE NewImageHandle4;

    EFI_DEVICE_PATH *DevicePath1;
    EFI_DEVICE_PATH *DevicePath2;
    EFI_DEVICE_PATH *DevicePath3;
    EFI_DEVICE_PATH *DevicePath4;

    // Print "Hello, x!"
    Print(L"Hello, x!\n");

    // Get the handle and protocol for the loaded image
    Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get the loaded image handle: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Open the root file system
    Status = gBS->OpenProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid,
                               (VOID**)&Volume, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the file system protocol: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Open the root directory
    Status = Volume->OpenVolume(Volume, &RootFs);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the root directory: %r\n", Status);
        return Status;
    }

    // Open the "hello-y.efi" file
    Status = RootFs->Open(RootFs, &File, L"\\EFI\\boot\\hello-y.efi",
                          EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the hello-y.efi file: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Get the size of the "hello-y.efi" file
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 255;
    Status = gBS->AllocatePool(EfiLoaderData, FileInfoSize, (VOID**)&FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate memory for file info: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file info: %r\n", Status);
        gBS->FreePool(FileInfo);
        return Status;
    }

    FileSize = FileInfo->FileSize;
    gBS->FreePool(FileInfo);

    // Allocate memory for the "hello-y.efi" file
    Status = gBS->AllocatePool(EfiLoaderData, FileSize, (VOID**)&FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate memory for the file buffer: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Read the "hello-y.efi" file into memory
    Status = File->Read(File, &FileSize, FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read the hello-y.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Close the "hello-y.efi" file
    File->Close(File);

    // Load the "hello-y.efi" file from memory as a new image
    Status = gBS->LoadImage(TRUE, ImageHandle, NULL, FileBuffer, FileSize, &NewImageHandle1);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to load the hello-y.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Retrieve the device path of the loaded image
    DevicePath1 = FileDevicePath(LoadedImage->DeviceHandle, L"\\EFI\\boot\\hello-y.efi");
    
// Start the "hello-y.efi" image
    Status = gBS->StartImage(NewImageHandle1, NULL, NULL);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to start hello-y.efi: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }
//################################################################################
    // Open the "hello-x.efi" file
    Status = RootFs->Open(RootFs, &File, L"\\EFI\\boot\\hello-x.efi",
                          EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the hello-x.efi file: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Get the size of the "hello-x.efi" file
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file info: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileInfo);
        return Status;
    }

    FileSize = FileInfo->FileSize;
    gBS->FreePool(FileInfo);

    // Allocate memory for the "hello-x.efi" file
    Status = gBS->AllocatePool(EfiLoaderData, FileSize, (VOID**)&FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate memory for the file buffer: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Read the "hello-x.efi" file into memory
    Status = File->Read(File, &FileSize, FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read the hello-x.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Close the "hello-x.efi" file
    File->Close(File);

   








// Convert the search pattern to an array of UINT8 values
UINT8 searchPattern1[] = { 0x4D, 0x00, 0x41, 0x00, 0x48, 0x00, 0x41, 0x00, 0x56, 0x00, 0x45, 0x00, 0x45, 0x00, 0x52 };
// Convert the replacement pattern to an array of UINT8 values
UINT8 replacePattern1[] = { 0x41, 0x00, 0x53, 0x00, 0x44, 0x00, 0x46, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34 };

// Calculate the length of the search pattern
UINTN searchPatternSize1 = sizeof(searchPattern1) / sizeof(searchPattern1[0]);
// Calculate the length of the replace pattern
UINTN replacePatternSize1 = sizeof(replacePattern1) / sizeof(replacePattern1[0]);

// Search for the pattern in the FileBuffer and replace it
BOOLEAN patternReplaced1 = FALSE;
UINTN i1;
for (i1 = 0; i1 < FileSize - searchPatternSize1 + 1; i1++) {
    BOOLEAN patternMatched1 = TRUE;

    for (UINTN j = 0; j < searchPatternSize1; j++) {
        if (((UINT8*)FileBuffer)[i1 + j] != searchPattern1[j]) {
            patternMatched1 = FALSE;
            break;
        }
    }

    if (patternMatched1) {
        patternReplaced1 = TRUE;
        Print(L"Pattern found at offset %u\n", i1);

        // Print the content at the offset before replacement
        Print(L"Content at offset %u before replacement: ", i1);
        for (UINTN j = 0; j < searchPatternSize1; j++) {
            Print(L"%02X ", ((UINT8*)FileBuffer)[i1 + j]);
        }
        Print(L"\n");

        // Replace the pattern with the replace pattern
        for (UINTN j = 0; j < replacePatternSize1; j++) {
            ((UINT8*)FileBuffer)[i1 + j] = replacePattern1[j];
        }
        break;  // Exit the loop after the first occurrence
    }
}

if (patternReplaced1) {
    Print(L"Pattern replaced successfully.\n");

    // Print the replaced content
    Print(L"Content at offset %u after replacement: ", i1);
    for (UINTN j = 0; j < replacePatternSize1; j++) {
        Print(L"%02X ", ((UINT8*)FileBuffer)[i1 + j]);
    }
    Print(L"\n");
} else {
    Print(L"Pattern not found in the file.\n");
}

/*
Print(L"Hex bytes and ASCII representation of hello-x.efi:\n");
UINT8* ImageBytes1 = (UINT8*)FileBuffer;
for (UINTN i1 = 0; i1 < FileSize; i1++) {
    Print(L"%02x ", ImageBytes1[i1]);
    if ((i1 + 1) % 16 == 0) {
        // Print ASCII representation
        Print(L"   ");
        for (UINTN j = i1 - 15; j <= i1; j++) {
            if (ImageBytes1[j] >= 32 && ImageBytes1[j] <= 126) {
                // Printable ASCII character
                Print(L"%c", ImageBytes1[j]);
            } else {
                // Non-printable ASCII character, print a dot instead
                Print(L".");
            }
        }
        Print(L"\n");
    }
}

// Print remaining bytes and their ASCII representation if any
if (FileSize % 16 != 0) {
    UINTN remainingBytes1 = FileSize % 16;
    UINTN lastLineStartIndex1 = FileSize - remainingBytes1;

    // Print padding spaces for alignment
    UINTN paddingSpaces1 = 3 * (16 - remainingBytes1);
    for (UINTN j = 0; j < paddingSpaces1; j++) {
        Print(L" ");
    }

    // Print ASCII representation
    for (UINTN j = lastLineStartIndex1; j < FileSize; j++) {
        if (ImageBytes1[j] >= 32 && ImageBytes1[j] <= 126) {
            // Printable ASCII character
            Print(L"%c", ImageBytes1[j]);
        } else {
            // Non-printable ASCII character, print a dot instead
            Print(L".");
        }
    }
    Print(L"\n");
}

*/





 // Load the "hello-x.efi" file from memory as a new image
    Status = gBS->LoadImage(TRUE, ImageHandle, NULL, FileBuffer, FileSize, &NewImageHandle2);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to load the hello-x.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Retrieve the device path of the loaded image
    DevicePath2 = FileDevicePath(LoadedImage->DeviceHandle, L"\\EFI\\boot\\hello-x.efi");

Print(L"Address of hello-x.efi: %p\n", NewImageHandle2);
gBS->Stall(5000000);


   // Start the "hello-x.efi" image
    Status = gBS->StartImage(NewImageHandle2, NULL, NULL);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to start hello-x.efi: %r\n", Status);
gBS->Stall(5000000);

        gBS->FreePool(FileBuffer);
        return Status;
    }


//################################################################################
    // Open the "hello.efi" file
    Status = RootFs->Open(RootFs, &File, L"\\EFI\\boot\\hello.efi",
                          EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the hello.efi file: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Get the size of the "hello.efi" file
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file info: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileInfo);
        return Status;
    }

    FileSize = FileInfo->FileSize;
    gBS->FreePool(FileInfo);

    // Allocate memory for the "hello.efi" file
    Status = gBS->AllocatePool(EfiLoaderData, FileSize, (VOID**)&FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate memory for the file buffer: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Read the "hello.efi" file into memory
    Status = File->Read(File, &FileSize, FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read the hello.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Close the "hello.efi" file
 File->Close(File);


//*********************************
// Convert the search pattern to an array of UINT8 values
UINT8 searchPattern2[] = { 0x49, 0x00, 0x6E, 0x00, 0x73, 0x00, 0x74, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x69 };
// Convert the replacement pattern to an array of UINT8 values
UINT8 replacePattern2[] = { 0x41, 0x00, 0x53, 0x00, 0x44, 0x00, 0x46, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34 };

// Calculate the length of the search pattern
UINTN searchPatternSize2 = sizeof(searchPattern2) / sizeof(searchPattern2[0]);
// Calculate the length of the replace pattern
UINTN replacePatternSize2 = sizeof(replacePattern2) / sizeof(replacePattern2[0]);

// Search for the pattern in the FileBuffer and replace it
BOOLEAN patternReplaced2 = FALSE;
UINTN i2;
for (i2 = 0; i2 < FileSize - searchPatternSize2 + 1; i2++) {
    BOOLEAN patternMatched2 = TRUE;

    for (UINTN j = 0; j < searchPatternSize2; j++) {
        if (((UINT8*)FileBuffer)[i2 + j] != searchPattern2[j]) {
            patternMatched2 = FALSE;
            break;
        }
    }

    if (patternMatched2) {
        patternReplaced2 = TRUE;
        Print(L"Pattern found at offset %u\n", i2);

        // Print the content at the offset before replacement
        Print(L"Content at offset %u before replacement: ", i2);
        for (UINTN j = 0; j < searchPatternSize2; j++) {
            Print(L"%02X ", ((UINT8*)FileBuffer)[i2 + j]);
        }
        Print(L"\n");

        // Replace the pattern with the replace pattern
        for (UINTN j = 0; j < replacePatternSize2; j++) {
            ((UINT8*)FileBuffer)[i2 + j] = replacePattern2[j];
        }
        break;  // Exit the loop after the first occurrence
    }
}

if (patternReplaced2) {
    Print(L"Pattern replaced successfully.\n");

    // Print the replaced content
    Print(L"Content at offset %u after replacement: ", i2);
    for (UINTN j = 0; j < replacePatternSize2; j++) {
        Print(L"%02X ", ((UINT8*)FileBuffer)[i2 + j]);
    }
    Print(L"\n");
} else {
    Print(L"Pattern not found in the file.\n");
}
/*

Print(L"Hex bytes and ASCII representation of hello-x.efi:\n");
UINT8* ImageBytes2 = (UINT8*)FileBuffer;
for (UINTN i2 = 0; i2 < FileSize; i2++) {
    Print(L"%02x ", ImageBytes2[i2]);
    if ((i2 + 1) % 16 == 0) {
        // Print ASCII representation
        Print(L"   ");
        for (UINTN j = i2 - 15; j <= i2; j++) {
            if (ImageBytes2[j] >= 32 && ImageBytes2[j] <= 126) {
                // Printable ASCII character
                Print(L"%c", ImageBytes2[j]);
            } else {
                // Non-printable ASCII character, print a dot instead
                Print(L".");
            }
        }
        Print(L"\n");
    }
}

// Print remaining bytes and their ASCII representation if any
if (FileSize % 16 != 0) {
    UINTN remainingBytes1 = FileSize % 16;
    UINTN lastLineStartIndex1 = FileSize - remainingBytes1;

    // Print padding spaces for alignment
    UINTN paddingSpaces1 = 3 * (16 - remainingBytes1);
    for (UINTN j = 0; j < paddingSpaces1; j++) {
        Print(L" ");
    }

    // Print ASCII representation
    for (UINTN j = lastLineStartIndex1; j < FileSize; j++) {
        if (ImageBytes2[j] >= 32 && ImageBytes2[j] <= 126) {
            // Printable ASCII character
            Print(L"%c", ImageBytes2[j]);
        } else {
            // Non-printable ASCII character, print a dot instead
            Print(L".");
        }
    }
    Print(L"\n");
}


*/


 // Load the "hello-x.efi" file from memory as a new image
    Status = gBS->LoadImage(TRUE, ImageHandle, NULL, FileBuffer, FileSize, &NewImageHandle3);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to load the hello.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Retrieve the device path of the loaded image
    DevicePath3 = FileDevicePath(LoadedImage->DeviceHandle, L"\\EFI\\boot\\hello.efi");

Print(L"Address of hello.efi: %p\n", NewImageHandle3);
gBS->Stall(5000000);






gBS->Stall(1000000); 
   // Start the "hello.efi" image
    Status = gBS->StartImage(NewImageHandle3, NULL, NULL);
gBS->Stall(5000000);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to start hello.efi: %r\n", Status);
gBS->Stall(55000000);

        gBS->FreePool(FileBuffer);
        return Status;
    }




//################################################################################
    // Open the "KonWin.efi" file
    Status = RootFs->Open(RootFs, &File, L"\\EFI\\boot\\KonWin.efi",
                          EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open the KonWin.efi file: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Get the size of the "KonWin.efi" file
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file info: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileInfo);
        return Status;
    }

    FileSize = FileInfo->FileSize;
    gBS->FreePool(FileInfo);

    // Allocate memory for the "KonWin.efi" file
    Status = gBS->AllocatePool(EfiLoaderData, FileSize, (VOID**)&FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to allocate memory for the file buffer: %r\n", Status);
gBS->Stall(5000000);
        return Status;
    }

    // Read the "KonWin.efi" file into memory
    Status = File->Read(File, &FileSize, FileBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read the KonWin.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Close the "KonWin.efi" file
  File->Close(File);

 
UINT8 searchPattern3[]  = { 0x4D, 0x00, 0x41, 0x00, 0x48, 0x00, 0x41, 0x00, 0x56, 0x00, 0x45, 0x00, 0x45, 0x00, 0x52 };
UINT8 replacePattern3[] = { 0x41, 0x00, 0x53, 0x00, 0x44, 0x00, 0x46, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34 };

UINTN searchPatternSize3 = sizeof(searchPattern3) / sizeof(searchPattern3[0]);
UINTN replacePatternSize3 = sizeof(replacePattern3) / sizeof(replacePattern3[0]);

// Search for the pattern in the FileBuffer and replace it
BOOLEAN patternReplaced3 = FALSE;
UINTN i3;
for (i3 = 0; i3 < FileSize - searchPatternSize3 + 1; i3++) {
    BOOLEAN patternMatched3 = TRUE;

    for (UINTN j = 0; j < searchPatternSize3; j++) {
        if (((UINT8*)FileBuffer)[i3 + j] != searchPattern3[j]) {
            patternMatched3 = FALSE;
            break;
        }
    }

    if (patternMatched3) {
        patternReplaced3 = TRUE;
        Print(L"Pattern found at offset %u\n", i3);

        // Print the content at the offset before replacement
        Print(L"Content at offset %u before replacement: ", i3);
        for (UINTN j = 0; j < searchPatternSize3; j++) {
            Print(L"%02X ", ((UINT8*)FileBuffer)[i3 + j]);
        }
        Print(L"\n");

        // Replace the pattern with the replace pattern
        for (UINTN j = 0; j < replacePatternSize3; j++) {
            ((UINT8*)FileBuffer)[i3 + j] = replacePattern3[j];
        }
        break;  // Exit the loop after the first occurrence
    }
}

if (patternReplaced3) {
    Print(L"Pattern replaced successfully.\n");

    // Print the replaced content
    Print(L"Content at offset %u after replacement: ", i3);
    for (UINTN j = 0; j < replacePatternSize3; j++) {
        Print(L"%02X ", ((UINT8*)FileBuffer)[i3 + j]);
    }
    Print(L"\n");
} else {
    Print(L"Pattern not found in the file.\n");
}


Print(L"Hex bytes and ASCII representation of hello-x.efi:\n");
UINT8* ImageBytes3 = (UINT8*)FileBuffer;
for (UINTN i3 = 0; i3 < FileSize; i3++) {
    Print(L"%02x ", ImageBytes3[i3]);
    if ((i3 + 1) % 16 == 0) {
        // Print ASCII representation
        Print(L"   ");
        for (UINTN j = i3 - 15; j <= i3; j++) {
            if (ImageBytes3[j] >= 32 && ImageBytes3[j] <= 126) {
                // Printable ASCII character
                Print(L"%c", ImageBytes3[j]);
            } else {
                // Non-printable ASCII character, print a dot instead
                Print(L".");
            }
        }
        Print(L"\n");
    }
}

// Print remaining bytes and their ASCII representation if any
if (FileSize % 16 != 0) {
    UINTN remainingBytes1 = FileSize % 16;
    UINTN lastLineStartIndex1 = FileSize - remainingBytes1;

    // Print padding spaces for alignment
    UINTN paddingSpaces1 = 3 * (16 - remainingBytes1);
    for (UINTN j = 0; j < paddingSpaces1; j++) {
        Print(L" ");
    }

    // Print ASCII representation
    for (UINTN j = lastLineStartIndex1; j < FileSize; j++) {
        if (ImageBytes3[j] >= 32 && ImageBytes3[j] <= 126) {
            // Printable ASCII character
            Print(L"%c", ImageBytes3[j]);
        } else {
            // Non-printable ASCII character, print a dot instead
            Print(L".");
        }
    }
    Print(L"\n");
}



 // Load the "KonWin.efi" file from memory as a new image
    Status = gBS->LoadImage(TRUE, ImageHandle, NULL, FileBuffer, FileSize, &NewImageHandle4);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to load the KonWin.efi file: %r\n", Status);
gBS->Stall(5000000);
        gBS->FreePool(FileBuffer);
        return Status;
    }

    // Retrieve the device path of the loaded image
    DevicePath4 = FileDevicePath(LoadedImage->DeviceHandle, L"\\EFI\\boot\\KonWin.efi");

Print(L"Address of KonWin.efi: %p\n", NewImageHandle4);
gBS->Stall(5000000);
 
   // %%%%%%%%%%%%Start the "KonWin.efi" image
    Status = gBS->StartImage(NewImageHandle4, NULL, NULL);
    
    if (EFI_ERROR(Status)) {
        Print(L"Failed to start KonWin.efi: %r\n", Status);
        gBS->Stall(55000000);

        gBS->FreePool(FileBuffer);
        return Status;
    }



    // Free the allocated memory
    gBS->FreePool(FileBuffer);
    gBS->FreePool(DevicePath1);
    gBS->FreePool(DevicePath2);
    gBS->FreePool(DevicePath3);
    gBS->FreePool(DevicePath4);

    return EFI_SUCCESS;
}

