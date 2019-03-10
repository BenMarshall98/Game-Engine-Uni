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
		ShootingPlayer(guardEntity, playerEntity, AIStateMachine, deltaTime)
	else
		--Move towards the player using the A* Algorithm
		MoveAIToPlayer(guardEntity, guardPosition, AIStateMachine, deltaTime)
	end
end

function ShootingPlayer(guardEntity, playerEntity, AIStateMachine, deltaTime)
	local timeLastShoot = GetAIValue(AIStateMachine, "timeSinceShoot", "float", "0")
	timeLastShoot = timeLastShoot - deltaTime;
	if timeLastShoot <= 0 then
		ShootPlayer()
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

function ShootPlayer()
	local entity = CreateEntity();
	local position = Vector3:new(5, 1, 5);
	local direction = Vector3:new(0, 1, 0);
	local size = Vector3:new(0.05, 0.05, 0.05);
	local angularLimits = Vector3:new(1, 1, 1);
	local cuboid = CreateCollisionCuboid(size);

	AddComponentPosition(entity, position);
	AddComponentDirection(entity, direction, 0);
	AddComponentPhysics(entity, cuboid, 0.1, "None", angularLimits);
	AddComponentModel(entity, "Bullet");
	AddComponentNormalTexture(entity, "BoxNormal");
	AddComponentTexture(entity, "Box");
	AddComponentShader(entity, "NormalShader");
	AddComponentShadowShader(entity, "DirectionalShadow", "PointShadow");

	FinishEntity(entity);
end