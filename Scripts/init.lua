game.loadFont("default", "Assets/FreePixel.ttf", 16)

player = {
    Player = 0,
    EventListeners = {
        MoveLeftPressed = function(self)
            self.Velocity.x = self.Velocity.x - 3
            self.Render.flipx = true;
        end,
        MoveLeftReleased = function(self)
            game.puts("default", self.Position.x, self.Position.y, "Hey.")
            self.Velocity.x = self.Velocity.x + 3
        end,
        MoveRightPressed = function(self)
            self.Velocity.x = self.Velocity.x + 3
            self.Render.flipx = false;
        end,
        MoveRightReleased = function(self)
            self.Velocity.x = self.Velocity.x - 3
        end,
        JumpKeyPressed = function(self)
            if self.Physics.standing == true then
                self.Velocity.y = self.Velocity.y + 9
            end
        end,
        JumpKeyReleased = function(self)
        end
    },
    Position = {
        x = 15,
        y = 75
    },
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Physics = 0,
    Name = "bord",
    hellotrue = true,
    hellofalse = false,
    Render = {
        texture = "Assets/player.png",
        visible = true
    },
    Light = {
        r = 1.0,
        g = 1.0,
        b = 1.0,
        strength = 1.0
    },
    Idle = function(self)
        --if (self.visibleTick >= 20) then
        --    self.Render.visible = not self.Render.visible
        --    self.visibleTick = 0
        --end
        --self.visibleTick = self.visibleTick + 1
    end,
    PhysicsIdle = function(self)
        if self.Velocity.x < 0 then
            self.Render.flipx = true
        elseif self.Velocity.x > 0 then
            self.Render.flipx = false
        end
    end,
    visibleTick = 0
}

-- Create the world
dofile("Scripts/world.lua")

playerSpawn = game.spawn(player);

-------------------
--  SERIALIZING  --
-------------------

--function serial (before_ser)
--    binary = string.dump(before_ser)
--    formatted_binary = ""
--    for i = 1, string.len(binary) do
--        dec, _ = ("\\%3d"):format(binary:sub(i, i):byte()):gsub(' ', '0')
--        formatted_binary = formatted_binary .. dec
--    end
--    return formatted_binary
--end
--
--function hello(herro)
--    print("Hello world ".. herro)
--end
--
----print(serial(hello))
--
--local ser = string.dump(hello)
--f2 = (loadstring or load)(ser)
--f2("shite")
--
--local testPut = string.dump(animalSpawn.TestMe)
--testRun = (loadstring or load)(testPut)
--testRun(animalSpawn)
