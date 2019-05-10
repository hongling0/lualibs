# lua_uniq
64bit number uniq id generator
  
    local uniq=require 'uniq.c'
    uniq.init(0xFE)
    print(uniq.id(),uniq.str())


# lua_queue
lua fifo queue. can replace table.insert(t) table.remove((t,1)
  
    local queue=require 'queue.c'
    local q=queue()
    for i=1,100000 do
        q.push(i)
    end
    for i=1,100000 do
        local ok,val=q.pop()
        assert(ok)
    end
    local ok,val=q.head()
    for i,v in pairs(q) do
      print(i,v)
    end


# lua string split
    
    local split=require 'split.c'
    print(split.splitrow(",asd,asd,,,asdasd,asd,,,",",")
    print(split.splitrow(",asd,asd,,,asdasd,asd,,,",",","i")
    print('split table-------------------1')
    for i,v in ipairs(split.split(",asd,asd,,,asdasd,asd,,,",",")) do print(k,v) end
    print('split table-------------------2')
    for i,v in ipairs(split.split(",asd,asd,,,asdasd,asd,,,",",""i")) do print(k,v) end
