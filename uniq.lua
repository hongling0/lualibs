local uniq=require 'uniq.c'

for k,v in pairs(uniq) do
    print(k,v)
end

uniq.init(0xFF)


local function gen_id(max)
    for i=1,max do print(i,uniq.id()) end
end

local function gen_str(max)
    for i=1,max do print(i,uniq.str()) end
end

local function gen_max()
    local i=0
    while true do
        i=i+1
        local ok,err=pcall(uniq.id)
        if not ok then
            print('gen_max',i)
            print(err)
            break
        end
    end
end

gen_id(15)
gen_str(15)
gen_max()
