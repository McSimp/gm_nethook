include("nethook.lua")

local PlyMeta = FindMetaTable("Player")

function PlyMeta:PrintMessage(str)
	local netchan = self:GetNetChannel()
	local msg = nethook.NewMessage("svc_Print", str)

	netchan:SendNetMsg(msg)
end

nethook.AddOutgoingHook("svc_GMod_ServerToClient", "TestOutput", function(msg)
	print(msg)
end)