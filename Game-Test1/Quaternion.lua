local Vector3 = require("Vector3")

local Quaternion = {}
Quaternion.__index = Quaternion

function Quaternion:new(pX, pY, pZ, pW)
	local self = setmetatable({}, Quaternion)
	self.x = pX
	self.y = pY
	self.z = pZ
	self.w = pW
	return self
end

function Quaternion:angleAxis(pAngle, pVector)
	local sinA = math.sin(pAngle / 2)
	local cosA = math.cos(pAngle / 2)
	local x = pVector:getX() * sinA
	local y = pVector:getY() * sinA
	local z = pVector:getZ() * sinA
	local w = cosA
	return Quaternion:new(x, y, z, w)
end

return Quaternion