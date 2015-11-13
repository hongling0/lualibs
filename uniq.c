#include <stdint.h>
#include <time.h>

#include <lauxlib.h>


#define UNIQ_MAIN_SHIFT 18
#define UNIQ_TIME_SHIFT 26
#define UNIQ_INCR_SHIFT 20

#define UNIQ_MAIN (1UL<<UNIQ_MAIN_SHIFT)
#define UNIQ_TIME (1UL<<UNIQ_TIME_SHIFT)
#define UNIQ_INCR (1UL<<UNIQ_INCR_SHIFT)

#define UNIQ_MAIN_MASK ((UNIQ_MAIN-1)<<(UNIQ_INCR_SHIFT+UNIQ_TIME_SHIFT))
#define UNIQ_TIME_MASK ((UNIQ_TIME-1)<<(UNIQ_INCR_SHIFT))
#define UNIQ_INCR_MASK ((UNIQ_INCR-1))


static uint64_t main_id=0;
static uint64_t last_inc=0;
static uint64_t max_inc=0;
static uint64_t last_time=0;


static inline uint64_t genid(){
    uint64_t now=(uint64_t)time(NULL);
    if(now!=last_time){
        last_inc=0;
        last_time=now;
    }else{
        last_inc++;
        if(last_inc>max_inc){
            max_inc=last_inc;
        }
    }
    return ((main_id<<(UNIQ_INCR_SHIFT+UNIQ_TIME_SHIFT))&UNIQ_MAIN_MASK)
        |((now<<(UNIQ_INCR_SHIFT))&UNIQ_TIME_MASK)
        |((last_inc)&UNIQ_INCR_MASK);
}

static inline const char* unit64_str(uint64_t u){
    static char c[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    static char tmp[17]={0};
    int i=0;
    for(;i<16;i++){
        tmp[i]=c[u>>(15-i)*4&0xf];
    }
    return tmp;
}

static int luauniq_init(lua_State* L){
   main_id=(uint64_t)luaL_checkunsigned(L,1); 
   return 0;
}

#define GEN_ID(val,L) \
    do { val=genid();\
    if(max_inc>=UNIQ_INCR){\
        max_inc=0;\
        luaL_error(L,"max support register %d uniqid per second",(int)(UNIQ_INCR-1));\
    }}while(0)

static int luauniq_genid(lua_State*L){
    int64_t val;
    GEN_ID(val,L);
    lua_pushunsigned(L,val);
    return 1;
}

static int luauniq_genstr(lua_State*L){
    int64_t val;
    GEN_ID(val,L);
    lua_pushstring(L,unit64_str(val));
    return 1;
}

int luaopen_uniq(lua_State*L){
    static const struct luaL_Reg lib[]={
        {"init",luauniq_init},
        {"id",luauniq_genid},
        {"str",luauniq_genstr}
    };
    luaL_newlib(L,lib);
    return 1;
}

