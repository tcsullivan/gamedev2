world = {
    Registry = {
        grass = {
            id = 0,
            texture = "Assets/grass.png",
            normal = "Assets/grass_normal"
        },
        dirt = {
            id = 1,
            texture = "Assets/dirt.png",
            normal = "Assets/dirt_normal.png"
        },
        stone = {
            id = 2,
            texture = "Assets/stone.png",
            normal = "Assets/dirt_normal.png"
        }
    },
    Seed = 5345345,
    Layers = 2,
    Generate = function(self)
        math.randomseed(self.Seed)
        --self.data = {}
        for Z = 0,self.Layers do
            --self.data[Z] = {}
            for X = 0,250 do
                --self.data[Z][X] = {}
                YGen = math.floor(6*math.sin(X/20) + Z) + 64
                YDepth = math.random(2,5)
                for Y = 0,128 do
                    if Y == YGen then
                        --self.data[Z][X][Y] = 0
                        self:setData(X, Y, Z, 0);
                    elseif Y < YGen and Y > (YGen - YDepth) then
                        --self.data[Z][X][Y] = 1
                        self:setData(X, Y, Z, 1);
                    elseif Y < YGen then
                        --self.data[Z][X][Y] = 2
                        self:setData(X, Y, Z, 2);
                    end
                    --print(X..","..Y..","..Z);
                end
            end
        end
        print("Done with world gen");
    end
}

--world:Generate()
game.worldRegister(world)
