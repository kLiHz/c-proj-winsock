#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "score.h"

int main() {
    score t[10];

    FILE* in = fopen("src.csv", "r");
    FILE* out = fopen("data.bin", "w");
    
    char buf[100], c;
    int k = 0;
    
    for (int i = 0; i < 10; ++i) {
        k = 0;
        c = '\0';
        while (c != '\n') {
            fscanf(in, "%c", &c);
            buf[k] = c;
            k += 1;
        }
        buf[k] = '\0';

        parse_score_record_str(buf, &t[i]);
        printf("%d, %s, %d\n", t[i].id, t[i].name, t[i].score);
    }

    // Dump to binary file
    fwrite(t, sizeof(score), 10, out);
    
    fclose(in);
    fclose(out);
}
