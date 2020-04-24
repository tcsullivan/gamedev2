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
                size = { x = 64, y = 64 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 0, y = 0 },
                size = { x = 64, y = 64 }
            }
        });
        self:registerMaterial("dirt", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 64, y = 0 },
                size = { x = 64, y = 64 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 64, y = 0 },
                size = { x = 64, y = 64 }
            }
        });
        self:registerMaterial("stone", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 128, y = 0 },
                size = { x = 64, y = 64 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 128, y = 0 },
                size = { x = 64, y = 64 }
            }
        });
        self:registerMaterial("flower", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 192, y = 0 },
                size = { x = 64, y = 64 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 192, y = 0 },
                size = { x = 64, y = 64 }
            },
            passable = true
        });
        self:registerMaterial("trunk", {
            texture = {
                file = "Assets/world.png",
                offset = { x = 256, y = 0 },
                size = { x = 64, y = 64 }
            },
            normal = {
                file = "Assets/world_normal.png",
                offset = { x = 256, y = 0 },
                size = { x = 64, y = 64 }
            }
        });
    end,

    Generate = function(self)
        math.randomseed(self.Seed)
        xsize, ysize, zsize = self:setSize(250, 128, 3)
        for Z = 0,zsize-1 do
            for X = 0,xsize-1 do
                if Z == 0 then
                    YGen = math.floor(6*math.sin(X/20)) + 64
                elseif Z == 1 then
                    YGen = math.floor(9*math.sin(X/20)) + 64
                else
                    YGen = math.floor(15*math.sin(X/20)) + 64
                end
                YDepth = math.random(3,5)
                for Y = 0,ysize-1 do
                    if Y == YGen then
                        self:setData(X, Y, Z, "grass");
                    elseif Y < YGen and Y > (YGen - YDepth) then
                        self:setData(X, Y, Z, "dirt");
                    elseif Y < YGen then
                        self:setData(X, Y, Z, "stone");
                    elseif Y == YGen + 1 then
                        if math.random(0, 100) == 53 then
                            self:setData(X, Y, Z, "flower");
                        elseif math.random(0, 100) == 45 then
                            self:setData(X, Y, Z, "trunk");
                        end
                    end
                    --print(X..","..Y..","..Z);
                end
            end
        end
        --self:setData(1000, 1345, 5, "grass"); -- Test error checking
        print("Done with world gen");
    end
}

--world:Generate()
game.worldRegister(world)
