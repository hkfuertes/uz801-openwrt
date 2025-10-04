#include <unistd.h>
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <stdio.h>
#include <string.h>

#define LINUX_REBOOT_MAGIC1     0xfee1dead
#define LINUX_REBOOT_MAGIC2     0x28121969
#define LINUX_REBOOT_CMD_RESTART2 0xA1B2C3D4  // <-- Este es el correcto

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
        fprintf(stderr, "Modes: edl, bootloader, recovery, download\n");
        return 1;
    }
    
    sync();
    syscall(__NR_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, 
            LINUX_REBOOT_CMD_RESTART2, argv[1]);
    
    perror("reboot failed");
    return 1;
}
