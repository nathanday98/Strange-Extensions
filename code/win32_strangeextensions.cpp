#include <windows.h>
#include <Wininet.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

static void *
GeneralAlloc(u64 Size)
{
    return VirtualAlloc(0, Size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

static void
GeneralFree(void *Memory)
{
    VirtualFree(Memory, 0, MEM_RELEASE);
}

struct file_data
{
    char *Contents;
    uint32_t ContentSize;
};

#include "strangeextensions_parse.cpp"

static file_data
Win32ReadEntireFile(char *Filename)
{
    file_data Result = {0};

    HANDLE FileHandle = CreateFileA(Filename, 
                                    GENERIC_READ, 
                                    FILE_SHARE_READ, 
                                    0, 
                                    OPEN_EXISTING, 
                                    0, 
                                    0);

    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle,
                         &FileSize))
        {
            u32 FileSize32 = (u32)FileSize.QuadPart;
            Result.Contents = (char *)GeneralAlloc((u64)FileSize32);
            if(Result.Contents)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, 
                            Result.Contents,
                            FileSize32,
                            &BytesRead,
                            0)
                    && (FileSize32 == BytesRead))
                {
                    Result.ContentSize = FileSize32;
                }
                else
                {
                    GeneralFree(Result.Contents);
                    Result.Contents = 0;
                }
            }
        }
    }
    else
    {
        // TODO(nathan): error invalid filename
    }

    CloseHandle(FileHandle);

    return(Result);
};
int 
main(int ArgCount,
     char *Args[])
{    
    HINTERNET InternetHandle = 
        InternetOpen("www.opengl.org",
                     INTERNET_OPEN_TYPE_DIRECT,
                     0,
                     0,
                     0);
    
    if(InternetHandle)
    {
        HINTERNET WebsiteHandle = InternetOpenUrl(InternetHandle,
                                                   "https://www.opengl.org/registry/api/GL/glcorearb.h",
                                                   0, 
                                                   0,
                                                   INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_NO_CACHE_WRITE,
                                                   0);
        if(WebsiteHandle)
        {
            #define  BufferSize 250000
            DWORD SizeOfFile = 0;
            
            char *Buffer = (char *)malloc(sizeof(char) * BufferSize);
            if(InternetReadFile(WebsiteHandle,
                                Buffer,
                                BufferSize,
                                &SizeOfFile))
            {
                HANDLE FileHandle = CreateFileA("corearb.h",
                                                GENERIC_WRITE,
                                                FILE_SHARE_WRITE,
                                                0,
                                                CREATE_NEW,
                                                0,
                                                0);
                
                DWORD BytesWritten;
                WriteFile(FileHandle, Buffer, SizeOfFile, &BytesWritten, 0);                               
                
                free(Buffer);
                CloseHandle(FileHandle);
            }
            else
            {
                // TODO(zak): Logging
            }
            
            
            InternetCloseHandle(WebsiteHandle);
        }
        else
        {
            // TODO(zak): Logging
        }               
        
        InternetCloseHandle(InternetHandle);
    }
    else
    {
        // TODO(zak): Logging
    }

    file_data CoreARBFile = Win32ReadEntireFile("corearb.h");

    if(ArgCount > 1)
    {
        for(int File = 1; 
            File < ArgCount; 
            File++)
        {
            file_data SourceFile = Win32ReadEntireFile(Args[File]);

            LARGE_INTEGER StartClock;
            QueryPerformanceCounter(&StartClock);

            ParseText(SourceFile.Contents, 
                      SourceFile.ContentSize, &CoreARBFile);

            LARGE_INTEGER EndClock;
            QueryPerformanceCounter(&EndClock);

            u64 Time = EndClock.QuadPart - StartClock.QuadPart;
            printf("Took %lld\n", Time);

        }
    }
    else
    {
        printf("No files specified\n");
    }

    return(0);
}
