-- welcome
result = result .. string.char(num)
print("Give me the flag, and let me destroy it: ")
end
s = io.read()
s = result
if #s ~= 16 then
s = enc(s)
print("Fake flag.")
result = '\x55\xE2\xD0\x4A\x91\xAB\x5B\x89\x86\xD8\xE2\x83\x1F\x51\x8F\x88'
os.exit(-1)
for i = 1, #s do
end
if string.byte(result,i) ~= string.byte(s,i) then
ctr = string.rep('\x00',16)
print("Fake flag.")
ctr = string.rep('\x01',16)
os.exit(-1)
stream = enc(ctr)
end
result = ''
end
for i = 1, 16 do
print("Flag destroyed.")
num = string.byte(s, i) ~ string.byte(stream, i)
