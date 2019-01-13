local Matrix4 = {}
Matrix4.__index = Matrix4

function Matrix4:new(pX1, pY1, pZ1, pW1, pX2, pY2, pZ2, pW2, pX3, pY3, pZ3, pW3, pX4, pY4, pZ4, pW4)
	local self = setmetatable({}, Matrix4)
	self.x1 = pX1
	self.y1 = pY1
	self.z1 = pZ1
	self.w1 = pW1
	self.x2 = pX2
	self.y2 = pY2
	self.z2 = pZ2
	self.w2 = pW2
	self.x3 = pX3
	self.y3 = pY3
	self.z3 = pZ3
	self.w3 = pW3
	self.x4 = pX4
	self.y4 = pY4
	self.z4 = pZ4
	self.w4 = pW4
	return self
end

return Matrix4