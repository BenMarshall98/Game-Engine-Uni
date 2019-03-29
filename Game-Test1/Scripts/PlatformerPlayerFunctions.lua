local Vector3 = require("Scripts/Vector3")
local wasTouchingGround = false

function HitGround(entity)
	if wasTouchingGround == false then
		local ComponentPhysics = GetComponentPhysics(entity)
		if GetTouchingGround(ComponentPhysics) then
			local ComponentPosition = GetComponentPosition(entity)
			local currentPosition = GetPosition(ComponentPosition)
			PlayAudioAtLocation("Thump", currentPosition)
			wasTouchingGround = true
		end
	end
end

function CollectCoin(entity)
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "CoinsCollected", "integer", 0);
	currentScore = currentScore + 1
	SetValue(ComponentState, "CoinsCollected", currentScore)
end

function PlayerLeft(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local x = currentVelocity:getX()
		x = x - (inputValue * 16)
		currentVelocity:setX(x)
		SetVelocity(ComponentPhysics, currentVelocity)
	end
end

function PlayerRight(entity, inputValue, deltaTime)
	if inputValue > 0.2 then
		local ComponentPhysics = GetComponentPhysics(entity)
		local currentVelocity = GetVelocity(ComponentPhysics)
		local x = currentVelocity:getX()
		x = x + (inputValue * 16)
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
			y = y + (inputValue * 1.2)
			currentImpulse:setY(y)
			SetImpulse(ComponentPhysics, currentImpulse)
			wasTouchingGround = false
		end
	end
end

function PlayerAnimation(entity, deltaTime)
	local ComponentState = GetComponentState(entity)
	local timeTillWin = GetValue(ComponentState, "TimeTillWin", "float", -1)
	if timeTillWin > -1 then
		timeTillWin = timeTillWin - deltaTime
		SetValue(ComponentState, "TimeTillWin", timeTillWin)
	end
end