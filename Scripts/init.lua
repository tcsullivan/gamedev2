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
local q = comp.Position(6, 3)
print(q.x .. "," .. q.y)

print("HEY")
