print("Using scene:", mainScene:getName())

local myObject = Object("main-scene")
-- Create a TextComponent and link it to the object.
-- The constructor signature is:
-- TextComponent(setMessage, newOffset, fontPath, fontSize, parent)
local textComp = TextComponent("Hello, World!", { x = 0, y = 0 }, "assets/arial.ttf", 20, myObject)