local process=require 'process.c'

for i=1,10 do
    local pid=process.fork(string.format("sleep %d;echo %d;",i,i))
    print(i,pid)
    --process.wait()
end

print('finsh')
