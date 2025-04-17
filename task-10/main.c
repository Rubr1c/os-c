#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <openssl/des.h>

void derive_key(const char *pw, DES_cblock *key_out, DES_key_schedule *ks_out) {
    DES_cblock key;
    memset(key, 0, sizeof(key));
    strncpy((char*)key, pw, 8);
    DES_set_odd_parity(&key);
    if (DES_set_key_checked(&key, ks_out) != 0) {
        fprintf(stderr, "Warning: DES key is weak or invalid.\n");
    }
    memcpy(key_out, &key, sizeof(key));
}

void des25_encrypt(const char *password, uint16_t salt, uint8_t out[8]) {
    DES_cblock key;
    DES_key_schedule ks;
    derive_key(password, &key, &ks);

    DES_cblock block;
    memset(block, 0, sizeof(block));
    block[0] = (salt >> 8) & 0xFF;
    block[1] = salt & 0xFF;

    for (int i = 0; i < 25; i++) {
        DES_ecb_encrypt(&block, &block, &ks, DES_ENCRYPT);
    }

    memcpy(out, block, 8);
}

static void print_hex(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", buf[i]);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char line[256];
    if (!fgets(line, sizeof(line), f)) {
        fprintf(stderr, "Error: input file is empty or unreadable.\n");
        fclose(f);
        return 1;
    }
    char *nl = strchr(line, '\n'); if (nl) *nl = '\0';
    uint16_t salt = (uint16_t)strtoul(line, NULL, 0);

    int entry = 1;
    while (fgets(line, sizeof(line), f)) {
        char *p = strchr(line, '\n'); if (p) *p = '\0';
        if (line[0] == '\0') continue;

        uint8_t hash[8];
        des25_encrypt(line, salt, hash);

        printf("Entry %2d: SALT=0x%04X  HASH=", entry++, salt);
        print_hex(hash, 8);
        printf("\n");
    }

    fclose(f);
    return 0;
}

