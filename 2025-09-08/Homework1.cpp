#include <stdio.h>
#include <stdint.h>


    // FUNCTION FOR PROBLEM 4 //

unsigned int mysteryop(unsigned int n) 
    {

        printf("The original number in hexadecimal is: 0x%08x\nThe original number in decimal is: %d\n\n", n, n);
        // set the low nibbles of each byte for the mask
        unsigned int lownibs = 0xf0f;    // 00000000 00000000 00001111 00001111 (mask)

        printf("the old mask is 0x%08x", lownibs);
        printf("\n");

        lownibs |= (lownibs << 16);      // 00001111 00001111 00000000 00000000 
                                         // 00000000 00000000 00001111 00001111
                                     //   | ___________________________________
                                         // 00001111 00001111 00001111 00001111 (new mask)

        printf("the new mask is 0x%08x\n", lownibs);
        printf("\n");

        n = ((n >> 4) & lownibs) | ((n & lownibs) << 4);
        return n;
    }

int main()
{

    // PROBLEM 0 //

    uint8_t foo0;
    int64_t foo1;
    uint16_t foo2;
    uint8_t foo3[128];

    unsigned char x0;
    long x1;
    unsigned short x2;
    unsigned char x3[128];

    if (sizeof(foo0) == sizeof(x0))
    {
        printf("lets gooo\n");
    }
    else
    {
        printf("you're cooked lmaooooo\n");
    }

    if (sizeof(foo1) == sizeof(x1))
    {
        printf("lets gooo\n");
    }
    else
    {
        printf("you're cooked lmaooooo\n");
    }

    if (sizeof(foo2) == sizeof(x2))
    {
        printf("lets gooo\n");
    }
      else
    {
        printf("you're cooked lmaooooo\n");
    }

    if (sizeof(foo3) == sizeof(x3))
    {
        printf("lets gooo\n");
    }
      else
    {
        printf("you're cooked lmaooooo\n");
    }

    // PROBLEM 1 //

    int foo4 = 0x01234567; //00000001 00100011 01000101 01100111  ~foo4 = 11111110 11011100 10111010 10011000
    int bar = 0x98765432;  //10011000 01110110 01010100 00110010    11111111 11111111 00000000 00000000      
                                                                                                         

    int expressions[4];  
    expressions[0] = foo4 >> 4; // 00000000 00010010 00110100 01010110 = 0x00123456 (logical shift)
    expressions[1] = bar >> 4;  // 11111001 10000111 01100101 01000011 = 0xf9876543 (arithmetic shift)
    expressions[2] = foo4 & bar; // 00000000 00100010 01000100 00100010 = 0x00224422
    expressions[3] = ~foo4 & bar; // 10011000 01010100 00010000 00010000 = 0x98541010 
    expressions[4] = (foo4 &0xffff)|(bar&0xffff0000); // (foo4 & 00000000 00000000 11111111 11111111) = 00000000 00000000 01000101 01100111
                                                      // (bar & 11111111 11111111 00000000 00000000)  = 10011000 01110110 00000000 00000000
                                                      //                                              | ___________________________________
                                                      //                                                10011000 01110110 01000101 01100111 = 0x98764567

    for (int i = 0; i <= 4; i++)
    {
        printf("0x%08x\n",expressions[i]);
    }    

        // PROBLEM 2 //

    uint16_t op0 = 20;
    uint16_t op1 = 10;
    uint16_t op2 = 9;
    uint16_t op3 = 63;
    uint16_t op4 = 252;
    uint16_t op5 = 255;
    uint16_t op6 = 4;
    uint16_t op7 = 3;

    int expressions2[4];
    expressions2[0] = (op0 & op1) | op2;
    expressions2[1] = (op3 ^ op4) & op5;
    expressions2[2] = (uint16_t)~(op5 << op6);
    expressions2[3] = (op7 << op7) | op7;

    for (int i = 0; i <= 3; i++)
    {
        printf("%d\n",expressions2[i]);
    }    

    printf("\n");

        // Problem 4

    uint32_t test = 0x1234; //4660
    
    uint32_t result = (mysteryop(test));

    printf("In hexadecimal, the result is: 0x%08x\nIn decimal, the result is %d\n", result, result);

    return 0;
}
