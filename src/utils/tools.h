// C program for implementation of ftoa()
#ifndef TOOLS_H
#define TOOLS_H
#include<stdio.h>
#include<math.h>
void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char *res, int afterpoint);
#endif // TOOLS_H
