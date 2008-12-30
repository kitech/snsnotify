#ifndef OPERA_FILE_FORMATS_H
#define OPERA_FILE_FORMATS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <WinSock.h>
#else
#include <arpa/inet.h>
#endif

#ifndef byte
#define byte  unsigned char
#endif
#ifndef uint8
#define uint8  unsigned char
#endif
#ifndef uint16 
#define uint16  unsigned short
#endif
#ifndef uint32 
#define uint32  unsigned int
#endif
#ifndef uint64
#define uint64  unsigned long long int
#endif

/*
1	0x7f
2	0x7fff
3	0x7fffff
4	0x7fffffff
 */
#define MSB8 0x7f
#define MSB16 0x7fff
#define MSB24 0x7fffff
#define MSB32 0x7fffffff

    struct opera_file_header {
        uint32 file_version_number;
        uint32 app_version_number;
        // number of bytes in the id tag, presently 1
        uint16 idtag_length;   
        // number of bytes in the length part of a record, presently 2
        uint16 length_length;          
    };

    struct opera_cookie_record {
        uint8 tag_id;
        uint16 length;
        //byte * payload;
    };
    
    struct opera_cookie_domain_record {
        uint16 tag_id;
        uint8  value;
    };

    int read_opera_cookie_file();

#ifdef __cplusplus
};
#endif
#endif









