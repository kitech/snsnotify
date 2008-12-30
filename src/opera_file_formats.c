#include "opera_file_formats.h"

//some format code maybe found at
///home/kneworld/sources/src/KDE/kdelibs/kio/bookmarks/kbookmarkimporter_opera*

int read_opera_cookie_file()
{
    struct opera_file_header  fh =  {0};
    struct opera_cookie_record  dr = {0};
    struct opera_cookie_domain_record cdr = {0};
    char *file_path = "/home/gzl/.opera/cookies4.dat";
    FILE * fp = NULL;
    
    fp = fopen(file_path, "r+");    
    fread(&fh, sizeof(fh), 1, fp);

    printf("sizeof(uint64)=%d\n", sizeof(uint64));
    printf("vvvvv: %d, appv: %d\n", ntohl(fh.file_version_number), ntohl(fh.app_version_number));
    printf("file major ver: %d\n", ntohl(fh.file_version_number)>>12);
    printf("file minor ver: %d\n", ntohl(fh.file_version_number) & 0x00000fff);    
    printf("app major ver: %d\n", ntohl(fh.app_version_number)>>12);
    printf("app minor ver: %d\n", ntohl(fh.app_version_number) & 0x00000fff);
    printf("idtag: %d, appv: %d\n", ntohs(fh.idtag_length), ntohs(fh.length_length));
    
    fread(&dr, sizeof(dr), 1, fp);

    printf("tag id: %d\n", dr.tag_id);
    printf("length: %d\n", ntohs(dr.length));

    fread(&cdr, sizeof(cdr), 1, fp);
    printf("tag id: %d\n", ntohs(cdr.tag_id));
    printf("cdr value: %c\n", cdr.value);


    fclose(fp);
    
    return 0;
}
