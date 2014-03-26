include("nethook.lua")

local PlyMeta = FindMetaTable("Player")

function PlyMeta:PrintMessage(str)
	local netchan = self:GetNetChannel()
	local msg = nethook.NewMessage("svc_Print", str)

	netchan:SendNetMsg(msg)
end

local function HandleLuaFileDownload(data)
	print(string.format("File Number = %d, CRC = %d", data:ReadWord(), data:ReadLong()))
	file.Append("file.txt", util.Decompress(data:ReadBytes(data:GetNumBytesLeft())))
end
--[[
nethook.AddOutgoingHook("svc_GMod_ServerToClient", "TestOutput", function(msg)
	local data = msg:GetData()
	if data:GetNumBytesLeft() > 0 then
		local msgType = data:ReadByte()
		if msgType == 4 then
			HandleLuaFileDownload(data)
		elseif msgType == 0 then
			if data:ReadWord() == util.NetworkStringToID("pingas") then
				print(data:ReadString())
			end
		else
			print(msgType)
			hex_dump(data:ReadBytes(data:GetNumBytesLeft()))
		end
	end
end)

util.AddNetworkString("pingas")

function TestMessage()
	net.Start("pingas") for i=1,10 do net.WriteString("pingas") end net.Broadcast()
end

]]

nethook.AddOutgoingHook("svc_ServerInfo", "TestOutput", function(msg)
	hex_dump(msg:GetMapMD5())
	msg:SetMapMD5("\x1D\x34\xB6\x5A\x53\xC9\x42\xDC\x55\xD5\x1A\xE3\x68\xBE\xE7\x4A")
end)

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