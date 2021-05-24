--'Aa3s-lUa-w3stEdQ'
print("Give me the flag, and let me destroy it: ")
s = io.read()

if #s ~= 16 then
    print("Fake flag.")
    os.exit(-1)
end

ctr = string.rep('\x00',16)
-- ctr = string.rep('\x01',16)
stream = enc(ctr)

--s ^= stream
result = ''
for i = 1, 16 do
    num = string.byte(s, i) ~ string.byte(stream, i)
    -- print(string.byte(s, i), string.byte(stream, i), num)
    result = result .. string.char(num)
end
s = result

s = enc(s)

-- compare
result = '\x55\xE2\xD0\x4A\x91\xAB\x5B\x89\x86\xD8\xE2\x83\x1F\x51\x8F\x88'
for i = 1, #s do
    --io.write(string.format('%02X', string.byte(s, i)))
    if string.byte(result,i) ~= string.byte(s,i) then
        print("Fake flag.")
        os.exit(-1)
    end
end
print("Flag destroyed.")
