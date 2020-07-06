function PlaySuperWorld()
	NewGameScene("Levels/PlatformLevel.json")
end

function GoToOptionsMenu()
	NewMenuScene("Options.json")
end

function GoToHitBoxMenu()
	SwapToMenuScene("HitBoxMainMenu.json")
end

function QuitToChoosingMenu()
	CloseScene()
end

function QuitToWindow()
	CloseWindow()
end