// Simple C++/Objective interface
// - myCounter object written in C++

#import <objc/Object.h>
#import "myCounter.mh"

main()
{  myCounter * c1 = [[myCounter alloc] init];
   int i, j;

   printf("\nC++ -> ObjC interface testing:\n\n");

   c1->dVal = 5.5;
   c1->iInc = 3;
   strcpy(c1->sName, "c1");
   for(i=0; i<2; i++)                    // ix[2][4] = [10 11 12 13
   {  for (j=0; j<4; j++)                //             20 21 22 23]
         c1->iaX[i][j] = 10*(i+1) + j;
   }
   [c1 prtvars]; [c1 next: 5]; [c1 prtvars];
   printf("Parameter passing using va_list:   Sum = %f\n", [c1 sumN_x1: 1.1, 2.2, 3.3, 4.4, 5.5]);
   printf("Normal ObjC parameter passing:     Sum = %f\n", [c1 sum5_x1:1.1 x2:2.2 x3:3.3 x4:4.4 x5:5.5]);
   printf("\nEnd testing.\n\n");
}

