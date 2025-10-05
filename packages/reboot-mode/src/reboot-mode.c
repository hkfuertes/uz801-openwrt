#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/reboot.h>
#include <linux/reboot.h>

#define TCSR_BASE           0x01937000
#define TCSR_BOOT_MISC      0xD100      // Offset dentro de TCSR
#define TCSR_SIZE           0x1000

// Valores de modo para msm8916
#define MODE_NORMAL         0x77665501
#define MODE_BOOTLOADER     0x77665500
#define MODE_RECOVERY       0x77665502
#define MODE_EDL            0x77665503
#define MODE_DOWNLOAD       0x646f776e

int write_reboot_mode(unsigned int mode) {
    int fd;
    void *map_base, *virt_addr;
    unsigned long target = TCSR_BASE + TCSR_BOOT_MISC;
    
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Error opening /dev/mem");
        return -1;
    }
    
    // Mapear la región TCSR
    map_base = mmap(0, TCSR_SIZE, PROT_READ | PROT_WRITE, 
                    MAP_SHARED, fd, TCSR_BASE);
    if (map_base == MAP_FAILED) {
        perror("Error mapping memory");
        close(fd);
        return -1;
    }
    
    // Calcular dirección virtual del offset
    virt_addr = map_base + TCSR_BOOT_MISC;
    
    // Escribir el valor del modo
    *((unsigned int *) virt_addr) = mode;
    
    // Asegurar que se escribió
    msync(map_base, TCSR_SIZE, MS_SYNC);
    
    munmap(map_base, TCSR_SIZE);
    close(fd);
    
    return 0;
}

int main(int argc, char *argv[]) {
    unsigned int mode;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
        fprintf(stderr, "Modes: edl, bootloader, recovery, download\n");
        return 1;
    }
    
    // Determinar el modo
    if (strcmp(argv[1], "edl") == 0) {
        mode = MODE_EDL;
    } else if (strcmp(argv[1], "bootloader") == 0) {
        mode = MODE_BOOTLOADER;
    } else if (strcmp(argv[1], "recovery") == 0) {
        mode = MODE_RECOVERY;
    } else if (strcmp(argv[1], "download") == 0) {
        mode = MODE_DOWNLOAD;
    } else {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        return 1;
    }
    
    // Escribir el valor en TCSR
    if (write_reboot_mode(mode) != 0) {
        fprintf(stderr, "Failed to write reboot mode\n");
        return 1;
    }
    
    printf("Rebooting to %s mode...\n", argv[1]);
    
    // Sincronizar y reiniciar
    sync();
    reboot(RB_AUTOBOOT);
    
    perror("reboot failed");
    return 1;
}
