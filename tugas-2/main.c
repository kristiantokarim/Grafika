#include "base.c"
#include <math.h>

void plot4points(double cx, double cy, double x, double y)
{
  printSquare(cx + x, cy + y);
  printSquare(cx - x, cy + y);
  printSquare(cx + x, cy - y);
  printSquare(cx - x, cy - y);
}

void plot8points(double cx, double cy, double x, double y)
{
  plot4points(cx, cy, x, y);
  plot4points(cx, cy, y, x);
}
int main() {
  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
      perror("Error: cannot open framebuffer device");
      exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
      perror("Error reading fixed information");
      exit(2);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
      perror("Error reading variable information");
      exit(3);
  }

  printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

  displayWidth = vinfo.xres;
  displayHeight = vinfo.yres;

  // Figure out the SIZE of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                      fbfd, 0);
  if (atoi(fbp) == -1) {
      perror("Error: failed to map framebuffer device to memory");
      exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");
  printExplosion(500,500);

}
