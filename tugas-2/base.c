#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define SIZE 5
#define POINT_SIZE 65

/* Kamus Global */
int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int displayWidth, displayHeight;
int NEff;

int isValid(int x, int y) {
    return x>=0 && x<displayWidth-5 && y>=0 & y<displayHeight-5;
}

void printBackground() {
    int x=0, y=0;
    int width = displayWidth - 6;
    int height = displayHeight - 6;
    long int location = 0;
    int i,j;
    for (j = y; j < y+height; j++) {
        for (i = x; i < x+width; i++) {

            location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (j+vinfo.yoffset) * finfo.line_length;

            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            } else  { //assume 16bpp
                int b = 0;
                int g = 0;     // A little green
                int r = 0;    // A lot of red
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
        }
    }
}

void printRectangle(int height, int width, int x, int y) {
    long int location = 0;
    int i,j;
    for (j = y; j < y+height; j++) {
        for (i = x; i < x+width; i++) {
            if(isValid(i,j)) {
                location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                            (j+vinfo.yoffset) * finfo.line_length;

                if (vinfo.bits_per_pixel == 32) {
                    *(fbp + location) = 100;        // Some blue
                    *(fbp + location + 1) = 15+(i-100)/2;     // A little green
                    *(fbp + location + 2) = 200-(j-100)/5;    // A lot of red
                    *(fbp + location + 3) = 0;      // No transparency
                } else  { //assume 16bpp
                    int b = 10;
                    int g = (i-100)/6;     // A little green
                    int r = 31-(j-100)/16;    // A lot of red
                    unsigned short int t = r<<11 | g << 5 | b;
                    *((unsigned short int*)(fbp + location)) = t;
                }
            }
        }
    }
}

void printSquare(int x, int y) {
    printRectangle(SIZE,SIZE,x,y);
}

void drawLine(int x1, int y1, int x2, int y2) {
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

	dx=x2-x1;			//Delta x
	dy=y2-y1;			//Delta y
	dxabs=abs(dx);		//Absolute delta
	dyabs=abs(dy);		//Absolute delta
	sdx=(dx>0)?1:-1; //signum function
	sdy=(dy>0)?1:-1; //signum function
	x=dyabs>>1;
	y=dxabs>>1;
	px=x1;
	py=y1;

	if (dxabs>=dyabs)
	{
		for(i=0;i<dxabs;i++)
		{
			y+=dyabs;
			if (y>=dxabs)
			{
				y-=dxabs;
				py+=sdy;
			}
			px+=sdx;
			printSquare(px,py);
		}
	}
	else
	{
		for(i=0;i<dyabs;i++)
		{
			x+=dxabs;
			if (x>=dyabs)
			{
				x-=dyabs;
				px+=sdx;
			}
			py+=sdy;
			printSquare(px,py);
		}
	}
}

void drawOutlineCircle(double cx, double cy, int radius) {
  inline void plot4points(double cx, double cy, double x, double y)
	{
		printSquare(cx + x, cy + y);
		printSquare(cx - x, cy + y);
		printSquare(cx + x, cy - y);
		printSquare(cx - x, cy - y);
	}

	int error = -radius;
	double x = radius;
	double y = 0;
	while (x >= y)
	{
    plot4points(cx, cy, x, y);
		plot4points(cx, cy, y, x);

    error += y;
		y++;
		error += y;

		if (error >= 0)
		{
			error += -x;
			x--;
			error += -x;
		}
	}
}

void printExplosion(int x, int y) {
  int i;
  for (i=0; i<100; i+=20) {
      drawOutlineCircle(x, y, 50+i);
      usleep(50000);
  }
}
