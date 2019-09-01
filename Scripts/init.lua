bird = {
    Position = {
        x = 1.2,
        y = 3.4
    },
    Name = "bord",
    Init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
        print("Bird spawn")
    end
}

dog = {
    Velocity = {
        x = 0.0,
        y = 0.0
    },
    Position = {
        x = 50,
        y = 0
    },
    Render = {
        texture = "assets/tex.png",
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
    Render = {
        texture = "assets/anim.png",
        visible = false
    }
}

birdSpawn = game.spawn(bird);

dogSpawn = game.spawn(dog);

animalSpawn = game.spawn(animal);
print("Animal pos: " .. animalSpawn.Position.x)
