#include "plugintype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* type
    0: double;
    1: boolean;
    2: string;
    3: object;
    4: Null;
    5: undefine;
*/
#define NEW(type, size) (type *)malloc(sizeof(type) * (size))

struct _JSType {
    unsigned char type;
    void * data;
};

char *JSTypeToString (JSTypeRef ref)
{
    char *str = NULL;
    switch (ref->type){
        case 0:str = NEW(char, 30);
            sprintf(str, "%f", (double)ref->data);
            break;
        case 1:str = NEW(char, 12);
            sprintf(str, "%d", (int)ref->data);
            break;
        case 2:str = strdup((const char *)ref->data);
            break;
        case 3:str = strdup("OBJECT");
            break;
        case 4:str = strdup("");
            break;
        case 5:str = strdup("");
            break;
        default:str = strdup("");
            break;
    }
    return str;
}

double JSTypeToNumber (JSTypeRef ref)
{
    double f;
    switch(ref->type){
        case 0:f = (double)ref->data;
            break;
        case 1:f = (double)(int)ref->data;
            break;
        case 2:sscanf((char *)ref->data, "%lf", &f);
            break;
        case 3:f = 1.0;
            break;
        case 4:f = 0;
            break;
        case 5:f = 0;
            break;
        default:
            f = 0;
    }
    return f;
}

int JSTypeToBoolean (JSTypeRef ref)
{
    int b;
    switch(ref->type){
        case 0:b = ((double)ref->data != 0);
            break;
        case 1:b = (int)ref->data;
            break;
        case 2:b = (strcmp((char *)ref->data, "") != 0 &&
                    strcmp((char *)ref->data, "0") != 0 &&
                    strcmp((char *)ref->data, "false") != 0 &&
                    strcmp((char *)ref->data, "False") != 0 &&
                    strcmp((char *)ref->data, "FALSE") != 0);
            break;
        case 3:b = 1;
            break;
        case 4:b = 0;
            break;
        case 5:b = 0;
            break;
        default:
            b = 0;
            break;
    }
    return b;
}
