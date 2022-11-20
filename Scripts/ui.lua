ui = {
    dialog = {
        buf = {},
        idx = 0,
        rdx = 0,

        queue = function(self, text)
            self.idx = self.idx + 1
            self.buf[self.idx] = text

            if self.idx == 1 then
                game.dialog(30, 50, 400, 100)
                game.puts("dialog", 36, 52, text)
            end
        end,

        update = function(self, mx, my)
            if self.idx > 0 then
                if mx > 30 and mx < 430 and my > 50 and my < 150 then
                    self.rdx = self.rdx + 1
                    if self.rdx == self.idx then
                        self.idx = 0
                        self.rdx = 0
                        game.dialogClear()
                    else
                        game.puts("dialog", 36, 52, self.buf[self.rdx + 1])
                    end
                end
            end
        end
    }
}

