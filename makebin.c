#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "score.h"

unsigned int atou(const char * s, unsigned int u) {
    if (*s < '0' || *s > '9') {
        return u;
    } else {
        return atou(s + 1, (*s - '0') + u * 10);
    }
}

void parse(const char* s, score* t) {
    char * p = t->name;
    t->id = atou(s, 0);
    while (*s != ',') { ++s; }
    ++s;
    while (*s != ',') { *(p++) = *(s++); }
    ++s;
    *p = 0;
    t->score = atou(s, 0);
}

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

        parse(buf, &t[i]);
        printf("%d, %s, %d\n", t[i].id, t[i].name, t[i].score);
    }
    fwrite(t, sizeof(score), 10, out);
    
    fclose(in);
    fclose(out);
}
