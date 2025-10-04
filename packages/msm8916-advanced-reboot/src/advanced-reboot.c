#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>

#define LINUX_REBOOT_MAGIC1     0xfee1dead
#define LINUX_REBOOT_MAGIC2     672274793
#define LINUX_REBOOT_CMD_RESTART2   0xA1B2C3D4

void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s [mode]\n", prog);
    fprintf(stderr, "\nModes:\n");
    fprintf(stderr, "  (empty)     - Normal reboot\n");
    fprintf(stderr, "  normal      - Normal reboot\n");
    fprintf(stderr, "  edl         - Reboot to EDL mode\n");
    fprintf(stderr, "  bootloader  - Reboot to fastboot/bootloader mode\n");
    fprintf(stderr, "  fastboot    - Reboot to fastboot/bootloader mode\n");
    fprintf(stderr, "  recovery    - Reboot to recovery mode\n");
}

int main(int argc, char *argv[]) {
    const char *mode = NULL;
    
    // Si no hay argumentos, reinicio normal
    if (argc == 1) {
        mode = NULL;
    } 
    // Si hay argumento, validar
    else if (argc == 2) {
        if (strcmp(argv[1], "normal") == 0) {
            mode = NULL;
        } else if (strcmp(argv[1], "edl") == 0) {
            mode = "edl";
        } else if (strcmp(argv[1], "bootloader") == 0 || strcmp(argv[1], "fastboot") == 0) {
            mode = "bootloader";
        } else if (strcmp(argv[1], "recovery") == 0) {
            mode = "recovery";
        } else {
            fprintf(stderr, "Error: Invalid mode '%s'\n\n", argv[1]);
            print_usage(argv[0]);
            return 1;
        }
    } 
    // Demasiados argumentos
    else {
        print_usage(argv[0]);
        return 1;
    }
    
    // Sincronizar filesystems antes de reiniciar
    sync();
    
    // Reiniciar según el modo
    if (mode == NULL) {
        // Reinicio normal
        printf("Rebooting normally...\n");
        if (syscall(__NR_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, 
                    LINUX_REBOOT_CMD_RESTART, NULL) != 0) {
            perror("reboot failed");
            return 1;
        }
    } else {
        // Reinicio con modo específico usando RESTART2
        printf("Rebooting to %s mode...\n", mode);
        if (syscall(__NR_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
                    LINUX_REBOOT_CMD_RESTART2, mode) != 0) {
            perror("reboot failed");
            return 1;
        }
    }
    
    // Si llegamos aquí, el reboot falló
    fprintf(stderr, "Error: System did not reboot\n");
    return 1;
}
