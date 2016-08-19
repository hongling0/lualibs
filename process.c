#include <errno.h>
#include <stdio.h>  
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>  

#include <lua.h>
#include <lauxlib.h>

static int luapid_gc(lua_State*L){
	pid_t *pid=lua_touserdata(L,1);
	if(*pid>0){
		waitpid(*pid,NULL,0);
	}
	return 0;
}

static inline void create_meta(lua_State*L){
	lua_createtable(L,0,1);
	lua_pushcfunction(L,luapid_gc);
	lua_setfield(L,-2,"__gc");
}

static inline void attach(lua_State*L,pid_t fpid){
	pid_t *pid;

	lua_pushnumber(L,fpid);
	pid=lua_newuserdata(L,sizeof(*pid));
	*pid=fpid;
	lua_pushvalue(L,lua_upvalueindex(2));
	lua_setmetatable(L,-2);
	
	lua_rawset(L,lua_upvalueindex(1));
}

static inline void detach(lua_State*L,pid_t fpid){
	lua_pushnumber(L,fpid);
	lua_pushvalue(L,-1);
	lua_rawget(L,lua_upvalueindex(1));
	pid_t *pid=lua_touserdata(L,-1);
	*pid=0;
	lua_pop(L,1);
	lua_pushnil(L);
	lua_rawset(L,lua_upvalueindex(1));
}

static int luafork(lua_State*L){
	const char* cmd=luaL_checkstring(L,1);
	pid_t fpid=fork();
	if(fpid<0){
		luaL_error(L,strerror(errno));
		return 0;
	}else if(fpid>0){
		attach(L,fpid);
		lua_pushnumber(L,fpid);
		return 1;
	}else if(cmd!=NULL){
		if(execl("/bin/sh","sh","-c",cmd,NULL)!=0){
			luaL_error(L,strerror(errno));
		}
		return 0;
	}else{
		return 0;
	}
}

static int luawait(lua_State*L){
	int status;
	pid_t fpid=wait(&status);
	if(fpid>0){
		detach(L,fpid);
		lua_pushnumber(L,fpid);
		return 1;
	}else{
		luaL_error(L,strerror(errno));
		return 0;
	}
}

int luaopen_process_c(lua_State*L){
	static const struct luaL_Reg lib[]={
		{"fork",luafork},
		{"wait",luawait},
		{NULL,NULL}
	};
	luaL_checkversion(L);
	luaL_newlibtable(L,lib);
	lua_newtable(L);
	create_meta(L);
	luaL_setfuncs(L,lib,2);
	return 1;
}

