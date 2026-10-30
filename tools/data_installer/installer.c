#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define SRC_DIR "/dev_hdd0/game/HALOCEDAT/USRDIR"
#define DST_DIR "/dev_hdd0/data/HaloCE"

static void copy_file(const char *src_path, const char *dst_path)
{
    FILE *src = fopen(src_path, "rb");
    if (!src) return;
    FILE *dst = fopen(dst_path, "wb");
    if (!dst) { fclose(src); return; }
    
    char buffer[1024 * 64];
    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, read_bytes, dst);
    }
    fclose(src);
    fclose(dst);
}

int main(int argc, char *argv[])
{
    mkdir("/dev_hdd0/data", 0777);
    mkdir(DST_DIR, 0777);
    mkdir(DST_DIR "/maps", 0777);

    DIR *dir = opendir(SRC_DIR "/maps");
    if (!dir) return 1;

    struct dirent *entry;
    char src[512], dst[512];

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        snprintf(src, sizeof(src), "%s/maps/%s", SRC_DIR, entry->d_name);
        snprintf(dst, sizeof(dst), "%s/maps/%s", DST_DIR, entry->d_name);
        copy_file(src, dst);
    }
    closedir(dir);

    snprintf(src, sizeof(src), "%s/config.toml", SRC_DIR);
    snprintf(dst, sizeof(dst), "%s/config.toml", DST_DIR);
    copy_file(src, dst);

    return 0;
}
