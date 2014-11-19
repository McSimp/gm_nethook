include("nethook.lua")

local PlyMeta = FindMetaTable("Player")

function PlyMeta:PrintMessage(str)
	local netchan = self:GetNetChannel()
	local msg = nethook.NewMessage("svc_Print", str)

	netchan:SendNetMsg(msg)
end

local serverInfoFormat = [[
svc_ServerInfo:
Protocol: %d
Server Count: %d
Is Dedicated: %s
Is HLTV: %s
OS: %s
Client CRC: %d
Map MD5: %s
Max Clients: %d
Max Classes: %d
Player Slot: %d
Tick Interval: %f
Game Dir: %s
Map Name: %s
Sky Name: %s
Host Name: %s
Loading URL: %s
Gamemode: %s
]]

nethook.AddOutgoingHook("svc_ServerInfo", "TestServerInfo", function(msg)
	print(string.format(serverInfoFormat, 
		msg:GetProtocol(),
		msg:GetServerCount(),
		tostring(msg:GetIsDedicated()),
		tostring(msg:GetIsHLTV()),
		msg:GetOS(),
		msg:GetClientCRC(),
		msg:GetMapMD5(),
		msg:GetMaxClients(),
		msg:GetMaxClasses(),
		msg:GetPlayerSlot(),
		msg:GetTickInterval(),
		msg:GetGameDir(),
		msg:GetMapName(),
		msg:GetSkyName(),
		msg:GetHostName(),
		msg:GetLoadingURL(),
		msg:GetGamemode()
	))

	msg:SetOS('l')
	msg:SetLoadingURL("http://steamcommunity.com")
end)

local function HandleLuaFileDownload(data)
	if true then
		hex_dump(data:ReadBytes(data:GetNumBytesLeft()))
		return
	end

	local info = string.format("File Number = %d, CRC = %d\n", data:ReadWord(), data:ReadLong())
	print(info)
	file.Append("file.txt", info .. util.Decompress(data:ReadBytes(data:GetNumBytesLeft())))
end

nethook.AddOutgoingHook("svc_GMod_ServerToClient", "TestOutput", function(msg)
	local data = msg:GetData()
	if data:GetNumBytesLeft() > 0 then
		local msgType = data:ReadByte()
		if msgType == 4 then
			HandleLuaFileDownload(data)
		elseif msgType == 0 then
			if data:ReadWord() == util.NetworkStringToID("pingas") then
				file.Write("file.txt", data:ReadBytes(data:GetNumBytesLeft()))
			end
		else
			print(msgType)
			hex_dump(data:ReadBytes(data:GetNumBytesLeft()))
		end
	end
end)

function PlyMeta:TestFile()
	local netchan = self:GetNetChannel()

	local f = file.Open("spoofedfile.txt", "rb", "DATA")
	local filedata = f:Read(f:Size())
	local writer = nethook.bf_write(f:Size() + 500)
	writer:WriteByte(1)
	writer:WriteString("!RELOAD")
	writer:WriteString("lua/includes/extensions/client/entity.lua")
	writer:WriteString("lua/includes/extensions/client/entity.lua")
	writer:WriteUBitLong(4 + f:Size(), 32)
	writer:WriteUBitLong(821089979, 32)
	writer:WriteBytes(filedata, f:Size())

	local msg = nethook.NewMessage("svc_GMod_ServerToClient")
	msg:SetData(writer)
	netchan:SendNetMsg(msg)
end

function PlyMeta:TestFile2()
	local netchan = self:GetNetChannel()

	local f = file.Open("spoofedfile.txt", "rb", "DATA")
	local filedata = f:Read(f:Size())
	local writer = nethook.bf_write(f:Size() + 100)
	writer:WriteByte(4)
	writer:WriteShort(157)
	writer:WriteUBitLong(821089979, 32)
	writer:WriteBytes(filedata, f:Size())

	local msg = nethook.NewMessage("svc_GMod_ServerToClient")
	msg:SetData(writer)
	netchan:SendNetMsg(msg)
end


function lel()
	local data = file.Read("spoofedfile.txt")
	print(hex_dump(data))
end

util.AddNetworkString("pingas")
util.AddNetworkString("pingascl")

function TestMessage()
	net.Start("pingas") net.WriteData("abc", 3) net.Broadcast()
end

function PlyMeta:SendPingas(str)
	local netchan = self:GetNetChannel()
	
	local writer = nethook.bf_write(10)
	writer:WriteByte(0)
	writer:WriteWord(util.NetworkStringToID("pingas"))
	writer:WriteBytes("abc", 3)

	local msg = nethook.NewMessage("svc_GMod_ServerToClient")
	msg:SetData(writer)

	netchan:SendNetMsg(msg)
end


net.Receive("pingascl", function()
	file.Write("file.txt", net.ReadData(65500))
end)

--[[
nethook.AddOutgoingHook("svc_ServerInfo", "TestOutput", function(msg)
	hex_dump(msg:GetMapMD5())
	msg:SetMapMD5("\x1D\x34\xB6\x5A\x53\xC9\x42\xDC\x55\xD5\x1A\xE3\x68\xBE\xE7\x4A")
end)
]]

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