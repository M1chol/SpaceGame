print("Using scene:", mainScene:getName())

-- Create a new Object, parented to the scene
local myObject = Object.new(mainScene)
myObject:setName("LuaObject")

-- Create a TextComponent and attach it to the object.
local textComp = TextComponent.new(
    "Hello, World!",
    iVect.new(200, 300),
    "res/HomeVideo.ttf",
    20,
    myObject
)

-- Create a Vect to use for movement.
local delta = Vect.new(0, 40)

-- Loop from 20 down to 1 (with a negative step).
for i = 20, 1, -1 do
    myObject:move(delta, false)
end
