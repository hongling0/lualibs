local queue=require 'queue.c'

for k,v in pairs(queue) do
    print(k,v)
end

local q=queue.new(100)
print('size',q.size())
for i=1,2500000 do
    q.push(i)
end
print('size',q.size())


for i=1,2500000 do
    local ok,val=q.pop()
    assert(ok)
end
print('size',q.size())

for i=1,2500000 do
    q.push(nil)
end
print('size',q.size())


for i=1,2500000 do
    local ok,val=q.pop()
    q.push(i)
    q.push(i)
    assert(ok)
end
print('size',q.size())
local i=0
for k,v in pairs(q.table()) do
    i=i+1
end
print('head',i,q.head())
