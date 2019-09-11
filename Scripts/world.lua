world = {
    Registry = {
        dirt = {
            id = "world0:dirt",
            texture = "Assets/dirt.png",
            normal = "Assets/dirt_normal.png"
        },
        stone = {
            id = "world0:stone",
            texture = "Assets/stone.png",
            normal = "Assets/dirt_normal.png"
        }
    },
    Seed = 5345345,
    Layers = 3,
    Generate = function(self)
        self.data = {}
        for Z = 0,2 do
            self.data[Z] = {}
            for X = 0,250 do
                self.data[Z][X] = {}
                YGen = math.floor(6*math.sin(X/20) + Z) + 64
                for Y = 0,128 do
                    if Y == YGen then
                        self.data[Z][X][Y] = 1
                    elseif Y < YGen then
                        self.data[Z][X][Y] = 2
                    end
                end
            end
        end
    end
}

--world:Generate()
game.worldRegister(world)
