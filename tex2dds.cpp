/*	Ghostbusters The Video Game Texture Converter
    Copyright barncastle
    Copyright 2010 Jonathan Wilson
    Copyright 2024 KeyofBlueS

    The Ghostbusters texture converter is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 3, or (at your option) any later
    version. See the file COPYING for more details.
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>  // For uint32_t, uint8_t
#include <stdlib.h>  // For strdup

typedef uint32_t DWORD;
typedef uint8_t BYTE;

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
            ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
            ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

struct DDS_PIXELFORMAT
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

constexpr DWORD DDS_MAGIC =      0x20534444;
constexpr DWORD DDS_FOURCC =     0x00000004;  // DDPF_FOURCC;
constexpr DWORD DDS_RGB =        0x00000040;  // DDPF_RGB
constexpr DWORD DDS_RGBA =       0x00000041;  // DDPF_RGB | DDPF_ALPHAPIXELS
constexpr DWORD DDS_LUMINANCE =  0x00020000;
constexpr DWORD DDS_LUMINANCEA = 0x00020001;
constexpr DWORD DDS_PF_SIZE =    sizeof(DDS_PIXELFORMAT);

const DDS_PIXELFORMAT DDSPF_DXT1 = { DDS_PF_SIZE, DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_DXT2 = { DDS_PF_SIZE, DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_DXT3 = { DDS_PF_SIZE, DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_DXT4 = { DDS_PF_SIZE, DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_DXT5 = { DDS_PF_SIZE, DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_A8R8G8B8 = { DDS_PF_SIZE, DDS_RGBA, 0, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 };
const DDS_PIXELFORMAT DDSPF_A1R5G5B5 = { DDS_PF_SIZE, DDS_RGBA, 0, 16, 0x00007C00, 0x000003E0, 0x0000001F, 0x00008000 };
const DDS_PIXELFORMAT DDSPF_A4R4G4B4 = { DDS_PF_SIZE, DDS_RGBA, 0, 16, 0x00000F00, 0x000000F0, 0x0000000F, 0x0000F000 };
const DDS_PIXELFORMAT DDSPF_R8G8B8 = { DDS_PF_SIZE, DDS_RGB, 0, 24, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000 };
const DDS_PIXELFORMAT DDSPF_R5G6B5 = { DDS_PF_SIZE, DDS_RGB, 0, 16, 0x0000F800, 0x000007E0, 0x0000001F, 0x00000000 };
const DDS_PIXELFORMAT DDSPF_A8L8 = { DDS_PF_SIZE, DDS_LUMINANCEA, 0, 16, 0xFF, 0, 0, 0xFF00 };
const DDS_PIXELFORMAT DDSPF_L8 = { DDS_PF_SIZE, DDS_LUMINANCE, 0, 8, 0xFF, 0, 0, 0 };
const DDS_PIXELFORMAT DDSPF_A16B16G16R16F = { DDS_PF_SIZE, DDS_FOURCC, 113, 0, 0, 0, 0, 0 };

constexpr DWORD DDS_HEADER_FLAGS_TEXTURE =    0x00001007;  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT ;
constexpr DWORD DDS_HEADER_FLAGS_MIPMAP =     0x00020000;  // DDSD_MIPMAPCOUNT;
constexpr DWORD DDS_HEADER_FLAGS_VOLUME =     0x00800000;  // DDSD_DEPTH;
constexpr DWORD DDS_HEADER_FLAGS_PITCH =      0x00000008;  // DDSD_PITCH;
constexpr DWORD DDS_HEADER_FLAGS_LINEARSIZE = 0x00080000;  // DDSD_LINEARSIZE;
constexpr DWORD DDS_SURFACE_FLAGS_TEXTURE =   0x00001000;  // DDSCAPS_TEXTURE
constexpr DWORD DDS_SURFACE_FLAGS_MIPMAP =    0x00400008;  // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
constexpr DWORD DDS_SURFACE_FLAGS_CUBEMAP =   0x00000008;  // DDSCAPS_COMPLEX
constexpr DWORD DDS_CUBEMAP_POSITIVEX =       0x00000600;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
constexpr DWORD DDS_CUBEMAP_NEGATIVEX =       0x00000a00;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
constexpr DWORD DDS_CUBEMAP_POSITIVEY =       0x00001200;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
constexpr DWORD DDS_CUBEMAP_NEGATIVEY =       0x00002200;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
constexpr DWORD DDS_CUBEMAP_POSITIVEZ =       0x00004200;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
constexpr DWORD DDS_CUBEMAP_NEGATIVEZ =       0x00008200;  // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
constexpr DWORD DDS_FLAGS_VOLUME =            0x00200000;  // DDSCAPS2_VOLUME

constexpr DWORD DDS_CUBEMAP_ALLFACES = DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | \
                                       DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | \
                                       DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ;

struct DDS_HEADER
{
    DWORD dwSize = 124;
    DWORD dwHeaderFlags = DDS_HEADER_FLAGS_TEXTURE;
    DWORD dwHeight = 0;
    DWORD dwWidth = 0;
    DWORD dwPitchOrLinearSize = 0;
    DWORD dwDepth = 0; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
    DWORD dwMipMapCount = 0;
    DWORD dwReserved1[11] = {};
    DDS_PIXELFORMAT ddspf = {};
    DWORD dwSurfaceFlags = DDS_SURFACE_FLAGS_TEXTURE;
    DWORD dwCubemapFlags = 0;
    DWORD dwReserved2[3] = {};
};

struct TEX_Header
{
    DWORD dwVersion;
    BYTE bHash[16];
    DWORD dwUnknown14;
    DWORD dwFormat;
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwUnknown24;
    DWORD dwMipCount;
    DWORD dwUnknown2C;
    DWORD dwUnknown30;
};

int main(int argc, char* argv[])
{
    FILE* tex, * dds;

    // Replace fopen_s with fopen
    tex = fopen(argv[1], "rb");
    if (!tex) {
        return 0;
    }

    // Replace _strdup with strdup for Linux
    char* c = strdup(argv[1]);  
    c[strlen(c) - 3] = 'd';
    c[strlen(c) - 2] = 'd';
    c[strlen(c) - 1] = 's';

    TEX_Header header;
    fread(&header, sizeof(header), 1, tex);
    fseek(tex, 0, SEEK_END);
    DWORD size = ftell(tex) - sizeof(header);
    fseek(tex, sizeof(header), SEEK_SET);
    char* texdata = new char[size];
    fread(texdata, size, 1, tex);
    fclose(tex);

    DDS_HEADER desc;
    desc.dwHeight = header.dwHeight;
    desc.dwWidth = header.dwWidth;

    if (header.dwMipCount != 0)
    {
        desc.dwHeaderFlags |= DDS_HEADER_FLAGS_MIPMAP;
        desc.dwMipMapCount = header.dwMipCount + 1;
        desc.dwSurfaceFlags |= DDS_SURFACE_FLAGS_MIPMAP;
    }

    switch (header.dwFormat)
    {
    case 0x03:
        desc.ddspf = DDSPF_A8R8G8B8;
        break;
    case 0x04:
        desc.ddspf = DDSPF_R5G6B5;
        break;
    case 0x05:
        desc.ddspf = DDSPF_A4R4G4B4;
        break;
    case 0x17:
        desc.ddspf = DDSPF_DXT3;
        break;
    case 0x2B:
        desc.ddspf = DDSPF_DXT1;
        break;
    case 0x18:
        desc.ddspf = DDSPF_A8R8G8B8;
        desc.dwSurfaceFlags |= DDS_SURFACE_FLAGS_CUBEMAP;
        desc.dwCubemapFlags = DDS_CUBEMAP_ALLFACES;
        break;
    case 0x2E:
        desc.ddspf = DDSPF_A16B16G16R16F;
        break;
    case 0x2F:
        desc.ddspf = DDSPF_A8L8;
        break;
    case 0x32:
        desc.ddspf = DDSPF_DXT5;
        break;
    case 0x37:
        desc.ddspf = DDSPF_L8;
        break;
    }

    dds = fopen(c, "wb");
    if (!dds) {
        return 0;
    }

    fwrite(&DDS_MAGIC, 4, 1, dds);
    fwrite(&desc, sizeof(desc), 1, dds);
    fwrite(texdata, size, 1, dds);
    fclose(dds);    
    return 0;
}
