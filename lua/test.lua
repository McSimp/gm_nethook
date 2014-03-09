include("nethook.lua")

local PlyMeta = FindMetaTable("Player")

function PlyMeta:PrintMessage(str)
	local netchan = self:GetNetChannel()
	local msg = nethook.NewMessage("svc_Print", str)

	netchan:SendNetMsg(msg)
end

nethook.AddOutgoingHook("svc_GMod_ServerToClient", "TestOutput", function(msg)
	local data = msg:GetData()
	print(data:GetNumBytesLeft())
	if data:GetNumBytesLeft() > 0 then
		print(data:ReadByte())
		hex_dump(data:ReadBytes(data:GetNumBytesLeft()))
	end
end)

util.AddNetworkString("pingas")

function TestMessage()
	net.Start("pingas") for i=1,100 do net.WriteString("pingas") end net.Broadcast()
end

function hex_dump(buf)
	for byte=1, #buf, 16 do
		local out = ""
		local chunk = buf:sub(byte, byte+15)
		out = out .. string.format('%08X  ',byte-1)
		chunk:gsub('.', function (c) out = out .. string.format('%02X ',string.byte(c)) end)
		out = out .. string.rep(' ',3*(16-#chunk))
		out = out .. ' ' .. chunk:gsub('%c','.')
		print(out)
	end
end
