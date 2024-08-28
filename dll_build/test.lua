--local ca = require("calua")

local ca = package.loadlib("calua.dll", "luaopen_calua_lib")
ca = ca()

local time_start = os.clock()
local ret, x, y, z, rot_x, rot_y, rot_z, rot_w, orig_x, orig_y, orig_z, model_id

ca.connect()

-- print(ca.raycast_line(2000, 1500, 10, 2000, 1500, 1))
-- print(ca.raycast_line(2001, 1500, 10, 1999, 1500, 1))

print(ca.raycast_line(2833, 1290, 12, 2833, 1290, 8))

math.randomseed(os.time())

local tests = 200
for i = 1, tests do
    ret, x, y, z, rot_x, rot_y, rot_z, rot_w, orig_x, orig_y, orig_z, model_id = ca.raycast_line_extra(
        math.random(-3000, 3000), math.random(-3000, 3000), math.random(-30, 60),
        math.random(-3000, 3000), math.random(-3000, 3000), math.random(-30, 60)
    )
    if ret == 1 then
        print("hit pos: [", x, y, z, "] rotation quanterion: [", rot_x, rot_y, rot_z, rot_w, "] model origin: [", orig_x,
            orig_y, orig_z, "] model id: ", model_id)
    end
end

ca.exit()

local took_time = os.clock() - time_start
print("took " .. took_time .. " seconds")
-- print("rps: " .. tests / took_time)
