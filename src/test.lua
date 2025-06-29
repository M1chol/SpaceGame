print("Using scene:", mainScene:getName())

-- Create a new Object, parented to the scene
-- local myObject = Object.new(mainScene)
-- myObject:setName("LuaObject")

-- Create a TextComponent and attach it to the object.
-- local textComp = TextComponent.new(
--     "Hello, World!",
--     iVect.new(200, 300),
--     "res/HomeVideo.ttf",
--     20,
--     myObject
-- )

local menuBase = uiRoundedRect.new(mainScene, 12, 5, 300, 500)

-- local sphereTest = uiSphere.new(myObject, 12, 30)
print(menuBase:getName())
menuBase:move(Vect.new(150, 100))
menuBase:setColor(150, 212, 175, 255)
