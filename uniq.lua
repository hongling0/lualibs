local uniq=require 'uniq.c'

for k,v in pairs(uniq) do
    print(k,v)
end

local a=uniq.init(16383)


local function gen_id(max)
    for i=1,max do print(i,uniq.id(1)) end
end

local function gen_str(max)
    for i=1,max do print(i,uniq.str(1)) end
end

local function gen_num(max)
    for i=1,max do print(i,uniq.num(1,16),uniq.num(1,10),uniq.num(1,8)) end
end

local function gen_max()
    local i=0
    while true do
        i=i+1
        local ok,err=pcall(uniq.id,1)
        if not ok then
            print('gen_max',i)
            print(err)
            
            i=0
            os.execute("sleep 1")
            print(pcall(uniq.id,1))
        end
    end
end

gen_id(15)
gen_str(15)
gen_num(15)
gen_max()
