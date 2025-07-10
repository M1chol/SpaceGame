function OnStart(...) end

function OnUpdate(...) end

--- @class Component
local Component = {}
function Component:getName(...) end

function Component:getParent(...) end

--- @class Object
local Object = {}
function Object:addTag(...) end

function Object:destroy(...) end

function Object:getName(...) end

function Object:getPos(...) end

function Object:getRotation(...) end

function Object:move(...) end

function Object:removeComponent(...) end

function Object:removePosParent(...) end

function Object:rotate(...) end

function Object:setName(...) end

function Object:setPosParent(...) end

--- @class RigidBodyComponent
local RigidBodyComponent = {}
function RigidBodyComponent:applyForce(...) end

function RigidBodyComponent:getHitBox(...) end

function RigidBodyComponent:isColliding(...) end

function RigidBodyComponent:render(...) end

function RigidBodyComponent:setCollision(...) end

function RigidBodyComponent:setEnergyLoss(...) end

function RigidBodyComponent:setMass(...) end

function RigidBodyComponent:solveCollision(...) end

function RigidBodyComponent:update(...) end

function RigidBodyComponent:whenLinked(...) end

--- @class Scene
local Scene = {}
function Scene:getDrawPriority(...) end

function Scene:getName(...) end

function Scene:getNrOfObjects(...) end

function Scene:getObjectByName(...) end

function Scene:getObjectsByTag(...) end

function Scene:setDrawPriority(...) end

function Scene:setName(...) end

--- @class SpriteComponent
local SpriteComponent = {}
function SpriteComponent:getCenter(...) end

function SpriteComponent:getDim(...) end

function SpriteComponent:load(...) end

function SpriteComponent:render(...) end

function SpriteComponent:rotate(...) end

function SpriteComponent:setCentered(...) end

function SpriteComponent:setOffset(...) end

function SpriteComponent:setScale(...) end

function SpriteComponent:setSheetIndex(...) end

function SpriteComponent:whenLinked(...) end

--- @class TextComponent
local TextComponent = {}
function TextComponent:setColor(...) end

function TextComponent:setFont(...) end

function TextComponent:setScale(...) end

function TextComponent:setSize(...) end

--- @class Vect
local Vect = {}
function Vect:magnitude(...) end

function Vect:normalized(...) end

function Vect:rotate(...) end

function Vect:toIVect(...) end

--- @class iVect
local iVect = {}

--- @class uiRoundedRect
local uiRoundedRect = {}
function uiRoundedRect:addTag(...) end

function uiRoundedRect:destroy(...) end

function uiRoundedRect:getName(...) end

function uiRoundedRect:getPos(...) end

function uiRoundedRect:getRotation(...) end

function uiRoundedRect:move(...) end

function uiRoundedRect:removeComponent(...) end

function uiRoundedRect:rotate(...) end

function uiRoundedRect:setColor(...) end

function uiRoundedRect:setName(...) end

--- @class uiSphere
local uiSphere = {}
function uiSphere:addTag(...) end

function uiSphere:destroy(...) end

function uiSphere:getName(...) end

function uiSphere:getPos(...) end

function uiSphere:getRotation(...) end

function uiSphere:move(...) end

function uiSphere:removeComponent(...) end

function uiSphere:rotate(...) end

function uiSphere:setColor(...) end

function uiSphere:setName(...) end

local uiButton = {}
function uiButton:setText(newText --[[string]]) end

function uiButton:setColor(newColor --[[Color]], borderShift --[[float]]) end

function uiButton:setOnClick(funct --[[functions]]) end
