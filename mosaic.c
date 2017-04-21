/* Copyright 2016 - 2017 Marc Volker Dickmann
 * A little mosaic filter!
 */
#include <stdio.h>
#include <stdlib.h>
#include "lib/libbmp.h"

#define FLTR_MKBORDER(a) ((a) >= 5 ? (a) - 5 : (a))

static bmp_pixel
filter_color_get (const bmp_img *img, const int px, const int py, const int width, const int height)
{
	int red = 0;
	int green = 0;
	int blue = 0;
	
	for (int y = py, x; y < py + height; y++)
	{
		for (x = px; x < px + width; x++)
		{
			if (x < img->img_header.biWidth &&
			    y < abs (img->img_header.biHeight))
			{
				red += img->img_pixels[y][x].red;
				green += img->img_pixels[y][x].green;
				blue += img->img_pixels[y][x].blue;
			}
		}
	}
	
	// Calculate this only once.
	const int s = width * height;
	
	bmp_pixel r;
	bmp_pixel_init (&r, (red / s) % 256, (green / s) % 256, (blue / s) % 256);
	return r;
}

static void
filter_color_set (const bmp_img *img, const int px, const int py, const int width, const int height, const bmp_pixel *pxl)
{
	for (int y = py, x; y < py + height; y++)
	{
		for (x = px; x < px + width; x++)
		{
			if (x < img->img_header.biWidth &&
			    y < abs (img->img_header.biHeight))
			{
				if (x == px ||
				    x == px + width - 1 ||
				    y == py ||
				    y == py + height - 1)
				{
					bmp_pixel_init (&img->img_pixels[y][x], FLTR_MKBORDER (pxl->red), FLTR_MKBORDER (pxl->green), FLTR_MKBORDER (pxl->blue));
				}
				else
				{
					bmp_pixel_init (&img->img_pixels[y][x], pxl->red, pxl->green, pxl->blue);
				}
			}
		}
	}
}

static void
filter_apply (const char *filename, const int tile_width, const int tile_height)
{
	bmp_img img;
	
	if (bmp_img_read (&img, filename) != BMP_OK)
	{
		return;
	}
	
	bmp_pixel pxl;
	for (int y = 0, x; y < abs (img.img_header.biHeight); y += tile_height)
	{   
		for (x = 0; x < img.img_header.biWidth; x += tile_width)
		{
			pxl = filter_color_get (&img, x, y, tile_width, tile_height);
			filter_color_set (&img, x, y, tile_width, tile_height, &pxl);
		}
	}

	bmp_img_write (&img, "out.bmp");
	bmp_img_free (&img);
}

int
main (int argc, char *argv[])
{
	printf ("Mosaic v. 0.0.1 A (c) 2016 - 2017 Marc Volker Dickmann\n");
	
	if (argc == 4)
	{
		filter_apply (argv[1], atoi (argv[2]), atoi (argv[3]));
	}
	else
	{
		printf ("Usage: %s <filename> <tile_width> <tile_height>!\n", argv[0]);
	}
	
	return 0;
}
