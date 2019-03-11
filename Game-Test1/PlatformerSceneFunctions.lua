local Vector3 = require("Scripts/Vector3")

function PlatformerSceneDisplay()
	local entity = GetEntity("Player")
	local ComponentState = GetComponentState(entity)
	local currentScore = GetValue(ComponentState, "CoinsCollected", "integer", 0)

	local scoreDisplay = "Collected " + currentScore + " / 10 coins"
	local colour = Vector3:new(1, 1, 1)
	DisplayText(scoreDisplay, "Centre", 50, 50, 10, colour)
end