#include "/home/refrain/csapp/Csapp/code/mycsapp.c"
/* 使用mmap将一个任意大小的磁盘文件复制到stdout, 输入文件名字以一个命令行参数形式传递 */
/* Just try !*/

/*
    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    我们有文件名="filename";
→   stdout = mmap(NULL, ?, int prot, int flags, ?, off_t offset);

    在下一章(cmu15-213是先讲的I/O) 我们会学到open,close等关于文件的函数
    int open(const char *pathname, int flags, mode_t mode);
    int creat(const char *pathname, mode_t mode);
    int close(int fd);
*/
int main(int argc, char **argv)
{
    char *addr;
    int fd;
    struct stat file_state;
    char *filename = argv[1];
    int flags = O_RDWR;

    fd = open(filename, flags);
    if ((fd = open(filename, flags))  < 0)
	    unix_error("Open error!");
    
    /* now fill the code! */
    fstat(fd, &file_state);
    
    addr = mmap(NULL, file_state.st_size , PROT_READ, MAP_PRIVATE, fd, 0);
    printf("%s\n", addr);

    Close(fd);
    return 0;
}