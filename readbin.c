#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "score.h"

int main() {
    score t[10];
    int p[10];

    FILE* fp = fopen("data.bin", "r");
    
    fread(t, sizeof(t), 1, fp);
    fclose(fp);

    printf("Name,ID,Score\n");
    
    for (int i = 0; i < 10; ++i) {
        p[i] = i;
        printf("%d, %s, %d\n", t[i].id, t[i].name, t[i].score);
    }

    for (int i = 1; i < 10; ++i) {
        for (int j = i; j >= 1; j--) {
            if (t[p[j-1]].score < t[p[j]].score) {
                int k = p[j-1];
                p[j-1] = p[j];
                p[j] = k;
            }
        }
    }

    printf("Sorted:\n");

    for (int i = 0; i < 10; ++i) {
        printf("%d, %s, %d\n", t[p[i]].id, t[p[i]].name, t[p[i]].score);
    }
}
