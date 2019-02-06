local Vector3 = require("Vector3")

function DeleteCoin(entity)
	DeleteEntity(entity)
end

function PlayerLeft(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentRotation = GetRotation(ComponentPhysics)
		local y = currentRotation:getX()
		y = y + (deltaTime * inputValue * 250)
		currentRotation:setY(y)
		SetRotation(ComponentPhysics, currentRotation);
	end
end

function PlayerRight(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentRotation = GetRotation(ComponentPhysics)
		local y = currentRotation:getX()
		y = y - (deltaTime * inputValue * 250)
		currentRotation:setY(y)
		SetRotation(ComponentPhysics, currentRotation);
	end
end

function PlayerForward(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local ComponentDirection = GetComponentDirection(entity)

		local currentDirection = GetDirection(ComponentDirection)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local changeVelocity = Vector3:new(1, 0, 0)
		local rotationMatrix = CreateRotationMatrix(currentDirection)

		changeVelocity = MultiplyMatrixVector(rotationMatrix, changeVelocity)
		changeVelocity = changeVelocity:multiply(deltaTime * inputValue * 500)
		currentVelocity = currentVelocity:add(changeVelocity)

		SetVelocity(ComponentPhysics, currentVelocity);
	end
end

function PlayerBackward(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local ComponentDirection = GetComponentDirection(entity)

		local currentDirection = GetDirection(ComponentDirection)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local changeVelocity = Vector3:new(-1, 0, 0)
		local rotationMatrix = CreateRotationMatrix(currentDirection)

		changeVelocity = MultiplyMatrixVector(rotationMatrix, changeVelocity)
		changeVelocity = changeVelocity:multiply(deltaTime * inputValue * 500)
		currentVelocity = currentVelocity:add(changeVelocity)

		SetVelocity(ComponentPhysics, currentVelocity);
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