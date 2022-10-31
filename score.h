#ifndef C_PROJ_WINSOCK_SCORE_H
#define C_PROJ_WINSOCK_SCORE_H

#include "utils.h"

typedef struct {
    int id;
    char name[20];
    int score;
} score;

inline const char* next_field(const char * s) {
    while (*s != ',') { ++s; }
    ++s;
    while (*s == ' ') { ++s; }
    return s;
}

inline void parse_score_record_str(const char* s, score* t) {
    while (*s == ' ') { ++s; }
    t->id = atou(s, 0);

    s = next_field(s);
    char * p = t->name;
    while (*s != ',') { *(p++) = *(s++); }
    while (*p == ' ') { --p; }
    *(p + 1) = '\0';

    s = next_field(s);
    t->score = atou(s, 0);
}

inline void score_to_record_str(char* p, score* t) {
    utoa(p, t->id);
    while (*p != '\0') { ++p; }
    *(p++) = ',';
    
    char * q = t->name;
    while (*q != '\0') { *(p++) = *(q++); }
    *(p++) = ',';
    
    utoa(p, t->score);
    while (*p != '\0') { ++p; }
    *p = '\0';
}

#endif
