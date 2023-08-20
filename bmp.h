typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef struct tagBITMAPFILEHEADER FILEHEADER;
typedef struct tagBITMAPINFOHEADER INFOHEADER;

struct tagBITMAPFILEHEADER
{
WORD bfType; 
DWORD bfSize; 
WORD bfReserved1; 
WORD bfReserved2; 
DWORD bfOffBits; 
};

struct tagBITMAPINFOHEADER
{
DWORD biSize; 
LONG biWidth; 
LONG biHeight; 
WORD biPlanes; 
WORD biBitCount; 
DWORD biCompression;
DWORD biSizeImage; 
LONG biXPelsPerMeter; 
LONG biYPelsPerMeter; 
DWORD biClrUsed; 
DWORD biClrImportant; 
};