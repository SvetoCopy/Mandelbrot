#include "NoAVXMandelbrot/Mandelbrot.h"
#include <cstdint>

uint64_t rdtsc(){
    unsigned int lo, hi;
    __asm__ __volatile__ ( 
      "xorl %%eax, %%eax\n" 
      "cpuid\n" 
      "rdtsc" 
      : "=a" (lo), "=d" (hi) 
      : 
      : "%ebx", "%ecx"); 
    return ((uint64_t)hi << 32) | lo;
}

int main()
{
    Mandelbrot mandelbrot = {800, 600};

    uint64_t start = rdtsc();

    for (int i = 0; i < 30; i++) {
        mandelbrot.CalculatePixels(0, 0, 1);
    }
    
    uint64_t end = rdtsc();
    printf("rdtsc : %lu\n", (end - start) / 30);

    return 0;
}