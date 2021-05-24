## not-wasm

难度：4/5，考点：wasm内存模型，修改版DES

题目描述：

纯天然，不含web assembly添加剂

本题的flag不含TSCTF{}包裹，请解出后加上再提交。

题目附件：

not-wasm.zip



### 编译过程

1. mydes2.c文件通过emcc编译成enc.wasm 编译命令见mydes2.c文件内注释
2. wasm2wat得到enc.wat文件。
3. 得到的enc.c和enc.h放到wasm2c文件夹，结合写的main函数编译，编译命令见main函数的顶部注释。
4. 编译前暴露出memory变量：去掉memory变量的`static`。



### WP

本题基于wasm逆向过程中的wasm2c工具生成，通过增加main函数将生成的c语言跑了起来。希望做题者能理解非普通的内存访问情况，同时在逆向的过程中能够得以熟悉wasm的内存访问。但是本题的重点还是考察算法逆向。

在出题过程中调整编译选项确保了emscripten产生的wasm代码量较少，在第二次编译的时候尽量确保了没有开启link time optimization改变程序的函数结构。最后去掉了几个和题目关联较大的符号信息，保留了剩下的大量符号信息。

DES修改的地方是去掉了IP和FP置换。一个可以对照标准DES过程的网站：https://paginas.fe.up.pt/~ei10109/ca/des.html 

逆向遇到困难的，可以考虑直接对照wasm2c生成的C源码，以及最原始的des C源码看。题目部分原始源码如下（完整版在GitHub上的`mydes2.c`）：

```c
unsigned char key[9] = "C1nE_KlB";
unsigned char result[20] = {0x82, 0xb0, 0xab, 0xe4, 0x2d, 0x54, 0x5b, 0x2b, 0xd7, 0xf2, 0x54, 0xb8, 0x1f, 0x1b, 0xc7, 0x92, 0x00};
unsigned char inp[20] = {0};
unsigned char cmp[20] = {0};

int check() {
    get_subkey(key);
    encryption(inp, cmp);
    encryption(inp+8, cmp+8);
    cmp[16] = '\x00';
    for(int i=0;i<16;i++) {
        if (cmp[i] != result[i]) {
            return 0;
        }
    }
    return 1;
}

void encryption(unsigned char *plaintext, unsigned char *ciphertext)
{
    unsigned char array_plaintext[64];
    unsigned char f_result[32];
    unsigned char *left_array;
    unsigned char *right_array;
    unsigned char *tmp;

    byte2Bit(plaintext, array_plaintext, 8);

    left_array = array_plaintext;
    right_array = array_plaintext + 32;
    D(printf("before: "); print_32bit(left_array); printf(" : "); print_32bit(right_array); printf("\n");)
    int i;
    for (i = 0; i < 16; ++i)
    {
        f_func(right_array, f_result, &subkey[i][0]);
        byteXOR(left_array, f_result, 32);

        tmp = left_array;
        left_array = right_array;
        right_array = tmp;
    }

    bit2Byte(array_plaintext, ciphertext, 8);
}

void byte2Bit(unsigned char *in, unsigned char *out, int len)
{
    int i, j;
    for (i = 0; i < len; ++i)
        for (j = 0; j < 8; ++j)
        {
            out[i * 8 + j] = (in[i] >> (7 - j)) & 0x1;
        }
}

void bit2Byte(unsigned char *in, unsigned char *out, int len)
{
    int i, j;
    unsigned char temp;
    for (i = 0; i < len; ++i)
    {
        temp = in[i * 8] & 0x1;
        for (j = 1; j < 8; ++j)
        {
            temp = temp << 1;
            temp = temp + (in[i * 8 + j] & 0x1);
        }
        out[i] = temp;
    }
}

void e_expand(unsigned char *in, unsigned char *out)
{
    // unsigned char temp[48];
    int i;

    // memcpy(temp, in, 48);
    for (i = 0; i < 48; ++i)
        out[i] = in[E_Table[i] - 1];
}

void s_replace(unsigned char *in, unsigned char *out)
{
    int i, j;
    int raw, col;
    char temp;
    for (i = 0, j = 0; i < 8; ++i)
    {
        raw = ((in[i * 6] << 1) | (in[i * 6 + 5])) & 0x3;                                                   //高低两位
        col = ((in[i * 6 + 1] << 3) | (in[i * 6 + 2] << 2) | (in[i * 6 + 3] << 1) | (in[i * 6 + 4])) & 0xF; //中间四位
        temp = S_Box[i][raw][col] & 0xF;
        out[j] = (temp >> 3) & 0x1;
        out[j + 1] = (temp >> 2) & 0x1;
        out[j + 2] = (temp >> 1) & 0x1;
        out[j + 3] = (temp)&0x1;
        j += 4;
    }
}

void p_replace(unsigned char *in, unsigned char *out)
{
    int i;

    for (i = 0; i < 32; ++i)
    {
        out[i] = in[P_Table[i] - 1];
    }
}

void byteXOR(unsigned char *a, unsigned char *b, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        a[i] = (a[i] + b[i]) & 0x1;
    }
}

void f_func(unsigned char *right, unsigned char *out, unsigned char *ki)
{
    unsigned char expand_result[48];
    unsigned char replace_result[32];

    e_expand(right, expand_result);
    byteXOR(expand_result, ki, 48);
    s_replace(expand_result, replace_result);
    p_replace(replace_result, expand_result);
    memcpy(out, expand_result, 32);
}

void pc1_replace(unsigned char *in, unsigned char *out)
{
    int i;
    for (i = 0; i < 56; ++i)
    {
        out[i] = in[PC1_Table[i] - 1];
    }
}

void pc2_replace(unsigned char *in, unsigned char *out)
{
    int i;
    for (i = 0; i < 48; ++i)
    {
        out[i] = in[PC2_Table[i] - 1];
    }
}

void shift_left(unsigned char *in, unsigned char *out, int len)
{
    // unsigned char temp[28];
    int i, j;

    // memcpy(temp,in,28);
    for (i = len, j = 0; i < 28; ++i)
    {
        out[j++] = in[i];
    }
    for (i = 0; i < len; ++i)
    {
        out[j++] = in[i];
    }
}
//use user's input key to generate 16 subkey
void get_subkey(unsigned char *key)
{
    unsigned char key_temp[64];
    unsigned char keybuf1[56];
    unsigned char keybuf2[56];
    unsigned char *pc1_result = keybuf1;
    unsigned char *after_rot = keybuf2;

    byte2Bit(key, key_temp, 8);
    pc1_replace(key_temp, pc1_result);

    for (int i = 0; i < 16; ++i)
    {
        shift_left(pc1_result, after_rot, LOOP_Table[i]);
        shift_left(pc1_result + 28, after_rot + 28, LOOP_Table[i]);

        unsigned char *tmp = pc1_result;
        pc1_result = after_rot;
        after_rot = tmp;

        pc2_replace(pc1_result, subkey[i]);
    }
}
```

wasm-rt会初始化完整的内存空间，内存空间都是在堆上的。在C语言里`memory.data[addr]`的方式就可以直接访问了。

部分函数对应关系：f2 - des_enc。sub_1880 - check。

C语言解密脚本在Github上的`mydes-decrypt-writeup.c`。

本来想要另外减少一轮加密，但是发现出题失误，好像还是16轮加密。。。导致hint给的不对。。尴尬，明明最后打包前检查了挺久的。。但是这算是难度减小了吧，也好。

