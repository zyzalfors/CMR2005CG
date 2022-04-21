#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* cheats[10] = { "All tracks code", "4WD cars code", "2WD cars code", "Super 2WD cars code", "RWD cars code", "4x4 cars code", "Classic cars code", "Special cars code", "Group B cars code", "Mirror mode code (inaccessible)" };

int indexOf(unsigned char** array, int size, unsigned char* text)
{
 for(int i = 0; i < size; i++)
 {
  if(strcmp(array[i], text) == 0)
  {
   return i;
  }
 }
 return -1;
}

unsigned char* toLower(unsigned char* text)
{
 unsigned char* lower = (unsigned char*) malloc(strlen(text) * sizeof(unsigned char));
 if(lower == NULL)
 {
  return text;
 }
 for(int i = 0; i < strlen(text); i++)
 {
  lower[i] = tolower(text[i]);
 }
 return lower;
}

unsigned char* toUpper(unsigned char* text)
{
 unsigned char* upper = (unsigned char*) malloc(strlen(text) * sizeof(unsigned char));
 if(upper == NULL)
 {
  return text;
 }
 for(int i = 0; i < strlen(text); i++)
 {
  upper[i] = toupper(text[i]);
 }
 return upper;
}

long quot(long n, long m)
{
 return n / m;
}

long rem(long n, long m)
{
 return n % m;
}

long toSigned32(long n)
{
 return ((n & 0xFFFFFFFFL) ^ 0x80000000L) - 0x80000000L;
}

long calcFeedback(long buffer[])
{
 long result = 0;
 for(int i = 0; i < 5; i++)
 {
  result += buffer[i] ^ 0x13C501L;
 }
 return toSigned32(result);
}

long calcSeed(long n)
{
 long seed = 1L;
 if(n != 0L)
 {
  seed = 0xF82DL;
  for(int i = 0; i < n - 1; i++)
  {
   seed = rem(toSigned32(0xF82DL * seed), 0x5243L);
  }
 }
 return seed;
}

unsigned char* generateCode(long array1[], long array2[], long accessCode, long cheatID)
{
 long cheatIDMagic = 0x13CB5BL * cheatID % 0x26DDL;
 long accessCodeMagic = (accessCode % 0x3E8L) ^ (0x20B9L * cheatIDMagic);
 long seed1 = calcSeed(accessCodeMagic % 0x9ADL);
 long seed2 = calcSeed(rem(toSigned32((accessCodeMagic ^ 0x114CF1L) * ((0x41BL * cheatIDMagic) ^ rem(quot(accessCode, 0x3E8L), 0x3E8L))), 0x91DL));
 long buffer[6] = { 0L, 0L, 0L, 0L, 0L, 0L };
 buffer[0] = rem(seed1, 26L) + 65L;
 buffer[1] = rem(quot(seed1, 676L), 26L) + 65L;
 buffer[2] = rem(quot(seed1, 26L), 26L) + 65L;
 buffer[3] = rem(quot(seed2, 26L), 26L) + 65L;
 buffer[4] = rem(quot(seed2, 676L), 26L) + 65L;
 buffer[5] = rem(seed2, 26L) + 65L;
 long bufMidXor = calcFeedback(buffer);
 long feedback1 = toSigned32((buffer[0] << 24L) + (buffer[1] << 16L) + (buffer[2] << 8L) + buffer[3]);
 long feedback2 = toSigned32((buffer[4] << 24L) + (buffer[5] << 16L) + ((bufMidXor + rem(cheatIDMagic ^ 0x197ABD9L, seed1 & 0xFFFFFFFFL)) << 8L) + bufMidXor + rem(cheatIDMagic ^ 0x13478FDDL, seed2 & 0xFFFFFFFFL));
 long tmpFeedback2 = 0L;
 for(int i = 0; i < 42; i++)
 {
  tmpFeedback2 = feedback2;
  feedback2 = feedback1 ^ array1[i % 10];
  feedback1 = tmpFeedback2 ^ feedback1;
 }
 for(int i = 0; i < 277; i++)
 {
  tmpFeedback2 = feedback2;
  feedback2 = feedback1 ^ array2[i % 10];
  feedback1 = feedback1 ^ tmpFeedback2;
 }
 buffer[0] = rem((feedback2 >> 24L) & 0xFFL, 26L) + 65L;
 buffer[1] = rem((feedback2 >> 16L) & 0xFFL, 26L) + 65L;
 buffer[2] = rem((feedback1 >> 24L) & 0xFFL, 26L) + 65L;
 buffer[3] = rem((feedback1 >> 16L) & 0xFFL, 26L) + 65L;
 buffer[4] = rem((feedback1 >> 8L) & 0xFFL, 26L) + 65L;
 buffer[5] = rem(feedback1 & 0xFFL, 26L) + 65L;
 unsigned char* code = (unsigned char*) malloc(6 * sizeof(unsigned char));
 if(code == NULL)
 {
  return "NULL";
 }
 for(int i = 0; i < 6; i++)
 {
  code[i] = (unsigned char) buffer[i];
 }
 return code;
}

