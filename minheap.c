#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>

struct min_heap {
	size_t cap;
	size_t num;
	int64_t *array;
};

static inline void
heap_resize(struct min_heap *heap) {
	size_t cap, ncap, i;
	int64_t *array, *narray;

	cap = heap->cap;
	array = heap->array;
	ncap = cap > 0 ? (cap << 1) : 64 ;
	narray = (int64_t*)malloc(sizeof(*narray) * (ncap));
	for (i = 0; i < cap; i++) {
		narray[i] = array[i];
	}
	heap->array = narray;
	heap->cap = ncap;
	free(array);
}

static inline  void
heap_shiftup(struct min_heap *heap, size_t hole, int64_t val) {
	int64_t *array = heap->array;
	while (hole > 0) {
		size_t parent = (hole - 1) / 2;
		if (array[parent] < val) {
			break;
		}
		array[hole] = array[parent];
		hole = parent;
	}
	array[hole] = val;
}

static inline void
heap_shiftdown(struct min_heap *heap, size_t hole, int64_t val) {
	int64_t *array = heap->array;
	size_t num = heap->num;
	size_t half = num / 2;
	while (hole < half) {
		size_t child = 2 * hole + 1;
		size_t right = child + 1;
		if (right < num && array[child] > array[right]) {
			child = right;
		}
		if (val < array[child]) {
			break;
		}
		array[hole] = array[child];
		hole = child;
	}
	array[hole] = val;
}

static inline void
heap_add(struct min_heap *heap, int64_t val) {
	if ((heap->num + 1) >= heap->cap ) {
		heap_resize(heap);
	}
	heap_shiftup(heap, heap->num++, val);
}

static inline void
heap_free(struct min_heap *heap) {
	free(heap->array);
}

static inline int64_t
heap_top(struct min_heap * heap) {
	if (heap->num == 0) {
		return 0;
	}
	return heap->array[0];
}

static inline int64_t
heap_pop(struct min_heap * heap, int64_t cmp) {
	if (heap->num == 0) {
		return 0;
	}
	int64_t min = heap->array[0];
	if (min <= cmp) {
		heap_shiftdown(heap, 0, heap->array[--heap->num]);
		return min;
	} else {
		return 0;
	}
}

static inline struct min_heap *
lcheck_timeheap(lua_State *L, int idx) {
	struct min_heap * heap = (struct min_heap *)lua_touserdata(L, idx);
	if (!heap) {
		luaL_error(L, "expected a min_heap but got a %s(#%d)", luaL_typename(L, idx), idx);
	}
	return heap;
}

static int
lminheap_gc(lua_State *L) {
	struct min_heap * heap = lcheck_timeheap(L, 1);
	heap_free(heap);
	return 1;
}

static int
lminheap_new(lua_State *L) {
	struct min_heap *heap;
	heap = (struct min_heap*)lua_newuserdata(L, sizeof(*heap));
	heap->array = NULL;
	heap->cap = heap->num = 0;
	lua_createtable(L, 0, 1);
	lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, lminheap_gc);
	lua_rawset(L, -3);
	lua_setmetatable(L, -2);
	return 1;
}

static int
lminheap_add(lua_State *L) {
	struct min_heap * heap = lcheck_timeheap(L, 1);
	int64_t val = luaL_checkinteger(L, 2);
	heap_add(heap, val);
	return 0;
}

static int
lminheap_pop(lua_State *L) {
	struct min_heap * heap = lcheck_timeheap(L, 1);
	int64_t val = luaL_checkinteger(L, 2);
	lua_pushinteger(L, heap_pop(heap, val));
	return 1;
}

static int
lminheap_top(lua_State *L) {
	struct min_heap * heap = lcheck_timeheap(L, 1);
	lua_pushinteger(L, heap_top(heap));
	return 1;
}

static int
lminheap_size(lua_State *L) {
	struct min_heap * heap = lcheck_timeheap(L, 1);
	lua_pushinteger(L, heap->num);
	return 1;
}



int luaopen_minheap_c(lua_State*L) {
	static const struct luaL_Reg lib[] = {
		{"new", lminheap_new},
		{"add", lminheap_add},
		{"pop", lminheap_pop},
		{"top", lminheap_top},
		{"size", lminheap_size},
		{NULL, NULL}
	};
	luaL_newlib(L, lib);
	return 1;
}