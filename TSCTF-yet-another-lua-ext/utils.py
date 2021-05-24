
s = \
'''55E2D04A91AB5B8986D8E2831F518F88'''
length = len(s)
out = []
for i in range(0, length, 2):
    out.append('0x'+s[i:i+2] + ', ')
# out.reverse()
for i in range(len(out)):
    if i % 16 == 0:
        print()
    print(out[i], end='')
# print(out)

print('\n------')
s = 'Aa3s-lUa-w3stEdQ'
for c in s:
    print(hex(ord(c))[2:], end='')

print()