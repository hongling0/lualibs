# lua_uniq
64bit number uniq id generator
  
    local uniq=require 'uniq.c'
    uniq.init(0xFE)
    print(uniq.id(),uniq.str())


# lua_queue
lua fifo queue. can replace table.insert(t) table.remove((t,1)
  
    local queue=require 'queue.c'
    local q=queue.new(10000)
    for i=1,100000 do
        q.push(i)
    end
    for i=1,100000 do
        local ok,val=q.pop()
        assert(ok)
    end
    local ok,val=q.head()
    local save=q.table()
