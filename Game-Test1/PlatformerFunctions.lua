local Vector3 = require("Vector3")

function DeleteCoin(entity)
	DeleteEntity(entity)
end

function PlayerLeft(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local x = currentVelocity:getX()
		x = x - (deltaTime * inputValue * 1000)
		currentVelocity:setX(x)
		SetVelocity(ComponentPhysics, currentVelocity)
	end
end

function PlayerRight(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local x = currentVelocity:getX()
		x = x + (deltaTime * inputValue * 1000)
		currentVelocity:setX(x)
		SetVelocity(ComponentPhysics, currentVelocity)
	end
end

function PlayerJump(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		if GetTouchingGround(ComponentPhysics) then
			local currentImpulse = GetImpulse(ComponentPhysics)
			local y = currentImpulse:getY()
			y = y + (deltaTime * inputValue * 100)
			currentImpulse:setY(y)
			SetImpulse(ComponentPhysics, currentImpulse)
		end
	end
end