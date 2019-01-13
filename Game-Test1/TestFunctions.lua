local Vector3 = require("Vector3")

function TestFunction(entity)
	local ComponentPosition = GetComponentPosition(entity)
	local currentPosition = GetPosition(ComponentPosition)
	local positionIncrease = Vector3:new(0, 1, 0)
	local newPosition = currentPosition:add(positionIncrease)
	SetPosition(ComponentPosition, newPosition)
end