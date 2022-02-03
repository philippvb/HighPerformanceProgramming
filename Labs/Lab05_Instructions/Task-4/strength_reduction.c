#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   int i;
   int a = 1, b = 2, c = 3, d = 1;
   float x = 0.1, y = 0.5, z = 0.33;
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);

   for (i=0; i<50000000; i++)
   {
      c = d<<1;
      b = (c<<4)-c;
      a = b>>4;
      // d = (int) ((unsigned int) a / (unsigned int) b);
      d = 16;

      y = 0.66;
      x = 0.66 / 1.33;
      z = 0.66 / 1.33;
   }
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);
   return 0;
}
