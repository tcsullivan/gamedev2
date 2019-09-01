bird = {
    Player = 0,
    Position = {
        x = 150,
        y = 75
    },
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Name = "bord",
    Init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
        print("Bird spawn")
    end,
    Render = {
        texture = "Assets/player.png",
        visible = true
    },
    Idle = function(self)
        --if (self.visibleTick >= 20) then
        --    self.Render.visible = not self.Render.visible
        --    self.visibleTick = 0
        --end
        --self.visibleTick = self.visibleTick + 1
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
        visible = true
    },
    Init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
    end,
    counter = 0;
    Idle = function(self)
        self.Velocity.x = -100 * math.sin(math.rad(self.counter));
        self.Velocity.y =  100 * math.cos(math.rad(self.counter));
        self.counter = self.counter + 5;
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
    Idle = function(self)
        self.Velocity.x = -200 * math.sin(math.rad(self.counter));
        self.Velocity.y =  500 * math.cos(math.rad(self.counter*5));
        self.counter = self.counter + 5;
    end,
    counter = 0;
}

birdSpawn = game.spawn(bird);

dogSpawn = game.spawn(cat);

animalSpawn = game.spawn(animal);
