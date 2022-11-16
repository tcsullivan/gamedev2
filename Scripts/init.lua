game.loadFont("default", "Assets/FreePixel.ttf", 16)

player = {
    Player = 0,
    EventListeners = {
        MoveLeftPressed = function(self)
            --self.Velocity.x = self.Velocity.x - 3.0
            self.Velocity.y = self.Velocity.y - 1.0
            self.Render.flipx = true;
        end,
        MoveLeftReleased = function(self)
            -- TODO can't put text at world coordinates right now
            --game.puts("default", self.Position.x, self.Position.y+100, "Hey. Hag?")
            --self.Velocity.x = self.Velocity.x + 3.0
            self.Velocity.y = self.Velocity.y + 1.0
        end,
        MoveRightPressed = function(self)
            --self.Velocity.x = self.Velocity.x + 3.0
            self.Velocity.y = self.Velocity.y + 1.0
            self.Render.flipx = false;
        end,
        MoveRightReleased = function(self)
            --self.Velocity.x = self.Velocity.x - 3.0
            self.Velocity.y = self.Velocity.y - 1.0
        end,
        JumpKeyPressed = function(self)
            if self.Physics.standing == true then
                game.play(self.Position, self.Audio)
                self.Velocity.y = self.Velocity.y + 9
            end
        end,
        JumpKeyReleased = function(self)
            game.dialog(30, -150, 400, 100)
        end
    },
    Position = {
        15.0, 20.0
    },
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Physics = {
        hitbox = {
            ll = {x = -0.5, y = -0.8},
            lr = {x = 0.5, y = -0.8},
            ul = {x = -0.5, y = 0.8},
            ur = {x = 0.5, y = 0.8},
        }
    },
    Name = "bord",
    Audio = {
        file = "Assets/jump.wav"
    },
    hellotrue = true,
    hellofalse = false,
    Render = {
        texture = "Assets/player.png",
        visible = true,
        offset = {
            ll = {x = -0.5, y = -0.8},
            lr = {x = 0.5, y = -0.8},
            ul = {x = -0.5, y = 0.8},
            ur = {x = 0.5, y = 0.8},
        }
    },
    Light = {
        r = 1.0,
        g = 1.0,
        b = 1.0,
        strength = 1.0
    },
    Idle = function(self)
    end,
    visibleTick = 0
}

ball = {
    Position = {
        x = 20,
        y = 100
    },
    Velocity = {
        x = 0.0,
        y = 0.0,
    },
    Physics = 0,
    Render = {
        texture = "Assets/ball.png",
        normal = "Assets/ball_normal.png",
        visible = true,
    },
    Idle = function(self)
        if self.Physics.standing == true then
            game.play(self.Position, self.Audio)
            self.Velocity.y = self.Velocity.y + 15
            self.Velocity.x = math.random(-1, 1);
        end
    end,
    Audio = {
        file = "Assets/boing.wav"
    },
}

npc = {
    Position = {
        x = 30,
        y = 75
    },
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Physics = 0,
    Name = "Paul",
    Render = {
        texture = "Assets/cat.png",
        visible = true
    },
    Light = {
        r = 1.0,
        g = 1.0,
        b = 1.0,
        strength = 0.5
    },
    EventListeners = {
        MousePressed = function(self, x, y, button)
            self.Velocity.y = 3.0;
        end
    },
    Idle = function(self)
		if (self.visibleTick == 0) then
			self.visibleTick = math.random(40, 60)
			self.Velocity.x = math.random(-1, 1) * 1.0
        else
            self.visibleTick = self.visibleTick - 1
        end
    end,
    visibleTick = 0
}

-- Create the world
dofile("Scripts/world.lua")

playerSpawn = game.spawn(player);
game.spawn(ball);
game.spawn(npc);

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
