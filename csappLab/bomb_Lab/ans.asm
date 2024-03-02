0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq

  第一个问题答案:Border relations with Canada have never been better.


  0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers> // 得知要输入6个数字
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)  // 得知第一个数字填1
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax //将第1···5个数字给eax
  400f1a:	01 c0                	add    %eax,%eax // eax乘以2
  400f1c:	39 03                	cmp    %eax,(%rbx) // 得知这六个数字是公比为2的等比数列
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx  // 通过之前的代码二者相差0x14
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40> // rbx的地址每次加4
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx  //将地址给rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp //将地址给rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>

  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq
 // 第二个问题答案 1 2 4 8 16 32


  
test 输入 1 2 3 4 5 6
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  // 可以看到这里%esi 存储的是格式字符串 %d %d
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  // 输入格式检测,暂时未知
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  // %eax > 0x1 跳转
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
  // (rsp+0x8) > 0x7 跳转:爆炸 ps:等价于rdx此时地址指向的值(1)
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  // 0x402470+8*rax
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  // rsp+0xc处(第二个数字)的值要等于0x137
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq   
// 1 311能过, 实际上根据第一个数字不超过7就会由于*0x402470(,%rax,8)结果不同匹配
// 的第二个数字不同, 0 207; 2 707都是可以的
  

000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp
  ; 开了3层
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax
  401024:	e8 c7 fb ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  ; 做这道题的时候发现调用的这个函数是将命令行输入的参数向右入栈:
  ; x1 x2 x3 ··· xn,xn第一个入栈
  401029:	83 f8 02             	cmp    $0x2,%eax
  40102c:	75 07                	jne    401035 <phase_4+0x29>
  ; 可以看到限制了两个输入x1 x2
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)
  ; 要求x1输入小于等于14的无符号整数,经过后续的func4,发现x1只能等于0 
  ; (可以发现参数入栈是x1 x2→的顺序,所以x1靠近rsp)
  401033:	76 05                	jbe    40103a <phase_4+0x2e>
  401035:	e8 00 04 00 00       	callq  40143a <explode_bomb>
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx
  40103f:	be 00 00 00 00       	mov    $0x0,%esi
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi
  401048:	e8 81 ff ff ff       	callq  400fce <func4>
  ; 调用函数后修改%eax的值
  40104d:	85 c0                	test   %eax,%eax
  40104f:	75 07                	jne    401058 <phase_4+0x4c>
  ; 要让%eax==0
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp)
  401056:	74 05                	je     40105d <phase_4+0x51>
  ; 要求此时的x2==0
  401058:	e8 dd 03 00 00       	callq  40143a <explode_bomb>
  40105d:	48 83 c4 18          	add    $0x18,%rsp
  401061:	c3                   	retq 

  ; 可以发现只有当第一个参数为0, func4才会让eax=0
  ; 所以输入的参数为0 0


  ; 传入的参数:
  ; %edx=$0xe  %edi=x1  %esi=0
  ; sub 0x8之后 栈中原有数据改变为: x1:0x10(%rsp), x2:0x14(%rsp)
  ; 
  0000000000400fce <func4>:
  400fce:	48 83 ec 08          	sub    $0x8,%rsp
  400fd2:	89 d0                	mov    %edx,%eax   ; eax = edx
  400fd4:	29 f0                	sub    %esi,%eax   ; eax -= esi
  400fd6:	89 c1                	mov    %eax,%ecx   ; ecx = eax
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx  ; ecx = 0(>0) else -1(<0)
  400fdb:	01 c8                	add    %ecx,%eax   ; eax += ecx
  400fdd:	d1 f8                	sar    %eax        ; 等价于sar %eax, 1 eax *= 2
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx ; ecx = rax + rsi
  400fe2:	39 f9                	cmp    %edi,%ecx
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24>
  ; %ecx <= %edi 则跳转, 否则下一步
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx  ;edx = rcx - 1
  400fe9:	e8 e0 ff ff ff       	callq  400fce <func4>   ;func()
  400fee:	01 c0                	add    %eax,%eax   ;eax *= 2
  400ff0:	eb 15                	jmp    401007 <func4+0x39> ; return
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax   ;eax = 0
  400ff7:	39 f9                	cmp    %edi,%ecx
  400ff9:	7d 0c                	jge    401007 <func4+0x39>
  ; ecx >= edi 则return
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi   ;esi = rcx + 1
  400ffe:	e8 cb ff ff ff       	callq  400fce <func4>   ;func()
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax  ;eax = 2*rax + 1
  401007:	48 83 c4 08          	add    $0x8,%rsp
  40100b:	c3                   	retq

