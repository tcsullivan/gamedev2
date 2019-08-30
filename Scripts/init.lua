--Player = entityx.manager:new()
--function Player:init(o)
--    o = o or {}
--    self.position = self:component("Position", o.position or {0, 0})
--end
--
--p = Player:instance({position = {10, 20}})
--p.position.x(5)
--print(p.position.x(), p.position.y())

--p = comp:Position(6.5, 2.3)

bird = {
    Position = {
        x = 1.2,
        y = 3.4
    },
    Name = "bord",
    init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
    end
}

dog = {
    Position = {
        x = 6.5,
        y = 1.3
    },
    Render = {
        texture = "assets/tex.png",
        visible = true
    },
    init = function(self)
        print(self.Position.x .. "," .. self.Position.y)
    end
}

animal = {
    Render = {
        texture = "assets/anim.png",
        visible = false
    }
}

birdSpawn = game.spawn(bird);
birdSpawn:init()
print(birdSpawn.Name.value)

dogSpawn = game.spawn(dog);
dogSpawn:init()
dogSpawn.Position.x = 37.5

animalSpawn = game.spawn(animal);
animalSpawn.Render.texture = "assets/newText.png"
