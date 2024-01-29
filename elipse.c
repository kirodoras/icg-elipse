// compilar: gcc elipse.c -o elipse
// executar: ./elipse

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void setPixel(unsigned char (**im)[3], int i, int j, unsigned char r, unsigned char g, unsigned char b)
{
    im[i][j][0] = r;
    im[i][j][1] = g;
    im[i][j][2] = b;
}

void setQuadrantes(unsigned char (**im)[3], int l, int h, int xc, int yc, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if (xc + x >= 0 && xc + x < l && yc + y >= 0 && yc + y < h)
        setPixel(im, xc + x, yc + y, r, g, b);
    if (xc - x >= 0 && xc - x < l && yc + y >= 0 && yc + y < h)
        setPixel(im, xc - x, yc + y, r, g, b);
    if (xc + x >= 0 && xc + x < l && yc - y >= 0 && yc - y < h)
        setPixel(im, xc + x, yc - y, r, g, b);
    if (xc - x >= 0 && xc - x < l && yc - y >= 0 && yc - y < h)
        setPixel(im, xc - x, yc - y, r, g, b);
}

void elipse(unsigned char (**im)[3], int l, int h, int xc, int yc, int a, int b, unsigned char red, unsigned char green, unsigned char blue)
{
    int x, y;
    float d;

    x = 0;
    y = b;
    d = b * b - a * a * b + a * a / 4;

    while (a * a * y >= b * b * x)
    {
        setQuadrantes(im, l, h, xc, yc, x, y, red, green, blue);
        if (d < 0)
        {
            d = d + b * b * (2 * x + 3);
            x++;
        }
        else
        {
            d = d + b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            x++;
            y--;
        }
    }

    d = b * b * (x + 1 / 2) * (x + 1 / 2) + a * a * (y - 1) * (y - 1) - a * a * b * b;

    while (y >= 0)
    {
        setQuadrantes(im, l, h, xc, yc, x, y, red, green, blue);
        if (d > 0)
        {
            d = d + a * a * (-2 * y + 3);
            y--;
        }
        else
        {
            d = d + b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
            y--;
        }
    }
}

void main(int argc, char **argv)
{
    int i, j, l, h;
    unsigned char cmax = 255;
    l = h = 500;
    FILE *fp;

    unsigned char(**imagem)[3];

    j = l * sizeof(char *);
    imagem = malloc(j);

    j = h * 3;
    for (i = 0; i < l; i++)
        imagem[i] = malloc(j);

    for (j = 0; j < h; j++)
        for (i = 0; i < l; i++)
        {
            setPixel(imagem, i, j, 0, 0, 0);
        }

    srand(time(NULL));
    for (i = 0; i < 50; i++)
        elipse(imagem, l, h, rand() % l, rand() % h, rand() % 300 + 30, rand() % 200 + 10, rand(), rand(), rand());

    fp = fopen("output.ppm", "w");
    fprintf(fp, "P6\n");
    fprintf(fp, "%u %u\n255\n", l, h);
    for (j = 0; j < h; j++)
        for (i = 0; i < l; i++)
            fprintf(fp, "%c%c%c", imagem[i][j][0], imagem[i][j][1], imagem[i][j][2]);
    fclose(fp);

    for (i = 0; i < l; i++)
        free(imagem[i]);
    free(imagem);
}
