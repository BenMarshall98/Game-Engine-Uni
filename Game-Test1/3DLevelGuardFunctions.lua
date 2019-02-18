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
	local timeLastShoot = GetValue(AIStateMachine, "timeSinceShoot", "float", 0)
	timeLastShoot = timeLastShoot - deltaTime;
	if timeLastShoot <= 0 then
		ShootPlayer(guardEntity, playerEntity)
		timeLastShoot = 1
	end
	--Else do nothing
	SetValue(AIStateMachine, "timeSinceShoot", timeLastShoot)
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

	if OnPath(AIStateMachine, guardPosition)
		--Move along guard path
		FindPath(AIStateMachine, guardPosition, guardDirection, GuardComponentPhysics, deltaTime)
	else
		--Move to the guard path
		local nearestPath = GetNearestPath(AIStateMachine, guardPosition)
		FindAIPath(AIStateMachine, guardPosition, guardDirection, GuardComponentPhysics, nearestPath, deltaTime)
	end
end