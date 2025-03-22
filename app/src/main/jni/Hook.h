#define __int8 char
#define __int32 int
#define __int64 long long
DWORD NewBase = 0;
DWORD libanogsBase = 0;
DWORD libanortBase = 0;
DWORD libanogsAlloc = 0;
DWORD libil2cppBase = 0;
DWORD libil2cppAlloc = 0;
DWORD libcBase = 0;
DWORD libEgl_base = 0;
DWORD LibcAlloc = 0;
DWORD EglAlloc = 0;
DWORD OriginalStackCheck = 0;
unsigned int libanogsSize = 0x33457d;
//unsigned int libil2cppSize = 0x5B2E3D8;
unsigned int libil2cppSize = 0x5B2fb00;
unsigned int libcSize = 0xC3000;
unsigned int libEglSize = 0x2000;
typedef unsigned char BYTE;   // 8-bit unsigned entity.
typedef BYTE* PBYTE;  // Pointer to BYTE.
class _BYTE;
class _BOOL4;
#define _BYTE  uint8_t
#define _WORD  uint16_t
#define _DWORD uint32_t
#define _QWORD uint64_t
#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long
typedef unsigned short _WORD;


void* hooked_memcpy(char* dest, const char* src, unsigned int size)
{
    char* destination = dest;
    DWORD SubBase = 0;
    if ((char*)libcBase <= src && (char*)(libcBase + libanogsSize) > src)
    {
        dest = (char*)(src - (char*)libcBase);
        SubBase = LibcAlloc;
    Change_Src:
        src = &dest[SubBase];
        goto Return_Code;
    }
    if ((char*)libEgl_base <= src && (char*)(libEglSize + libEgl_base) > src)
    {
        dest = (char*)(src - (char*)libEgl_base);
        SubBase = EglAlloc;
        goto Change_Src;
    }
    if (size > 0x10000 && (char*)libil2cppBase <= src && DWORD(libil2cppBase + libil2cppSize) > (DWORD)src)
    {
        dest = (char*)(src - (char*)libil2cppBase);
        SubBase = libil2cppAlloc;
        goto Change_Src;
    }
    if (size > 0x10000 && (char*)libanogsBase <= src && DWORD(libanogsBase + libanogsSize) > (DWORD)src)
    {
        dest = (char*)(src - (char*)libanogsBase);
        SubBase = libanogsAlloc;
        goto Change_Src;
    }
Return_Code:
    return memcpy(destination, src, size);
}
