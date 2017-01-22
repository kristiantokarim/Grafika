/* Nama File : alphabet.c */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define SIZE 10

/* Kamus Global */
int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int displayWidth, displayHeight;

int isValid(int x, int y) {
    return x>=0 && x<displayWidth-5 && y>=0 & y<displayHeight-5;
}

//print persegi
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

//print sudut kiri bawah
void printLeftDownTriangle(int height, int width, int x, int y) {
    long int location = 0;
    int i,j;
    for (j = y; j < y+height; j++) {
        for (i = j; i < x+width; i++) {
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

//Untuk clear layar biar item semua
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
