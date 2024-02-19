/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-10-18 16:21:06
 * @LastEditors: ruog__
 * @LastEditTime: 2023-10-18 16:21:07
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define ADC_ADDR 0xe0000a000
#define MEM_LEN 0x100

void *devm_map(unsigned long addr, int len, int *phandle)
{
    off_t offset;
    void *map_base;

    if ((*phandle = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
    {
        printf("cannot open '/dev/mem'\n");
        goto err_open;
    }

    /* offset for mmap() must be page aligned */
    offset = addr & ~(sysconf(_SC_PAGE_SIZE) - 1);

    map_base = mmap(NULL, len + addr - offset, PROT_READ | PROT_WRITE, MAP_SHARED, *phandle, offset);
    if (map_base == MAP_FAILED)
    {
        printf("mmap failed\n");
        goto err_mmap;
    }
    // printf("Memory mapped at address %p.\n", map_base);

    return map_base + addr - offset;

err_mmap:
    close(*phandle);

err_open:
    return NULL;
}

int main(void)
{
    static adcEnHdl = -1;
    unsigned int *adc_ddr = (unsigned int *)devm_map(ADC_ADDR, MEM_LEN, &adcEnHdl);
    if (adc_ddr == NULL)
    {
        printf("mmap fail!\n");
        goto error;
    }
error: 
    return 0;
}