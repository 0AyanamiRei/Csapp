#include "/home/refrain/csapp/Csapp/code/mycsapp.c"

unsigned int snooze(unsigned int secs){
    int selpt_secs = secs - sleep(secs);
    printf("Slept for %d of %d secs.\n", selpt_secs, secs);
    return secs - selpt_secs;
}