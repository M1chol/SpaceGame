local function create_menu(scene, text_list, pos, options)
    -- default values
    local resolution = options.resolution or 16
    local radius = options.radius or 20
    local width = options.width or 200
    local height = options.height or 70
    local borderSize = options.borderSize or 4
    local paddingX = options.padding and options.padding.x or 10
    local paddingY = options.padding and options.padding.y or 10
    print("[ LUA ] Creating menu ")

    local menu = {}

    for i, text in ipairs(text_list) do
        local button = uiButton.new(scene, resolution, radius, width, height, borderSize, text)
        local name = "menuButton-" .. i
        button:setName(name)
        local newPosition = Vect.new(pos[1] + paddingX, pos[2] + i * (height + paddingY))
        button:move(newPosition)
        menu[i] = button
    end

    return { menu }
end

return {
    create_menu = create_menu
}
