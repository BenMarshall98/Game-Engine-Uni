local Vector3 = require("Scripts/Vector3")

function DeleteCoin(entity)
	DeleteEntity(entity)
end

function PlayerLeft(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentRotation = GetRotation(ComponentPhysics)
		local y = currentRotation:getX()
		y = y + (inputValue * 4)
		currentRotation:setY(y)
		SetRotation(ComponentPhysics, currentRotation);
	end
end

function PlayerRight(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentRotation = GetRotation(ComponentPhysics)
		local y = currentRotation:getX()
		y = y - (inputValue * 4)
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
		changeVelocity = changeVelocity:multiply(inputValue * 8)
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
		changeVelocity = changeVelocity:multiply(inputValue * 8)
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
			y = y + (inputValue * 1.5)
			currentImpulse:setY(y)
			SetImpulse(ComponentPhysics, currentImpulse)
		end
	end
end

function PlayerShoot(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentState = GetComponentState(entity)
		local timeLastShoot = GetValue(ComponentState, "timeSinceShoot", "float", "0")
		
		if timeLastShoot <= 0 then
			local ComponentPosition = GetComponentPosition(entity)
			local ComponentDirection = GetComponentDirection(entity)
			local currentPosition = GetPosition(ComponentPosition)
			local currentDirection = GetDirection(ComponentDirection)
			local changePosition = Vector3:new(1, 0, 0)
			local rotationMatrix = CreateRotationMatrix(currentDirection)

			changePosition  = MultiplyMatrixVector(rotationMatrix, changePosition)

			local entity = CreateEntity();
			local position = currentPosition:add(changePosition);
			local direction = Vector3:new(0, 1, 0);
			local size = Vector3:new(0.05, 0.05, 0.05);
			local angularLimits = Vector3:new(1, 1, 1);
			local cuboid = CreateCollisionCuboid(size);

			local collisionFunctionMap = CreateCollisionFunctionMap()
			AddToCollisionFunctionMap(collisionFunctionMap, "PLAYER", "DestroyBullet")
			AddToCollisionFunctionMap(collisionFunctionMap, "WALL", "DestroyBullet")

			AddComponentPosition(entity, position);
			AddComponentDirection(entity, direction, 0);
			AddComponentPhysics(entity, cuboid, 0.1, "PBULLET", angularLimits, false, collisionFunctionMap);
			AddComponentModel(entity, "Bullet");
			AddComponentNormalTexture(entity, "BoxNormal");
			AddComponentTexture(entity, "Box");
			AddComponentShader(entity, "NormalShader");
			AddComponentShadowShader(entity, "DirectionalShadow", "PointShadow");
			AddComponentAnimation(entity, "BulletMovement");
			AddComponentState(entity);

			local ComponentState = GetComponentState(entity);
			SetValue(ComponentState, "xDirection", changePosition:getX())
			SetValue(ComponentState, "yDirection", changePosition:getY())
			SetValue(ComponentState, "zDirection", changePosition:getZ())
			SetValue(ComponentState, "BulletTime", 5)

			FinishEntity(entity);
		end
	end
end

function PlayerDown(entity)
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "Health", "integer", 100)
	currentScore = currentScore - 10

	SetValue(ComponentState, "Health", currentScore)
end