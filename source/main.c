#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <dirent.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv) {

    VIDEO_Init();
    WPAD_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    console_init(xfb, 20, 20, rmode->fbWidth - 20, rmode->xfbHeight - 20,
                 rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("\x1b[2;0H");
    printf("[+] WiiBoot - Emulated Processing Unit Launcher\n");
    printf("[!] I DO NOT TAKE ANY RESPONSIBILITY FOR WHATEVER HAPPENS TO YOUR WII.\n");
    printf("[!] PLEASE INSTALL PRELOADER/PRIILOADER BEFORE USING THIS SOFTWARE.\n");
    printf("[+] Press A to install an EPU.\n");
    printf("[+] Press B or HOME to exit.\n\n");

    // Initialize FAT/SD
    if (!fatInitDefault()) {
        printf("[-] Failed to initialize SD card.\n");
        printf("[-] Insert an SD card and try again.\n");
        printf("[-] Press HOME to exit.\n");

        while (1) {
            WPAD_ScanPads();
            if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
            VIDEO_WaitVSync();
        }
    }

    printf("[+] SD card initialized successfully.\n");

    // Open EPU directory
    DIR *dir = opendir("sd:/epu");
    if (!dir) {
        printf("[-] Folder 'sd:/epu' not found.\n");
        printf("[-] Create it and place at least one .epu file inside.\n");
        printf("[-] Press HOME to exit.\n");

        while (1) {
            WPAD_ScanPads();
            if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
            VIDEO_WaitVSync();
        }
    }

    // Count EPU files
    struct dirent *entry;
    int epu_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".epu")) {
            epu_count++;
        }
    }
    closedir(dir);

    if (epu_count == 0) {
        printf("[-] No .epu files found in sd:/epu.\n");
        printf("[-] Add at least one EPU file.\n");
        printf("[-] Press HOME to exit.\n");

        while (1) {
            WPAD_ScanPads();
            if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
            VIDEO_WaitVSync();
        }
    }

    printf("[+] Found %d EPU file(s).\n", epu_count);

    // Main loop
    while (1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_HOME) exit(0);
        if (pressed & WPAD_BUTTON_B) exit(0);

        if (pressed & WPAD_BUTTON_A) {
            printf("[+] Installing EPU... (not implemented yet)\n");
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
/*
this homebrew project has been made possible thanks to the work of the WiiDev Team and support from The Countryballs Group on Discord.
Please note that this software is provided "as is" without any warranties. Use it at your own risk. The developers are not responsible for any damage to your Wii or data loss.
Special Thanks to:
- petrofizkulture
- and others from the WiiDev Team and The Countryballs Group who contributed to testing and development.
*/
