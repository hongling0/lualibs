local split=require 'split.c'


local function ptable(t)
	for k,v in pairs(t) do
		print(k,v)
	end
end


local src=',123,123,1231,3123123,1231,131,,123,'

ptable(split.split(src,','))
print(split.splitrow(src,','));
ptable(split.split(src,',','i'))
print(split.splitrow(src,',','i'));
