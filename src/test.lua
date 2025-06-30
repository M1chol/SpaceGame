print("Using scene:", mainScene:getName())

local menuBase
local sphere

function onStart()
    menuBase = uiRoundedRect.new(mainScene, 12, 5, 300, 500)
    sphere = uiSphere.new(mainScene, 64, 120)
    sphere:move(Vect.new(200, 200))
    menuBase:setColor(150, 212, 175, 255)
    menuBase:setName("LuaTest")
    print(menuBase:getName())
    print("Lua file loaded")
end

local pos = Vect.new(200, 100)
local dir = 1

local hue = 0
local hueSpeed = 60
local saturation = 1.0 
local lightness = 0.5

function onUpdate(delta)
    pos.x = pos.x + 2 * dir
    menuBase:move(pos)
    if(pos.x > 700 or pos.x < 200)
    then
        dir = dir * -1
    end

    hue = hue + (hueSpeed * delta)
    if hue > 360 then
        hue = hue - 360
    end

    local r, g, b = hslToRgb(hue / 360, saturation, lightness)
    sphere:setColor(r, g, b, 255)
end


function hslToRgb(h, s, l)
    local r, g, b

    if s == 0 then
        r, g, b = l, l, l -- achromatic
    else
        local function hue2rgb(p, q, t)
            if t < 0 then t = t + 1 end
            if t > 1 then t = t - 1 end
            if t < 1/6 then return p + (q - p) * 6 * t end
            if t < 1/2 then return q end
            if t < 2/3 then return p + (q - p) * (2/3 - t) * 6 end
            return p
        end

        local q = l < 0.5 and l * (1 + s) or l + s - l * s
        local p = 2 * l - q
        r = hue2rgb(p, q, h + 1/3)
        g = hue2rgb(p, q, h)
        b = hue2rgb(p, q, h - 1/3)
    end
    -- Return RGB values scaled to 0-255
    return math.floor(r * 255), math.floor(g * 255), math.floor(b * 255)
end
