local newWorld = {
    Register = function(self)
        self.unitSize = 8;
        self:createLayer(0, {
            texture = { file = "Assets/world/world1/layers/0/texture.png" },
            normal = { file = "Assets/world/world1/layers/0/normal.png" },
            hitbox =  "Assets/world/world1/layers/0/hitbox.png"
        });
        self:createLayer(1, {
            texture = { file = "Assets/world/world1/layers/1/texture.png" },
            normal = { file = "Assets/world/world1/layers/1/normal.png" },
            hitbox = "Assets/world/world1/layers/1/hitbox.png"
        });
        self:createDecoLayer(7, {
            texture = { file = "Assets/world/world1/layers/deco/forestTileBack.png" },
            normal = { file = "Assets/world/world1/layers/deco/normal.png" },
        });
        self:createDecoLayer(5.5, {
            texture = { file = "Assets/world/world1/layers/deco/forestTileMid.png" },
            normal = { file = "Assets/world/world1/layers/deco/normal.png" },
        });
        self:createDecoLayer(4, {
            texture = { file = "Assets/world/world1/layers/deco/forestTileFront.png" },
            normal = { file = "Assets/world/world1/layers/deco/normal.png" },
        });
    end,
    Generate = function(self)
    end
}

game.worldRegister(newWorld);
