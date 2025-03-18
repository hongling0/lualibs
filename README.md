# lua_uniq
64bit number uniq id generator
  
    local uniq=require 'uniq.c'
    uniq.init(0xFE)
    print(uniq.id(),uniq.str())


# lua_queue
lua fifo queue. can replace table.insert(t) table.remove(t,1)
  
    local q=require 'queue'()
    for i=1,100000 do
        q.push(i)
        if i % 2 == 1 then
            print(q.pop())
        end
    end
    print(q.top(), #q)


# lua string split
    
    local split=require 'split.c'
    print(split.splitrow(",asd,asd,,,asdasd,asd,,,",","))
    print(split.splitrow(",asd,asd,,,asdasd,asd,,,",",","i"))
    print('split table-------------------1')
    for i,v in ipairs(split.split(",asd,asd,,,asdasd,asd,,,",",")) do print(k,v) end
    print('split table-------------------2')
    for i,v in ipairs(split.split(",asd,asd,,,asdasd,asd,,,",",""i")) do print(k,v) end
