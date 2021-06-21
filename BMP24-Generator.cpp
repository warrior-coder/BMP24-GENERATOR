#include <cmath>
#include <stdio.h>
#define PI 3.14159265358979323846

using namespace std;

typedef unsigned char BYTE;

typedef struct {
    BYTE B;
    BYTE G;
    BYTE R;
}RGB;

// Buffer - part of file which contains pixels data
RGB** buffer;
short bw, bh;

RGB get_color(short x, short y)
{
    RGB color = {};

    color.R = (BYTE)(255 - (float)x / bw * 255);
    color.G = (BYTE)((float)x / bw * 255);
    color.B = (BYTE)((float)y / bh * 255);

    return color;
}

void set_pixel(short x, short y)
{
    if (x > -1 && y > -1 && x < bw && y < bh)
    {
        buffer[y][x] = { 255,255,255 };
    }
}

void fill_circle(short x0, short y0, float R)
{
    float f;

    for (float x = x0 - R; x < x0 + R; x++)
    {
        for (float y = y0 - R; y < y0 + R; y++)
        {
            f = (x - x0) * (x - x0) + (y - y0) * (y - y0) - R*R;
            if (f < 0) set_pixel((short)x, (short)y);
        }
    }
}

void fill_rect(short x1, short y1, short x2, short y2)
{
    for (short y = y1; y <= y2; y++)
    {
        for (short x = x1; x <= x2; x++)
        {
            set_pixel(x, y);
        }
    }
}

void write_bmp(const char* fname, short img_width, short img_height)
{
    // -+-+-+-+-+-+-+-+-+-+- GENERATE BUFFER -+-+-+-+-+-+-+-+-+-+-
    // Create buffer
    bw = img_width;
    bh = img_height;
    buffer = new RGB*[bh];

    // Draw gradient background
    for (short y = 0; y < bh; y++)
    {   
        buffer[y] = new RGB[bw];

        for (short x = 0; x < bw; x++)
        {
            buffer[y][x] = get_color(x, y);
        }
    }

    // Draw C++
    short x, y;
    for (float deg = -90; deg < 90; deg += 0.5)
    {
        x = (short)round(img_width / 3 - cos(deg / 180 * PI) * img_height / 4);
        y = (short)round(img_height / 2 + sin(deg / 180 * PI) * img_height / 4);

        fill_circle(x, y, 8);
    }
    for (short i = -bh/6; i < bh/6; i++)
    {
        fill_circle(bw/6*2 + i, bh/2, 8);
        fill_circle(bw/6*2, bh/2 + i, 8);

        fill_circle(bw/12*9 + i, bh/2, 8);
        fill_circle(bw/12*9, bh/2 + i, 8);
    }

    // -+-+-+-+-+-+-+-+-+-+- GENERATE BOF -+-+-+-+-+-+-+-+-+-+-
    // File header
    const short f_header_size = 14;
    BYTE f_header[f_header_size];

    // File info
    const short f_info_size = 40;
    BYTE f_info[f_info_size];

    // File size
    int p_size = (4 - (img_width * 3) % 4) % 4;
    int fsize = f_header_size + f_info_size + (img_width * img_height * 4) + (p_size * img_height);

    // -+-+-+-+-+-+-+-+-+-+- HEADER - 14 BYTES -+-+-+-+-+-+-+-+-+-+-
    // File type
    f_header[0] = 'B';
    f_header[1] = 'M';

    // File size, Max file size = 2^(4 * 8) = 4.294.967.296 bits = 536.870.912 bytes
    f_header[2] = fsize;
    f_header[3] = fsize >> 8;
    f_header[4] = fsize >> 16;
    f_header[5] = fsize >> 24;

    // Reserved (Not used)
    f_header[6] = 0;
    f_header[7] = 0;

    // Reserved (Not used)
    f_header[8] = 0;
    f_header[9] = 0;

    // Pixel data offset
    f_header[10] = f_header_size + f_info_size;
    f_header[11] = 0;
    f_header[12] = 0;
    f_header[13] = 0;

    // -+-+-+-+-+-+-+-+-+-+- INFO - 40 BYTES -+-+-+-+-+-+-+-+-+-+-
    // Header size
    f_info[0] = f_info_size;
    f_info[1] = 0;
    f_info[2] = 0;
    f_info[3] = 0;

    // Image width
    f_info[4] = img_width;
    f_info[5] = img_width >> 8;
    f_info[6] = img_width >> 16;
    f_info[7] = img_width >> 24;

    // Image height
    f_info[8] = img_height;
    f_info[9] = img_height >> 8;
    f_info[10] = img_height >> 16;
    f_info[11] = img_height >> 24;

    // Planes
    f_info[12] = 1;
    f_info[13] = 0;

    // Bits per pixel (RGB - 3*8 bits)
    f_info[14] = 24;
    f_info[15] = 0;

    // Compression (No compression)
    f_info[16] = 0;
    f_info[17] = 0;
    f_info[18] = 0;
    f_info[19] = 0;

    // Image size (No compression)
    f_info[20] = 0;
    f_info[21] = 0;
    f_info[22] = 0;
    f_info[23] = 0;

    // X pixels per meter (Not specified)
    f_info[24] = 0;
    f_info[25] = 0;
    f_info[26] = 0;
    f_info[27] = 0;

    // Y pixels per meter (Not specified)
    f_info[28] = 0;
    f_info[29] = 0;
    f_info[30] = 0;
    f_info[31] = 0;

    // Total colors (Color palette not used)
    f_info[32] = 0;
    f_info[33] = 0;
    f_info[34] = 0;
    f_info[35] = 0;

    // Important colors (Ignored)
    f_info[36] = 0;
    f_info[37] = 0;
    f_info[38] = 0;
    f_info[39] = 0;

    // -+-+-+-+-+-+-+-+-+-+- WRITE FILE -+-+-+-+-+-+-+-+-+-+-
    FILE* fp;

    fopen_s(&fp, fname, "wb");
    if (fp)
    {
        fwrite(f_header, sizeof(BYTE), f_header_size, fp);
        fwrite(f_info, sizeof(BYTE), f_info_size, fp);

        BYTE padding = 0;

        for (short y = 0; y < img_height; y++)
        {
            fwrite(buffer[y], sizeof(RGB), img_width, fp);
            fwrite(&padding, sizeof(BYTE), p_size, fp);
        }

        fclose(fp);
        delete[] buffer;

        printf("File creadted.");
    }
    else printf("File create error.");
}

int main()
{
    write_bmp("out.png", 640, 640);

    return 0;
}