# BMP24 Generator
Just call **write_bmp()** function and programm creates a new bmp according to the draw script.

```CPP
write_bmp("result.bmp", 640, 640);
```

## Result: ##
![cpp](result.bmp)

## Example of draw script: ##
```CPP
// Draw gradient background
for (short y = 0; y < bh; y++)
{   
    buffer[y] = new RGB[bw];
    for (short x = 0; x < bw; x++)
    {
        buffer[y][x].R = (BYTE)(255 - (float)x / bw * 255);
        buffer[y][x].G = (BYTE)((float)x / bw * 255);
        buffer[y][x].B = (BYTE)((float)y / bh * 255);
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
```

You can write any draw script you want and get the result.