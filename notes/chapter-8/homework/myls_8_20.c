/* Read me!
 * 使用execve编写一个叫"myls"的程序，行为与/bin/ls程序一样, 你的程序也应该接受相同的命令行参数, 解释同样的环境变量
 * 产生相同的输出
 * 
 * 
 * Some recommend!
 * ls程序从COLUMNS环境变量中获得屏幕的宽度, 默认假设宽80列, 因此你可以设置其小于80来检查你对环境变量的处理
 * e.g.
 * linux> setenv COLUMNS 40(修改为40)
 * linux> ./myls
 *   ·
 *   ·   // 输出40列的宽
 *   ·
 * linux> unsetenv COLUMNS(默认为80)
*/
/*** Just try and believe yourself !! ***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv, char **envp)
{   
    if (setenv("COLUMNS", "40", 1) == -1) {
        perror("setenv");
        return 1;
    }

    argv[0] = "/bin/ls";
    execve("/bin/ls", argv, envp);

    exit(0);
}