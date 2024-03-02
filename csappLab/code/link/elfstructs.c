typedef struct {
    long offset; /* Offset of the reference to relocate */
    long type:32, /* Relocation type */
    symbol:32; /* Symbol table index */
    long addend; /* Constant part of relocation expression */
} Elf64_Rela;

/* 假设ADDR(s)和ADDR(r.symbol)表示section s和符号r.symbol在运行时候的地址 */
foreach section s {
    foreach relocation entry r {
        refptr= s + r.offset; /* ptr to reference to be relocated */

        /* PC相对寻址 */
        if (r.type == R_X86_64_PC32) {
            refaddr = ADDR(s) + r.offset; /* ref’s run-time address */
            *refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr);
        }

        /* 绝对寻址 */
        if (r.type == R_X86_64_32)
            *refptr = (unsigned) (ADDR(r.symbol) + r.addend);
    }
 }

1 0000000000000000 <main>:
2       0: 48 83 ec 08          sub $0x8,%rsp
3       4: be 02 00 00 00       mov $0x2,%esi
4       9: bf 00 00 00 00       mov $0x0,%edi               //%edi = &array
5                           a: R_X86_64_32 array            //Relocation entry
       0xe↓  ↓0xf
6       e: e8 00 00 00 00       callq 13 <main+0x13>        //sum()
7                           f: R_X86_64_PC32 sum-0x4        //Relocation entry
8       13: 48 83 c4 08         add $0x8,%rsp
9       17: c3                  retq
0xe = 14   0xf = 15   0x13=19
/*************重定位PC相对引用的例子：调用sum函数************

    让我们先解析一下第6行的含义  e: e8 00 00 00 00       callq 13 <main+0x13>
    0xe表示call指令开始于section offset的位置，0xe8是1-byte的opcode；
    后面的0则是为32-bit-PC相对引用预留的placeholder(占位符)；
    callq 13 <main+0x13>表示call指令与call调用的sum结束后应该执行section offset = 0x13处的指令(add)

    然后我们查看一下相应的relocation entry：
        r.offset = 0xf
        r.symbol = sum
        r.type = R_X86_64_PC32
        r.addend = -4
    因为在介绍Elf64_Rela的成员时候不太理解，我们重新解释一下各部分代表的值：
    r.offset代表call这条指令的操作数地址(依旧是相对<main>的offset位置),图示标出来在0xe8的后面一个byte；
    r.symbol表示要重定位的符号名，这里是对函数sum进行重定向；
    r.type的值是32种的里的一个，也就前文提到的ELF定义了32种不同的重定位类型；
    r.attend一个固定的偏移量，取决于该指令(符号)的位置与下一指令(符号)的位置

    然后我们严格按照重定位的算法来执行：

    1、refptr= s + r.offset = main + r.offset = 0x0 + 0xf = 0xf
    s(应该)是距.text section的的offset,比如这里就是距<main>的offset,s也就是main
    所以refptr是该文本中距离0000000000000000 <main> r.offset处的地址,也就是第6行指令call的操作数

    2、refaddr = ADDR(s) + r.offset = ADDR(main) + r.offset = 0x4004d0 + 0xf = 0x400df
    假设ADDR(main) = ADDR(.text) = 0x4004d0， 回想一下ELF可重定位目标文件的格式
    由于main.o的代码存放在ELF文件.text section中,所以ADDR(main)理所应当也该等于ADDR(.text)
    refaddr(0x400df)实际上就是运行时call指令操作数的地址

    假设ADDR(sum) = ADDR(r.symbol) = 0x4004e8
    3、*refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr) = ADDR(sum) + r.addend - refaddr
               = 0x4004e8 + (-4) - 0x400df = 0x5
    假设ADDR(sum) = ADDR(r.symbol) = 0x4004e8
    用中文翻译一遍我们就能理解*refptr的含义:
    *refptr = 运行时符号sum的地址 + r.addend常量 - 运行时指令call操作数的地址
    同时代入refaddr的值可以得到
    *refptr = ADDR(r.symbol) - ADDR(main) + r.addend  - r.offset
    也就是说*refptr存储的也是一个offset，至于是什么offset继续往下看。

    至此我们可以得到重定位结束的第6行
 old：        e: e8 00 00 00 00       callq 13 <main+0x13>
 new：   4004de: e8 05 00 00 00       callq 4004e8 <sum>
 next：  4004e3: 48 83 c4 08          add   $0x8,%rsp
                             r.addend = -4                            *refptr = 0x5       24 - 14 = 10 = 4 + 4
    代码运行时的位置: 0x4004de->call   call: 0x400def  PC=0x4004e3(-0x4004d0)   ADDR(r.symbol)=0x4004e8(-0x4004d0)
    存放在ELF中的位置:     0xe->call   call:      0xf  PC=0x13=(0xf + 0x4)            r.symbol=0x18=(0xf + 0x4 + 0x5)
    整体做了一次ADDR(main)=0x4004d0大小的偏移
    
    所以现在我们可以说近乎完全搞懂Relocation操作了
    简单的代换一下：
    *refptr = ADDR(r.symbol) - ADDR(main) + r.addend  - r.offset

    在本例子中 r.addend = -4是因为程序计数器PC在执行一个指令的时候需要把该指令的内容读完(自然而然也就指向了下一条指令的地址)
              *refptr = 0x5是因为调用call后下一步PC需要指向sum函数的地址，而此时差值就是0x5

    再回顾一下前面的知识，当我们执行callq 4004e8 <sum>这条指令的时候CPU在做什么, 就能更好的理解上面的内容了。
    将PC的值压入栈中, 注意此时PC的值是下一条指令的地址。
    更新PC的值, PC=PC+*refptr = 0x4004e8(->sum)
*/