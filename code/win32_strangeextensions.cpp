#include <windows.h>
#include <Wininet.h>

int CALLBACK 
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
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
                
    return(0);
}
