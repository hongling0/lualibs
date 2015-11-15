#include <stdint.h>
#include <time.h>

#include <lauxlib.h>


struct queue{
    uint64_t read;
    uint64_t write;
    uint64_t size;
};

static int readonlyerr(lua_State* L){
    luaL_error(L,"don't change this readonly table");
    return 0;
}

static inline void setreadonly(lua_State* L,int objidx){
    lua_createtable(L,0,1);
    lua_pushcfunction(L,readonlyerr);
    lua_setfield(L,-2,"__newindex");
    lua_setmetatable(L,objidx);
}

static inline void luaqueue_grown(lua_State* L,struct queue* q,int functab,int valuetbl){
    uint64_t i;
    uint64_t newsz=q->size<<1;
    lua_createtable(L,newsz,0);
    setreadonly(L,-1);
    for(i=q->read;i<q->write;i++){
        lua_rawgeti(L,valuetbl,i&(q->size-1));
        lua_rawseti(L,-2,i&(newsz-1));
    }
    lua_pushnil(L);
    while(lua_next(L,functab)){
        lua_pushvalue(L,-3);
        lua_setupvalue(L,-2,2);
        lua_pop(L,1);
    }
    q->size=newsz;
}

static int luaqueue_push(lua_State* L){
    struct queue* q;

    luaL_checkany(L,1);
    q=lua_touserdata(L,lua_upvalueindex(1));
    lua_rawseti(L,lua_upvalueindex(2),q->write&(q->size-1));
    if((++q->write&(q->size-1))==(q->read&(q->size-1))){
        luaqueue_grown(L,q,lua_upvalueindex(3),lua_upvalueindex(2));
    }
    return 0;
}

static int luaqueue_head(lua_State* L){
    struct queue* q;
    q=lua_touserdata(L,lua_upvalueindex(1));
    if(q->read==q->write){
        lua_pushboolean(L,0);
        return 1;
    }
    lua_pushboolean(L,1);
    lua_rawgeti(L,lua_upvalueindex(2),q->read&(q->size-1));
    return 2;
}

static int luaqueue_pop(lua_State* L){
    struct queue* q;
    q=lua_touserdata(L,lua_upvalueindex(1));
    if(q->read==q->write){
        lua_pushboolean(L,0);
        return 1;
    }
    lua_pushboolean(L,1);
    lua_rawgeti(L,lua_upvalueindex(2),q->read&(q->size-1));
    lua_pushnil(L);
    lua_rawseti(L,lua_upvalueindex(2),q->read&(q->size-1));
    q->read++;
    return 2;
}

static int luaqueue_size(lua_State* L){
    struct queue* q=lua_touserdata(L,lua_upvalueindex(1));
    uint64_t sz=q->write-q->read;
    lua_pushinteger(L,sz);
    lua_pushinteger(L,q->size);
    return 2;
}

static int luaqueue_table(lua_State* L){
    lua_pushvalue(L,lua_upvalueindex(2));
    return 1;
}

static int luaqueue_create(lua_State* L){
    struct queue* q;
    uint64_t sz,n;

    static const struct luaL_Reg lib[]={
        {"push",luaqueue_push},
        {"pop",luaqueue_pop},
        {"head",luaqueue_head},
        {"size",luaqueue_size},
        {"table",luaqueue_table},
        {NULL,NULL}
    };
    n=(uint64_t)luaL_optinteger(L,1,64);
    sz=32;
    while(sz<n){
        sz<<=1;
    }

    luaL_newlibtable(L,lib);
    setreadonly(L,-1);

    q=(struct queue*)lua_newuserdata(L,sizeof(*q));
    q->read=0;
    q->write=0;
    q->size=sz;
    lua_createtable(L,sz,0);
    setreadonly(L,-1);
    lua_pushvalue(L,-3);
    luaL_setfuncs(L,lib,3);
    return 1;
}

int luaopen_queue_c(lua_State*L){
    static const struct luaL_Reg lib[]={
        {"new",luaqueue_create},
        {NULL,NULL}
    };
    luaL_newlib(L,lib);
    return 1;
}

