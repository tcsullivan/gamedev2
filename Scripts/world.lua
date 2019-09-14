world = {
    Seed = 5345345,
    Layers = 2,

    -- This is run when the world is registered and not after,
    -- although it is possible to register materials later
    Register = function(self)
        self:registerMaterial("grass", {
            texture = "Assets/grass.png",
            normal = "Assets/grass_normal"
        });
        self:registerMaterial("dirt", {
            texture = "Assets/dirt.png",
            normal = "Assets/dirt_normal.png"
        });
        self:registerMaterial("stone", {
            texture = "Assets/stone.png",
            normal = "Assets/dirt_normal.png"
        });
        self:registerMaterial("flower", {
            texture = "Assets/flower.png",
            normal = "Assets/flower_normal.png",
            passable = true
        });
    end,

    Generate = function(self)
        math.randomseed(self.Seed)
        xsize, ysize, zsize = self:setSize(250, 128, 3)
        --self.data = {}
        for Z = 0,zsize-1 do
            --self.data[Z] = {}
            for X = 0,xsize-1 do
                --self.data[Z][X] = {}
                YGen = math.floor(6*math.sin(X/20) + Z) + 64
                YDepth = math.random(2,5)
                for Y = 0,ysize-1 do
                    if Y == YGen then
                        --self.data[Z][X][Y] = 0
                        self:setData(X, Y, Z, "grass");
                    elseif Y < YGen and Y > (YGen - YDepth) then
                        --self.data[Z][X][Y] = 1
                        self:setData(X, Y, Z, "dirt");
                    elseif Y < YGen then
                        --self.data[Z][X][Y] = 2
                        self:setData(X, Y, Z, "stone");
                    end
                    --print(X..","..Y..","..Z);
                end
            end
        end
        self:setData(1000, 1345, 5, "grass"); -- Test error checking
        print("Done with world gen");
    end
}

--world:Generate()
game.worldRegister(world)
