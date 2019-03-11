local Vector3 = require("Scripts/Vector3")
local Quaternion = require("Scripts/Quaternion")

function DeleteCoin(entity)
	PlayAudio("Collectable")
	DeleteEntity(entity)
end

function SpinCoin(entity, deltaTime)
	local ComponentDirection = GetComponentDirection(entity)
	local ComponentState = GetComponentState(entity)

	local currentDirection = GetValue(ComponentState, "CurrentDirection", "float", 0)
	currentDirection = currentDirection + (deltaTime * 150 * math.pi / 180)
	SetValue(ComponentState, "CurrentDirection", currentDirection)
	local up = Vector3:new(0, 1, 0)
	local direction = Quaternion:angleAxis(currentDirection, up)

	SetDirection(ComponentDirection, direction)
end