int main(int argc, unsigned char** argv)
{
 printf("Colin McRae Rally 2005 Cheat Generator\n");
 if(argc != 5 || (indexOf(argv, argc, "-p") == -1 && indexOf(argv, argc, "-P") == -1) || (indexOf(argv, argc, "-c") == -1 && indexOf(argv, argc, "-C") == -1))
 {
  printf("Usage: CMR2005CG [commands]\n");
  printf("Commands  Parameters            Explanation\n");
  printf("-p        [pc, ps2, psp, xbox]  Set game platform\n");
  printf("-c        [1, ..., 99999]       Set access code\n");
  return 0;
 }
 int ip = indexOf(argv, argc, "-p") != -1 ? indexOf(argv, argc, "-p") : indexOf(argv, argc, "-P");
 int ic = indexOf(argv, argc, "-c") != -1 ? indexOf(argv, argc, "-c") : indexOf(argv, argc, "-C");
 unsigned char* platform = toLower(argv[ip + 1]);
 long accessCode = strtol(argv[ic + 1], NULL, 10);
 if(accessCode < 1L || accessCode > 99999L)
 {
  printf("Invalid access code. Valid access codes are in range 1 - 99999\n");
  return 0;
 }
 long array1[10];
 long array2[10];
 if(strcmp(platform, "pc") == 0)
 {
  long arr1[10] = { 331L, 1259L, 241L, 109L, 521L, 853L, 71L, 719L, 941L, 269L };
  long arr2[10] = { 72481L, 180307L, 130241L, 392827L, 421019L, 949147L, 32801L, 1296649L, 91249L, 639679L };
  for(int i = 0; i < 10 ; i++)
  {
   array1[i] = arr1[i];
   array2[i] = arr2[i];
  }
 }
 else if(strcmp(platform, "ps2") == 0 || strcmp(platform, "xbox") == 0)
 {
  long arr1[10] = { 859L, 773L, 151L, 47L, 487L, 211L, 617L, 131L, 947L, 313L };
  long arr2[10] = { 69119L, 67783L, 70271L, 77929L, 238099L, 148151L, 472751L, 818963L, 1195489L, 839381L };
  for(int i = 0; i < 10 ; i++)
  {
   array1[i] = arr1[i];
   array2[i] = arr2[i];
  }
 }
 else if(strcmp(platform, "psp") == 0)
 {
  long arr1[10] = { 743L, 1663L, 227L, 991L, 443L, 89L, 571L, 199L, 1373L, 601L };
  long arr2[10] = { 35491L, 783019L, 1116491L, 591319L, 194591L, 37369L, 822839L, 86083L, 354661L, 99809L };
  for(int i = 0; i < 10 ; i++)
  {
   array1[i] = arr1[i];
   array2[i] = arr2[i];
  }
 }
 else
 {
  printf("Invalid platform. Enter a platform among pc, ps2, psp, xbox\n");
  return 0;
 }
 printf("Platform: %s\n", toUpper(platform));
 printf("Access code: %li\n", accessCode);
 for(int i = 0; i < 10; i++)
 {
  printf("%s: %s\n", cheats[i], generateCode(array1, array2, accessCode, i));
 }
 return 0;
}
