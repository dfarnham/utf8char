#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdarg.h>

/*
 * A simple utf8 char stream reader to test my understanding UTF-8
 */

/*
 * UTF-8
 *
 *  Number  | Bits for   |    First   |    Last    |          |          |          |          |
 * of bytes | code point | code point | code point |  Byte 1  |  Byte 2  |  Byte 3  |  Byte 4  |
 * ---------------------------------------------------------------------------------------------
 *     1    |     7      |   U+0000   |  U+007F    | 0xxxxxxx |          |          |          |
 *     2    |    11      |   U+0080   |  U+07FF    | 110xxxxx | 10xxxxxx |          |          |
 *     3    |    16      |   U+0800   |  U+FFFF    | 1110xxxx | 10xxxxxx | 10xxxxxx |          |
 *     4    |    21      |   U+10000  |  U+10FFFF  | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |
 */

/*
 * Local functions
 */
typedef struct {
    char size; // 1-4
    unsigned char *charStr;
} UTF8Char;

typedef struct {
    unsigned int size;  // number of bytes
    unsigned int len;   // number of utf8 chars
    unsigned char **ch;
} UTF8String;



void          error                   (const char *, ...                   );
int           validateUTF8            (unsigned char *                     );
int           UTF8CharSize            (unsigned char *                     );
UTF8String *  utf8string_new          (unsigned char *                     );
void          utf8string_outputString (FILE *, UTF8String *, char *, char *);
void          utf8string_outputChar   (FILE *, unsigned char *             );
void          utf8string_destroy      (UTF8String *                        );
int           main                    (int, char *[]                       );


void
error(const char *str, ...) {
    va_list vptr;
    char buf[BUFSIZ];

    va_start(vptr, str);
    vsprintf(buf, str, vptr);
    va_end(vptr);

    fputs(buf, stderr);
    exit(1);
}

// returns 0 if the utf8 char is invalid
// returns a number [1,4] representing the number of bytes in the valid utf8 char
int
validateUTF8(unsigned char *ptr) {
    int n = UTF8CharSize(ptr);

    for (int i = 1; i < n; i++) {
        if ((ptr[i] & 0xc0) != 0x80) return 0;
    }
    return n;
}

// returns the number of bytes representing the utf8 char [1,4]
int
UTF8CharSize(unsigned char *ptr) {
    int num = 1;

    if (*ptr >= 0xf0) {
        num = 4;
    } else if (*ptr >= 0xe0) {
        num = 3;
    } else if (*ptr >= 0xc0) {
        num = 2;
    }

    return num;
}

UTF8String *
utf8string_new(unsigned char *str) {
    int num = 0;
    unsigned char *ptr = str;
    UTF8String *utf8string;
    
    if (!(utf8string = (UTF8String *)malloc(sizeof(UTF8String)))) {
        error("Can't malloc() UTF8String");
    }
    utf8string->size = 0;
    utf8string->len = 0;

    // count the number of utf8 chars in the input string
    // count the number of bytes in the input string
    // validate the utf8 chars
    while (*ptr) {
        if (!(num = validateUTF8(ptr))) {
            error("Malformed UTF-8: required %d bytes\n", UTF8CharSize(ptr));
        }
        utf8string->size += num;
        utf8string->len++;
        ptr += num;
    }

    // allocate char pointer array
    if (!(utf8string->ch = (unsigned char **)malloc(utf8string->len * sizeof(unsigned char *)))) {
        error("Can't malloc() UTF8String *");
    }

    // assign pointers
    ptr = str;
    for (int i = 0; i < utf8string->len; i++) {
        utf8string->ch[i] = ptr;
        ptr += UTF8CharSize(ptr);
    }

    return utf8string;
}

void
utf8string_outputString(FILE *fp, UTF8String *utf8string, char *prefixStr, char *postfixStr) {
    for (int i = 0; i < utf8string->len; i++) {
        if (prefixStr) fprintf(fp, "%s", prefixStr);
        utf8string_outputChar(fp, utf8string->ch[i]);
        if (postfixStr) fprintf(fp, "%s", postfixStr);
    }
}

void
utf8string_outputChar(FILE *fp, unsigned char *utf8char) {
    int num = UTF8CharSize(utf8char);
    for (int i = 0; i < num; i++) {
        fprintf(fp, "%c", utf8char[i]);
    }
}

void
utf8string_destroy(UTF8String *utf8string) {
    free(utf8string->ch);
    free(utf8string);
}

int
main(int argc, char *argv[]) {
    unsigned char buf[BUFSIZ];
    char *prefixStr = "";
    char *postfixStr = "";

    if (argc == 2 || argc == 3) {
        prefixStr = argv[1];
        if (argc == 3) postfixStr = argv[2];
    }

    while (fgets((char *)buf, sizeof buf, stdin)) {
        UTF8String *utf8string = utf8string_new(buf);
        utf8string_outputString(stdout, utf8string, prefixStr, postfixStr);
        utf8string_destroy(utf8string);
    }
}
