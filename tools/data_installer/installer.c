#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sysmodule/sysmodule.h>
#include <sysutil/sysutil.h>

#define SRC_DIR "/dev_hdd0/game/HALOCEDAT/USRDIR"
#define DST_DIR "/dev_hdd0/data/HaloCE"

static void copy_file(const char *src_path, const char *dst_path, int index, int total, const char *name)
{
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        printf(" [%d/%d] Error leyendo: %s\n", index, total, name);
        return;
    }
    FILE *dst = fopen(dst_path, "wb");
    if (!dst) {
        printf(" [%d/%d] Error escribiendo: %s\n", index, total, name);
        fclose(src);
        return;
    }
    
    char buffer[1024 * 64];
    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, read_bytes, dst);
    }
    fclose(src);
    fclose(dst);
    printf(" [%d/%d] OK: %s\n", index, total, name);
}

int main(int argc, char *argv[])
{
    consoleInit();
    printf("\n");
    printf(" =================================================== \n");
    printf("        HALO: COMBAT EVOLVED PS3 - DATA INSTALLER    \n");
    printf(" =================================================== \n\n");
    printf(" Preparando carpetas de destino...\n");

    mkdir("/dev_hdd0/data", 0777);
    mkdir(DST_DIR, 0777);
    mkdir(DST_DIR "/maps", 0777);

    DIR *dir = opendir(SRC_DIR "/maps");
    if (!dir) {
        printf(" Error: No se encontro " SRC_DIR "/maps\n");
        printf(" Saliendo en 5 segundos...\n");
        sleep(5);
        sysProcessExit(1);
        return 1;
    }

    struct dirent *entry;
    char src[512], dst[512];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') count++;
    }
    rewinddir(dir);

    printf(" Copiando %d mapas a " DST_DIR "/maps/ ...\n\n", count);

    int current = 1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        snprintf(src, sizeof(src), SRC_DIR "/maps/%s", entry->d_name);
        snprintf(dst, sizeof(dst), DST_DIR "/maps/%s", entry->d_name);
        copy_file(src, dst, current, count, entry->d_name);
        current++;
    }
    closedir(dir);

    snprintf(src, sizeof(src), SRC_DIR "/config.toml");
    snprintf(dst, sizeof(dst), DST_DIR "/config.toml");
    FILE *test_cfg = fopen(src, "rb");
    if (test_cfg) {
        fclose(test_cfg);
        copy_file(src, dst, current, count + 1, "config.toml");
    }

    printf("\n =================================================== \n");
    printf("  INSTALACION DE DATOS COMPLETADA CON EXITO!\n");
    printf("  Los mapas ya estan listos en %s\n", DST_DIR "/maps");
    printf("  Ya puedes iniciar Halo CE desde el XMB.\n");
    printf("  (Puedes borrar este instalador si deseas liberar espacio)\n");
    printf(" =================================================== \n");
    printf(" Saliendo al XMB en 6 segundos...\n");
    sleep(6);
    sysProcessExit(0);
    return 0;
}
