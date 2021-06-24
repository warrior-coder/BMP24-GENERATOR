#include <cmath>
#include <stdio.h>

typedef unsigned char BYTE;

typedef struct
{
    BYTE B;
    BYTE G;
    BYTE R;
} RGB;

class IMAGE
{
public:
    RGB** buffer = nullptr;
    short width;
    short height;
    RGB pen_color = {};

    IMAGE(short imageWidth, short imageHeight)
    {
        width = imageWidth;
        height = imageHeight;

        buffer = new RGB* [height];
        for (short y = 0; y < height; y++)
        {
            buffer[y] = new RGB[width];

            for (short x = 0; x < width; x++)
            {
                buffer[y][x] = {};
            }
        }
    }

    ~IMAGE()
    {
        if (buffer)
        {
            for (short i = 0; i < height; i++)
            {
                delete[] buffer[i];
            }
            delete[] buffer;
            buffer = nullptr;
        }
    }

    void set_pixel(short x, short y)
    {
        if (x > -1 && y > -1 && x < width && y < height)
        {
            buffer[y][x] = pen_color;
        }
    }

    void set_circle(short x0, short y0, short R)
    {
        for (short x = x0 - R; x < x0 + R; x++)
        {
            for (short y = y0 - R; y < y0 + R; y++)
            {
                if (((x-x0)*(x-x0) + (y-y0)*(y-y0) - R*R) < 0) set_pixel(x, y);
            }
        }
    }

    void save(const char* fname)
    {
        const short f_header_size = 14;
        BYTE f_header[f_header_size];

        const short f_info_size = 40;
        BYTE f_info[f_info_size];

        int p_size = (4 - (width * 3) % 4) % 4;

        int fsize = f_header_size + f_info_size + (width * height * 4) + (p_size * height);

        /* HEADER - 14 BYTES */
        // File signature
        f_header[0] = 'B';
        f_header[1] = 'M';

        // File size, Max file size = 2^(4*8) = 4.294.967.296 bits = 536.870.912 bytes
        f_header[2] = fsize;
        f_header[3] = fsize >> 8;
        f_header[4] = fsize >> 16;
        f_header[5] = fsize >> 24;

        // Reserved
        f_header[6] = 0;
        f_header[7] = 0;
        f_header[8] = 0;
        f_header[9] = 0;

        // Pixel data offset
        f_header[10] = f_header_size + f_info_size;
        f_header[11] = 0;
        f_header[12] = 0;
        f_header[13] = 0;

        /* INFO - 40 BYTES */
        // Header size
        f_info[0] = f_info_size;
        f_info[1] = 0;
        f_info[2] = 0;
        f_info[3] = 0;

        // Image width
        f_info[4] = width;
        f_info[5] = width >> 8;
        f_info[6] = width >> 16;
        f_info[7] = width >> 24;

        // Image height
        f_info[8] = height;
        f_info[9] = height >> 8;
        f_info[10] = height >> 16;
        f_info[11] = height >> 24;

        // Planes
        f_info[12] = 1;
        f_info[13] = 0;

        // Bits per pixel (RGB - 3*8 = 24 bits)
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

        // X pixels per meter
        f_info[24] = 0;
        f_info[25] = 0;
        f_info[26] = 0;
        f_info[27] = 0;

        // Y pixels per meter
        f_info[28] = 0;
        f_info[29] = 0;
        f_info[30] = 0;
        f_info[31] = 0;

        // Total colors (Color palette not used)
        f_info[32] = 0;
        f_info[33] = 0;
        f_info[34] = 0;
        f_info[35] = 0;

        // Important colors (0 - all)
        f_info[36] = 0;
        f_info[37] = 0;
        f_info[38] = 0;
        f_info[39] = 0;

        // Write file
        FILE* fp;

        fopen_s(&fp, fname, "wb");
        if (fp)
        {
            // Write header
            fwrite(f_header, sizeof(BYTE), f_header_size, fp);

            // Write info
            fwrite(f_info, sizeof(BYTE), f_info_size, fp);

            // Write buffer
            BYTE padding = 0;

            for (short y = 0; y < height; y++)
            {
                fwrite(buffer[y], sizeof(RGB), width, fp);
                fwrite(&padding, sizeof(BYTE), p_size, fp);
            }

            fclose(fp);

            printf("File creadted.");
        }
        else printf("File creadte error.");
    }
};

int main()
{
    IMAGE img(640, 640);

    // Set pen color
    img.pen_color = { 255,255,255 };

    // Draw gradient background
    for (short y = 0; y < img.height; y++)
    {
        for (short x = 0; x < img.width; x++)
        {
            img.buffer[y][x].R = (BYTE)(255 - (float)x  / img.width * 255);
            img.buffer[y][x].G = (BYTE)((float)x / img.width * 255);
            img.buffer[y][x].B = (BYTE)((float)y / img.height * 255);
        }
    }
    // Draw C++
    for (float deg = 1.39626; deg < 4.88692; deg += .0174533)
    {
        img.set_circle((short)(230 + cos(deg) * 180), (short)(320 + sin(deg) * 180), 8);
    }

    for (short i = 0; i < 200; i++)
    {
        img.set_circle(130+i, 320, 8);
        img.set_circle(230, 220+i, 8);
        img.set_circle(380 + i, 320, 8);
        img.set_circle(480, 220+i, 8);
    }

    // Save result
    img.save("out.bmp");

    return 0;
}