; edx=0, esi=0, edi=x1, eax=2
; func4()
; {
;    eax = edx - esi
;    ecx = eax
;    if(ecx > 0) ecx=0 else ecx=-1
;    eax += ecx
;    eax /= 2
;    ecx = eax+esi
;    ----if(ecx <= edi)
;    {
;       eax = 0
;       if(ecx >= edi) return
;       esi = ecx+1
;       func()
;       eax = 2 * eax + 1
;       return
;    }
;    edx = ecx - 1
;    func()
;    eax = eax * 2
;    return


0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp
  401067:	48 89 fb             	mov    %rdi,%rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  ; %fs??  见下面
  401071:	00 00 
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  ; eax=0 清零
  40107a:	e8 9c 02 00 00       	callq  40131b <string_length>
  40107f:	83 f8 06             	cmp    $0x6,%eax
  ; 长度为6的字符串
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
  ; je 相等/零
  401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
  ; jmp 间接跳转
  ;----------------------------------------------------------
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx

  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  ; 0x4024b0再偏移0x10后才是正常的一句话 但是跟0xf做&运算后能够得到0x10
  ; 的数字是11100000没有这样的字符,因为movzbl移动后只保留了最后4位
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
  ; 循环,将
  ;-----------------------------------------------------------
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)
  ; 
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi
  ; 0x40245e处的字符串: "flyers"
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	callq  401338 <strings_not_equal>
  ; 让经过循环得到的字符串跟"flyers"相等即可,返回去推算其二进制数字
  ; 再对比ASCII码就可以了
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
  ; eax=0才进行跳转
  4010c6:	e8 6f 03 00 00       	callq  40143a <explode_bomb>
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
  ----
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>

  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010e5:	00 00 
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>
  ; 栈保护机制,如果这里的rax不等于fs:0x28这个特殊值就会调用下面这个函数
  4010e9:	e8 42 fa ff ff       	callq  400b30 <__stack_chk_fail@plt>
  4010ee:	48 83 c4 20          	add    $0x20,%rsp
  4010f2:	5b                   	pop    %rbx
  4010f3:	c3                   	retq   

  ;汇编中
  mov %fs:0x28, %rax的作用:栈保护功能,将一个特殊值fs:0x28存在栈的底部,函数
  运行结束后取出这个值和fs:0x28做比较,如果改变了说明栈被破坏,调用stack_chk_fail

  ;中间的循环:
  ; char *ptr = 0x4024b0;
  ; char *str = 0x6038c0; 输入的6个字符
  ; char *rsp = 0x7fffffffdda0;
  ; for(rax = 0; rax < 6; rax ++)
  ;     char rcx = str[rax];
  ;     *rsp = t;
  ;     char rdx = *rsp;
  ;     rdx = rdx & 0xf;
  ;     rdx = *(rdx+ptr);
  ;     *(rsp+rax+0x10) = rdx;
  ;解释一下就是对ptr做6次偏移,这6次的偏移量是由输入的6个字符(转成ASCII码)
  ;跟0xf做&运算后的结果,并将这6次偏移后的结果组成的新的字符串赋给
  ;了rsp+0x10处

  ;查阅后面的代码发现新组成的字符串要等于"flyers", 在ptr中分别为第
  ;10  16  15  6   7  8那么对应的偏移量减1即可
  ;只需要末四位为(1001)(1111)(1110)(0101)(0110)(0111)即可
  9ONEFG



  00000000004010f4 <phase_6>:
  4010f4:	41 56                	push   %r14
  4010f6:	41 55                	push   %r13
  4010f8:	41 54                	push   %r12
  4010fa:	55                   	push   %rbp
  4010fb:	53                   	push   %rbx
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp
  401100:	49 89 e5             	mov    %rsp,%r13
  401103:	48 89 e6             	mov    %rsp,%rsi
  401106:	e8 51 03 00 00       	callq  40145c <read_six_numbers>
  40110b:	49 89 e6             	mov    %rsp,%r14
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d
  401114:	4c 89 ed             	mov    %r13,%rbp
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax
  40111b:	83 e8 01             	sub    $0x1,%eax
  40111e:	83 f8 05             	cmp    $0x5,%eax
  401121:	76 05                	jbe    401128 <phase_6+0x34>
  401123:	e8 12 03 00 00       	callq  40143a <explode_bomb>
  401128:	41 83 c4 01          	add    $0x1,%r12d
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d
  401130:	74 21                	je     401153 <phase_6+0x5f>
  401132:	44 89 e3             	mov    %r12d,%ebx
  401135:	48 63 c3             	movslq %ebx,%rax
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp)
  40113e:	75 05                	jne    401145 <phase_6+0x51>
  401140:	e8 f5 02 00 00       	callq  40143a <explode_bomb>
  401145:	83 c3 01             	add    $0x1,%ebx
  401148:	83 fb 05             	cmp    $0x5,%ebx
  40114b:	7e e8                	jle    401135 <phase_6+0x41>
  40114d:	49 83 c5 04          	add    $0x4,%r13
  401151:	eb c1                	jmp    401114 <phase_6+0x20>
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi
  401158:	4c 89 f0             	mov    %r14,%rax
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx
  401160:	89 ca                	mov    %ecx,%edx
  401162:	2b 10                	sub    (%rax),%edx
  401164:	89 10                	mov    %edx,(%rax)
  401166:	48 83 c0 04          	add    $0x4,%rax
  40116a:	48 39 f0             	cmp    %rsi,%rax
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>
  40116f:	be 00 00 00 00       	mov    $0x0,%esi
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx
  40117a:	83 c0 01             	add    $0x1,%eax
  40117d:	39 c8                	cmp    %ecx,%eax
  40117f:	75 f5                	jne    401176 <phase_6+0x82>
  401181:	eb 05                	jmp    401188 <phase_6+0x94>
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2)
  40118d:	48 83 c6 04          	add    $0x4,%rsi
  401191:	48 83 fe 18          	cmp    $0x18,%rsi
  401195:	74 14                	je     4011ab <phase_6+0xb7>
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx
  40119a:	83 f9 01             	cmp    $0x1,%ecx
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>
  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi
  4011ba:	48 89 d9             	mov    %rbx,%rcx
  4011bd:	48 8b 10             	mov    (%rax),%rdx
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx)
  4011c4:	48 83 c0 08          	add    $0x8,%rax
  4011c8:	48 39 f0             	cmp    %rsi,%rax
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>
  4011cd:	48 89 d1             	mov    %rdx,%rcx
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)
  4011d9:	00 
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax
  4011e3:	8b 00                	mov    (%rax),%eax
  4011e5:	39 03                	cmp    %eax,(%rbx)
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>
  4011e9:	e8 4c 02 00 00       	callq  40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>
  4011f7:	48 83 c4 50          	add    $0x50,%rsp
  4011fb:	5b                   	pop    %rbx
  4011fc:	5d                   	pop    %rbp
  4011fd:	41 5c                	pop    %r12
  4011ff:	41 5d                	pop    %r13
  401201:	41 5e                	pop    %r14
  401203:	c3                   	retq