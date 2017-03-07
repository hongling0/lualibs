local c=require "minheap.c"

local obj=c.new()
local add=c.add
local pop=c.pop
local top=c.top

local random=math.random
for i=1,100 do
	add(obj,random(1,99999999))
end
print(c.size(obj))
repeat
	print(top(obj))
	n=pop(obj,99999999)
	print(n)
until (n==0)