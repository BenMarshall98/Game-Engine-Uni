local Vector3 = require("Scripts/Vector3")
local totalCoins = 7

function PlatformerSceneDisplay()
	local entity = GetEntity("Player")
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "CoinsCollected", "integer", 0)

	local scoreDisplay = "Collected " .. currentScore .. " / " .. totalCoins .. " coins"
	local colour = Vector3:new(1, 1, 1)
	DisplayText(scoreDisplay, "Centre", 50, 50, 10, colour)

	local timeTillWin = GetValue(ComponentState, "TimeTillWin", "float", -1)
	if timeTillWin == -1 and currentScore == totalCoins then
		SetValue(ComponentState, "TimeTillWin", 1.0)
	end

	if (timeTillWin ~= -1) and timeTillWin <= 0 then
		SwapToMenuScene("WinScene.json")
	end
end