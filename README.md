# lua_uniq
64bit number uniq id generator
  
    local uniq=require 'uniq'
    uniq.init(0xFE)
    print(uniq.id(),uniq.str())
