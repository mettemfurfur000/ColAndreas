--local ca = require("calua")

local ca = package.loadlib("calua.dll", "luaopen_calua_lib")
ca = ca()

local time_start = os.clock()
local ret, x, y, z, w

ca.connect()

-- print(ca.raycast_line(2000, 1500, 10, 2000, 1500, 1))
-- print(ca.raycast_line(2001, 1500, 10, 1999, 1500, 1))

print(ca.raycast_line(2833, 1290, 12, 2833, 1290, 8))

local tests = 100
for i = 1, tests do
    ret, x, y, z, w = ca.raycast_line(
        math.random(-3000, 3000), math.random(-3000, 3000), math.random(-30, 60),
        math.random(-3000, 3000), math.random(-3000, 3000), math.random(-30, 60)
    )
    if (ret == 1) then
        print(ret, x, y, z, w)
    end
end

ca.exit()

local took_time = os.clock() - time_start
print("took " .. took_time .. " seconds")
-- print("rps: " .. tests / took_time)
