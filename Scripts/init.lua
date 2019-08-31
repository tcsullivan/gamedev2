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
        x = 0.01,
        y = 10.5
    },
    Position = {
        x = 6.5,
        y = 1.3
    },
    Render = {
        texture = "assets/tex.png",
        visible = true
    },
    Init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
    end,
    Idle = function(self)
        self.Position.x = self.Position.x + 0.01;
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
dogSpawn.Position.x = 37.5

animalSpawn = game.spawn(animal);
print("Animal pos: " .. animalSpawn.Position.x)
