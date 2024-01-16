#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#ifndef PRIVATE
#   define TKS_API extern
#else
#   define TKS_API static
#endif

#define STATIC static

#define RAND_STEPS 100

#define MAGIC_A 4523
#define MAGIC_B 6971
#define MAGIC_C 7481
#define MAGIC_D 7919


#define LOG(m) printf("%s\n", m);
#define LOGError(m) printf("Error: %s\n", m);

#define MALLOC(s) malloc(s);
#define FREE(ptr) free((void*)ptr);

typedef uint32_t u32;

STATIC char symbol_table[16] = {
    '0', '1', '2', '3', 
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

typedef struct uuid_state_st {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} uuid_state_t;


TKS_API uuid_state_t genUUID();

TKS_API void print_uuid(uuid_state_t uuid);
TKS_API const char* uuid_to_string(uuid_state_t uuid);

TKS_API void file_append_string(const char* fp, const char* str);

int main() {
    
    for (int i = 0; i < 1000000; ++i) {
        uuid_state_t n   = genUUID();
        const char* uuid = uuid_to_string(n);
        file_append_string("ids.txt", uuid);
        FREE(uuid);
    }

    return 0;
} 

TKS_API uuid_state_t genUUID() {
    uuid_state_t n = {
        .a = MAGIC_A,
        .b = MAGIC_B,
        .c = MAGIC_C,
        .d = MAGIC_D
    };

    for (int i = 0; i < RAND_STEPS; ++i) {
        n.a = n.a + rand()*MAGIC_A;
        n.b = n.b + rand()*MAGIC_B;
        n.c = n.c + rand()*MAGIC_C;
        n.d = n.d + rand()*MAGIC_D;
    }
    
    return n;
}

TKS_API void print_uuid(uuid_state_t uuid) {
    const char* str = uuid_to_string(uuid);

    if (!str) {
        LOGError("Couldn\t generate string of uuid state.");
        return;
    }

    LOG(str);
    FREE(str);
}

TKS_API const char* uuid_to_string(uuid_state_t uuid) {
    
    int digits       = 8;
    size_t len       = digits * 4;
    const char* buff = MALLOC(len + 1);
    int sg           = 4; // state groupings in 4 fields of ints.
 
    for (int j = 0; j < sg; ++j) {
        int n = ((int*)(&uuid))[j];

        
        for (int i = 0; i < digits; ++i) {
            int shift = 4*((digits - 1) - i);
            u32 mask  = 0xf0000000 >> (4 * i);
            int index = ((u32)(n & mask)) >> shift;
            ((char*)buff)[j*digits + i] = symbol_table[index];
        }
    }

    ((char*)buff)[len] = '\0';
    return buff;
}

TKS_API void file_append_string(const char* fp, const char* str) {
    FILE* fh = fopen(fp, "a");
    fwrite(str, 1, strlen(str), fh);
    fputc('\n', fh);
    fclose(fh);
}