#ifndef C_PROJ_WINSOCK_UTILS_H
#define C_PROJ_WINSOCK_UTILS_H

inline unsigned int atou(const char * s, unsigned int u) {
    if (*s < '0' || *s > '9') {
        return u;
    } else {
        return atou(s + 1, (*s - '0') + u * 10);
    }
}

inline char* utoa(char* s, unsigned int u) {
    if (u == 0) {
        *s = '0';
        *(s + 1) = '\0';
        return s;
    }
    char* p = utoa(s, u / 10);
    *p = '0' + (u % 10);
    *(p + 1) = '\0';
    return p + 1;
}

#endif
