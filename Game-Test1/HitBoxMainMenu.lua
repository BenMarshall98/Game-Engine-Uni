function PlayHitBox2()
	NewGameScene("Levels\3DLevel.json")
end

function GoToOptionsMenu()
	NewMenuScene("Options.json")
end

function GoToSuperWorldMenu()
	SwapToMenuScene("SuperWorldMenu.json")
end

function QuitToChoosingMenu()
	CloseScene()
end

function QuitToWindow()
	CloseWindow()
end