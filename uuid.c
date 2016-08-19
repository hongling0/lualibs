#include <stdio.h>
#include <uuid/uuid.h>
#include <lauxlib.h>

typedef union c4{
    unsigned int n;
    unsigned char c[4];
} c4;

typedef union c2{
    unsigned short n;
    unsigned char c[2];
} c2;

static inline unsigned int u4(unsigned char t[4]){
    c4 c;
    c.c[0]=t[0];
    c.c[1]=t[1];
    c.c[2]=t[2];
    c.c[3]=t[3];
    return c.n;
}

static inline unsigned int u2(unsigned char t[2]){
    c2 c;
    c.c[0]=t[0];
    c.c[1]=t[1];
    return c.n;
}

static int gen(lua_State *L){
    static char str[40]={0};
    uuid_t uu;
    uuid_generate(uu);
    sprintf(str,"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            u4(&uu[0]),
            u2(&uu[4]),
            u2(&uu[6]),
            uu[8],
            uu[9],
            uu[10],
            uu[11],
            uu[12],
            uu[13],
            uu[14],
            uu[15]
           );
    lua_pushstring(L,str);
    return 1;
}


int luaopen_uuid_c(lua_State*L){
    static const struct luaL_Reg lib[]={
        {"gen",gen},
        {NULL,NULL}
    };
    luaL_newlib(L,lib);
    return 1;
}

