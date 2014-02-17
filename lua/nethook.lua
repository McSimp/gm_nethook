require("nethook")

local nethook = nethook
local string = string
local isfunction = isfunction
local isstring = isstring
local table = table
local pairs = pairs
local error = error

local MessageFactory = nethook.MessageFactory

local WriteHooks = {}
local ProcessHooks = {}

local function AddInternal(msgName, hookTable, hookName, func)
	if not isfunction(func) then return end
	if not isstring(msgName) then return end

	-- No hooks exist for this message, so we need to attach to it
	if hookTable[msgName] == nil then
		nethook.AttachMessage(msgName)
		hookTable[msgName] = {}
	end

	hookTable[msgName][hookName] = func
end

local function RemoveInternal(msgName, hookTable, hookName)
	if not isstring(msgName) then return end
	if not hookTable[msgName] then return end

	hookTable[msgName][hookName] = nil

	-- If we don't have any remaining hooks for this message, we can safely
	-- detach from it.
	if table.Count(hookTable[msgName]) == 0 then
		nethook.DetachMessage(msgName)
	end
end

local function CallInternal(msgName, hookTable, msg)
	local hooks = hookTable[msgName]
	if hooks ~= nil then
		local ret
		for k,v in pairs(hooks) do 
			ret = v(msg)

			-- If we explicitly return a value from our hook, stop the rest of
			-- the hooks from executing and pass the value back into the module.
			if ret ~= nil  then
				return ret
			end
		end
	end
end

local function WriteCallback(msgName, msg)
	return CallInternal(msgName, WriteHooks, msg)
end

nethook.SetWriteCallback(WriteCallback)

local function ProcessCallback(msgName, msg)
	return CallInternal(msgName, ProcessHooks, msg)
end

-- nethook.SetProcessCallback(ProcessCallback)

-- ==========
-- Public API
-- ==========

function nethook.AddWriteHook(msgName, hookName, func)
	return AddInternal(msgName, WriteHooks, hookName, func)
end

function nethook.AddProcessHook(msgName, hookName, func)
	return AddInternal(msgName, ProcessHooks, hookName, func)
end

function nethook.RemoveWriteHook(msgName, hookName)
	return RemoveInternal(msgName, WriteHooks, hookName)
end

function nethook.RemoveProcessHook(msgName, hookName)
	return RemoveInternal(msgName, ProcessHooks, hookName)
end

function nethook.GetWriteHooks()
	return WriteHooks
end

function nethook.GetProcessHooks()
	return ProcessHooks
end

function nethook.NewMessage(msgName, ...)
	if not MessageFactory[msgName] then
		error(string.format("%s is not a valid message name", msgName))
	end

	return MessageFactory[msgName](...)
end
