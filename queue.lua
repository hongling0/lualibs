return function()
	local read=1
	local write=1
	local cap=64
	local q={}
	local function size()
		local sz=write-read
		if sz>=0 then return sz,cap end
		return cap+sz,cap
	end
	local function pop()
		local val
		if read==write then return end
		val,q[read]=q[read],nil
		read=read+1
		if read>cap then read=1 end
		return val
	end
	local function top()
		if read==write then return end
		local val=q[read]
		return val
	end
	local function push(val)
		q[write]=val
		write=write+1
		if write>cap then write=1 end
		if write==read then
			local oq=q
			local owrite=write
			local oread=read
			local ocap=cap

			read=1
			write=1
			cap=ocap*2
			q={}
			for i=oread,ocap do
				push(oq[i])
			end
			for i=1,owrite-1 do
				push(oq[i])
			end
		end
	end
	local function pairs()
		local i=0
		local sz=size()
		return function()
			if i==sz then return end
			local idx=i+read
			if idx>cap then idx=idx-cap end
			i=i+1
			return i,q[idx]
		end
	end
	return setmetatable({
		pop=pop,
		push=push,
		size=size,
		top=top,
		--table=function() return q end,
	},{__pairs=pairs})
end
