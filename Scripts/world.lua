world = {
    Seed = 5345345,
    Layers = 2,

    -- This is run when the world is registered and not after,
    -- although it is possible to register materials later
    Register = function(self)

        -- TODO make world have global textures to speed up rendering

        self:registerMaterial("grass", {
            -- TODO combine both of these into 1
            texture = {
                file = "Assets/world.png",
                offset = { x = 0, y = 0 },
                size = { x = 8, y = 8 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 0, y = 0 },
                size = { x = 8, y = 8 }
            }
        });
        self:registerMaterial("dirt", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 8, y = 0 },
                size = { x = 8, y = 8 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 8, y = 0 },
                size = { x = 8, y = 8 }
            }
        });
        self:registerMaterial("stone", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 16, y = 0 },
                size = { x = 8, y = 8 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 16, y = 0 },
                size = { x = 8, y = 8 }
            }
        });
        self:registerMaterial("flower", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 24, y = 0 },
                size = { x = 8, y = 8 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 24, y = 0 },
                size = { x = 8, y = 8 }
            },
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
                        self:setData(X, Y, Z, "grass");
                    elseif Y < YGen and Y > (YGen - YDepth) then
                        self:setData(X, Y, Z, "dirt");
                    elseif Y < YGen then
                        --self:setData(X, Y, Z, "stone");
                        self:setData(X, Y, Z, "grass");
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
