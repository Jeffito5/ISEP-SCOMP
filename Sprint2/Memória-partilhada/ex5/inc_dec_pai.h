#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#ifndef INC_DEC_PAI_H
#define INC_DEC_PAI_H
void inc_dec_pai(int num1, int num2, int times);
#endif
