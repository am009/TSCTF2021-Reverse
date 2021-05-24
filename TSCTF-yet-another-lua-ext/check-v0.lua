-- welcome

print("Give me the flag, and let me destroy it: ")

s = io.read()

if #s ~= 16 then

    print("Fake flag.")

    os.exit(-1)

end

ctr = string.rep('\x00',16)

-- ctr = string.rep('\x01',16)

stream = enc(ctr)

result = ''

for i = 1, 16 do

    num = string.byte(s, i) ~ string.byte(stream, i)

    result = result .. string.char(num)

end

s = result

s = enc(s)

result = '\x55\xE2\xD0\x4A\x91\xAB\x5B\x89\x86\xD8\xE2\x83\x1F\x51\x8F\x88'

for i = 1, #s do

    if string.byte(result,i) ~= string.byte(s,i) then

        print("Fake flag.")

        os.exit(-1)

    end

end

print("Flag destroyed.")
