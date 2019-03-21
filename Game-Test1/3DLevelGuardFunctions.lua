local Vector3 = require("Scripts/Vector3")

function GuardStart(guardEntity, playerEntity, AIStateMachine, deltaTime)
	local GuardComponentPosition = GetComponentPosition(guardEntity)
	local PlayerComponentPosition = GetComponentPosition(playerEntity)
	local guardPosition = GetPosition(GuardComponentPosition)
	local playerPosition = GetPosition(PlayerComponentPosition)

	if CanSeePlayer(guardPosition, playerPosition) then
		--Move to function to deal with the case where the guard can see the player
		SeePlayer(guardEntity, playerEntity, guardPosition, playerPosition, AIStateMachine, deltaTime)
	else
		--Move to function to deal with the case where the guard cannot see the player
		CannotSeePlayer(guardEntity, guardPosition, AIStateMachine, deltaTime)
	end
end

function SeePlayer(guardEntity, playerEntity, guardPosition, playerPosition, AIStateMachine, deltaTime)
	local distanceVector = guardPosition:subtract(playerPosition)
	local distance = distanceVector:magnitude()

	if distance < 10 then
		--Shoot at the player
		ShootingPlayer(guardEntity, guardPosition, playerEntity, playerPosition, AIStateMachine, deltaTime)
	else
		--Move towards the player using the A* Algorithm
		MoveAIToPlayer(guardEntity, guardPosition, AIStateMachine, deltaTime)
	end
end

function ShootingPlayer(guardEntity, guardPosition, playerEntity, playerPosition, AIStateMachine, deltaTime)
	local timeLastShoot = GetAIValue(AIStateMachine, "timeSinceShoot", "float", "0")
	timeLastShoot = timeLastShoot - deltaTime;
	if timeLastShoot <= 0 then
		ShootPlayer(guardPosition, playerPosition)
		timeLastShoot = 1
	end
	--Else do nothing
	SetAIValue(AIStateMachine, "timeSinceShoot", timeLastShoot)
end

function MoveAIToPlayer(guardEntity, guardPosition, AIStateMachine, deltaTime)
	local GuardComponentPhysics = GetComponentPhysics(guardEntity)
	local GuardComponentDirection = GetComponentDirection(guardEntity)
	local guardDirection = GetDirection(GuardComponentDirection)

	MoveOffPath(AIStateMachine)
	FindAIPath(AIStateMachine, guardPosition, guardDirection, GuardComponentPhysics, deltaTime)
end

function CannotSeePlayer(guardEntity, guardPosition, AIStateMachine, deltaTime)
	local GuardComponentPhysics = GetComponentPhysics(guardEntity)
	local GuardComponentDirection = GetComponentDirection(guardEntity)
	local guardDirection = GetDirection(GuardComponentDirection)

	if OnPath(AIStateMachine, guardPosition) then
		--Move along guard path
		FindPath(AIStateMachine, guardPosition, guardDirection, GuardComponentPhysics, deltaTime)
	else
		--Move to the guard path
		local nearestPath = GetNearestPath(AIStateMachine, guardPosition)
		FindAIPath(AIStateMachine, guardPosition, guardDirection, GuardComponentPhysics, nearestPath, deltaTime)
	end
end

function ShootPlayer(guardPosition, playerPosition)
	local entity = CreateEntity();
	local dir = playerPosition:subtract(guardPosition)
	dir = dir:normalise()
	local position = guardPosition:add(dir);
	local direction = Vector3:new(0, 1, 0);
	local size = Vector3:new(0.05, 0.05, 0.05);
	local angularLimits = Vector3:new(1, 1, 1);
	local cuboid = CreateCollisionCuboid(size);

	local collisionFunctionMap = CreateCollisionFunctionMap()
	AddToCollisionFunctionMap(collisionFunctionMap, "PLAYER", "DestroyBullet")
	AddToCollisionFunctionMap(collisionFunctionMap, "WALL", "DestroyBullet")

	AddComponentPosition(entity, position);
	AddComponentDirection(entity, direction, 0);
	AddComponentPhysics(entity, cuboid, 0.1, "BULLET", angularLimits, false, collisionFunctionMap);
	AddComponentModel(entity, "Bullet");
	AddComponentNormalTexture(entity, "BoxNormal");
	AddComponentTexture(entity, "Box");
	AddComponentShader(entity, "NormalShader");
	AddComponentShadowShader(entity, "DirectionalShadow", "PointShadow");
	AddComponentAnimation(entity, "BulletMovement");
	AddComponentState(entity);

	local ComponentState = GetComponentState(entity);
	SetValue(ComponentState, "xDirection", dir:getX())
	SetValue(ComponentState, "yDirection", dir:getY())
	SetValue(ComponentState, "zDirection", dir:getZ())
	SetValue(ComponentState, "BulletTime", 5)

	FinishEntity(entity);
end

function BulletMovement(entity, deltaTime)
	local ComponentPhysics = GetComponentPhysics(entity)
	local ComponentState = GetComponentState(entity)

	local xDir = GetValue(ComponentState, "xDirection", "float", 0)
	local yDir = GetValue(ComponentState, "yDirection", "float", 0)
	local zDir = GetValue(ComponentState, "zDirection", "float", 0)
	local time = GetValue(ComponentState, "BulletTime", "float", 0)
	time = time - deltaTime

	if time < 0 then
		DeleteEntity(entity)
	end

	local velocity = Vector3:new(xDir, yDir, zDir)
	velocity = velocity:multiply(3000 * deltaTime)

	SetVelocity(ComponentPhysics, velocity)
end

function ThreeDLevelSceneDisplay()
	local entity = GetEntity("Player")
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "Health", "integer", 100)

	local scoreDisplay = "Health: " .. currentScore
	local colour = Vector3:new(1, 1, 1)
	DisplayText(scoreDisplay, "Centre", 50, 50, 10, colour)
end

function PlayerDown(entity)
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "Health", "integer", 100)
	currentScore = currentScore - 10

	SetValue(ComponentState, "Health", currentScore)
end

function DestroyBullet(entity)
	DeleteEntity(entity)
end

function GuardDown(entity)
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "Health", "integer", 100)
	currentScore = currentScore - 10

	if currentScore <= 0 then
		DeleteEntity(entity)
	end

	SetValue(ComponentState, "Health", currentScore)
end