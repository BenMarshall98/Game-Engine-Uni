local Vector3 = {}
Vector3.__index = Vector3

function Vector3:new(pX, pY, pZ)
	local self = setmetatable({}, Vector3)
	self.x = pX
	self.y = pY
	self.z = pZ
	return self
end

function Vector3:getX()
	return self.x
end

function Vector3:setX(pX)
	self.x = pX
end

function Vector3:getY()
	return self.y
end

function Vector3:setY(pY)
	self.y = pY
end

function Vector3:getZ()
	return self.z
end

function Vector3:setZ(pZ)
	self.z = pZ
end

function Vector3:add(pVector)
	local x = self.x + pVector:getX()
	local y = self.y + pVector:getY()
	local z = self.z + pVector:getZ()
	return Vector3:new(x, y, z)
end

function Vector3:subtract(pVector)
	local x = self.x - pVector:getX()
	local y = self.y - pVector:getY()
	local z = self.z - pVector:getZ()
	return Vector3:new(x, y, z)
end

function Vector3:multiply(pScalar)
	local x = self.x * pScalar
	local y = self.y * pScalar
	local z = self.z * pScalar
	return Vector3:new(x, y, z)
end

function Vector3:divide(pScalar)
	local x = self.x / pScalar
	local y = self.y / pScalar
	local z = self.z / pScalar
	return Vector3:new(x, y, z)
end

function Vector3:magnitude()
	local x2 = self.x * self.x
	local y2 = self.y * self.y
	local z2 = self.z * self.z
	return math.sqrt(x2 + y2 + z2)
end

function Vector3:normalise()
	local mag = self:magnitude()
	return self:divide(mag)
end

function Vector3:dotProduct(pVector)
	local x = self.x + pVector:getX()
	local y = self.y + pVector:getY()
	local z = self.z + pVector:getZ()
	return x + y + z
end

function NewVector3(x, y, z)
	return Vector3:new(x, y, z);
end

return Vector3;