bird = {
    Player = 0,
    EventListeners = {
        MoveLeftPressed = function(self)
            self.Velocity.x = self.Velocity.x - 100
        end,
        MoveLeftReleased = function(self)
            self.Velocity.x = self.Velocity.x + 100
        end,
        MoveRightPressed = function(self)
            self.Velocity.x = self.Velocity.x + 100
        end,
        MoveRightReleased = function(self)
            self.Velocity.x = self.Velocity.x - 100
        end
    },
    Position = {
        x = 150,
        y = 75
    },
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Name = "bord",
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

cat = {
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Position = {
        x = 180,
        y = -75
    },
    Render = {
        texture = "Assets/cat.png",
        normal = "Assets/cat_normal.png",
        visible = true
    },
    counter = 0;
    Idle = function(self)
        self.Velocity.x = -100 * math.sin(math.rad(self.counter));
        self.Velocity.y =  100 * math.cos(math.rad(self.counter));
        self.counter = self.counter + 5;
    end,
    PhysicsIdle = function(self)
        if self.Velocity.x < 0 then
            self.Render.flipx = true
        elseif self.Velocity.x > 0 then
            self.Render.flipx = false
        end
    end
}

animal = {
    Name = "animal",
    Velocity = { -- This will automatically assign position to (0,0)
        x = 0.0,
        y = 0.0
    },
    Render = {
        texture = "Assets/rabbit.png",
        visible = true
    },
    Light = {
        r = 0.0,
        b = 1.0,
        g = 0.2,
        strength = 1.0
    },
    Idle = function(self)
        self.Velocity.x = -200 * math.sin(math.rad(self.counter));
        self.Velocity.y =  500 * math.cos(math.rad(self.counter*5));
        self.counter = self.counter + 5;
    end,
    counter = 0,
    TestMe = function(self)
        print("Light: rgb("..self.Light.r..","..self.Light.g..","..self.Light.b..")")
    end
}

wall = {
    Position = {
        y = -100
    },
    Render = {
        texture = "Assets/rock.png",
        normal = "Assets/rock_normal.png",
        visible = true
    }
}

birdSpawn = game.spawn(bird);

dogSpawn = game.spawn(cat);

animalSpawn = game.spawn(animal);

wallSpawn = game.spawn(wall);

game.spawn({
    Velocity = {
        x = 0,
        y = 0
    },
    Light = {
        r = 0.8,
        g = 0.1,
        b = 0.2,
        strength = 0.75
    },
    counter = 0;
    Idle = function(self)
        self.Velocity.x = -100 * math.sin(math.rad(self.counter));
        self.Velocity.y =  100 * math.cos(math.rad(self.counter));
        self.counter = self.counter + 5;
    end
});

-------------------
--  SERIALIZING  --
-------------------

function serial (before_ser)
    binary = string.dump(before_ser)
    formatted_binary = ""
    for i = 1, string.len(binary) do
        dec, _ = ("\\%3d"):format(binary:sub(i, i):byte()):gsub(' ', '0')
        formatted_binary = formatted_binary .. dec
    end
    return formatted_binary
end

function hello(herro)
    print("Hello world ".. herro)
end

--print(serial(hello))

local ser = string.dump(hello)
f2 = (loadstring or load)(ser)
f2("shite")

local testPut = string.dump(animalSpawn.TestMe)
testRun = (loadstring or load)(testPut)
testRun(animalSpawn)

--blah = load(serial(hello))
--blah()

--draw = loadstring("\027\076\074\001\000\009\064\109\097\105\110\046\108\117\097\084\000\000\004\000\004\000\008\009\002\002\052\000\000\000\055\000\001\000\055\000\002\000\037\001\003\000\039\002\010\000\039\003\010\000\062\000\004\001\071\000\001\000\017\104\101\108\108\111\044\032\119\111\114\108\100\010\112\114\105\110\116\013\103\114\097\112\104\105\099\115\009\108\111\118\101\001\001\001\001\001\001\001\002\000\000")
--draw()
