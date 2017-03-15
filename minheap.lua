local c=require "minheap.c"

local obj=c.new()
local add=c.add
local pop=c.pop
local top=c.top

local random=math.random
for i=1,10 do
	local  expire=random(1,99999999)
	print(i,expire)
	add(obj,i,expire)
end
print(c.size(obj),"-----------------------------",top(obj))
repeat
	--print(top(obj))
	local id,expire=pop(obj,99999999)
	print(id,expire)
until (id==nil)