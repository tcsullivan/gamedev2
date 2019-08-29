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
local q = Position.new(6, 3)
print(q.x .. "," .. q.y)

print("HEY")

local g = game.testfunc({});
print(g.Position.x .. "," .. g.Position.y);
g.Position.x = 5.4;
g.Position.y = 1.2;

--bird = {
--    Position = {
--        x = 1.2,
--        y = 3.4
--    },
--    init = function(self)
--        print(self.Position.x .. "," .. self.Position.y)
--    end
--}
--
--dog = {
--    Position = {
--        x = 6.5,
--        y = 1.3
--    },
--    init = function(self)
--        print(self.Position.x .. "," .. self.Position.y)
--    end
--}
--
--birdSpawn = game.spawn(bird);
--birdSpawn:init()
--
--dogSpawn = game.spawn(dog);
--dogSpawn:init()
--dogSpawn.Position.x = 37.5
