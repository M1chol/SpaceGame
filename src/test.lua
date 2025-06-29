print("Using scene:", mainScene:getName())

-- Create a new Object, parented to the scene
local myObject = Object.new(mainScene)
myObject:setName("LuaObject")

-- Create a TextComponent and attach it to the object.
-- local textComp = TextComponent.new(
--     "Hello, World!",
--     iVect.new(200, 300),
--     "res/HomeVideo.ttf",
--     20,
--     myObject
-- )

local menuBase = uiRoundedRect.new(myObject, 12, 5, 300, 500)

-- local sphereTest = uiSphere.new(myObject, 12, 30)
myObject:move(Vect.new(100, 200))
menuBase:setColor(212, 212, 212, 255)
