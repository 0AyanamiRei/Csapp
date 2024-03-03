#include"/home/refrain/csapp/Csapp/code/mycsapp.c"

jmp_buf env1, env2;

void func2() {
    printf("In func2, about to jump back to func1\n");
    longjmp(env1, 2);
}

void func1() {
    if (setjmp(env1) == 0) {
        printf("First time through func1\n");
        setjmp(env2);  // 这是"最近一次"的setjmp调用
        func2(); /* in func2 we jump to the setjmp(env1) and return 2 != 0, so is go "else"*/
    }
    else {
        printf("Back in func1 after longjmp\n");
    }
}

int main() {
    func1();
    return 0;
}

/*
the output is :
    First time through func1
    In func2, about to jump back to func1
    Back in func1 after longjmp
*/