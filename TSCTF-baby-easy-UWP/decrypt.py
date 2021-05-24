

class randomizer:
    '''
    完全二叉树的数组表示（层序遍历存放时），下标为x的节点的左孩子下标为2x，右孩子为2x+1。（下标从1开始）
    加密是层序遍历转中序遍历。
    '''
    def __init__(self):
        self.result = ''
        self.to_encrypt = ''
        self.length = 0
    def traverse(self, root):
        if root > self.length:
            return
        self.traverse(2*root)
        self.result += self.to_encrypt[root-1]
        self.traverse(2*root+1)

    def randomize(self, s):
        self.length = len(s)
        self.to_encrypt = s
        self.traverse(1)
        return self.result


class derandomizer:
    '''
    解密时是建树，中序遍历的过程中填进去，
    '''
    def __init__(self):
        self.result = []
        self.to_decrypt = ''
        self.length= 0
        self.counter = 0
    def traverse(self, root):
        if root > self.length:
            return
        self.traverse(2*root)
        self.result[root-1] = self.to_decrypt[self.counter]; self.counter += 1
        self.traverse(2*root+1)
    def derandomize(self, s):
        self.length = len(s)
        self.to_decrypt = s
        self.result = ['a' for i in range(self.length)]
        self.traverse(1)
        return ''.join(self.result)

def xorer(s):
    b = s.encode('ascii')
    result = []
    for i in b:
        result.append(i ^ ord('U'))
    return bytes(result)

def dexorer(b):
    result = []
    for i in b:
        result.append(i ^ ord('U'))
    return bytes(result).decode("ascii")

cipertext = b'cdee\nme\x17gfaabx\x164\x1d`\x10f6\x10>794;bf&g,e\x0cm'

randomized = dexorer(cipertext)
# print(randomized)

derandomized = derandomizer().derandomize(randomized)
print(derandomized)

# ------------

# derandomized = "347Baby03-5EasY18247CHEckln320860_0"

# randomized = randomizer().randomize(derandomized)
# print(randomized)

# xored = xorer(randomized)
# print(xored)
