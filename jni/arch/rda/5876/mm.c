#include <string.h>
void *MemCpy(void *__dest,void *__src,size_t __n) {
    return memcpy(__dest,__src,__n);
}

void *MemSet(void *__s, int __c, size_t __n) {
    return memset(__s,__c,__n);
}

int MemCmp(const void *__s1,const void *__s2,size_t __n) {
    return memcmp(__s1,__s2,__n);
}

