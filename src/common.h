/* common.h - 公共头文件，包含常用库和辅助函数声明 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>    /* 数学函数需要 */
#include <ctype.h>   /* 可能用到字符处理函数 */
#include <limits.h>  /* 提供INT_MAX等定义 */
#include <errno.h>   /* 错误码errno用于部分函数 */
#include <unistd.h>

#define MAX_BUF_SIZE 256  /* 定义测试缓冲区的最大大小 */

#endif /* COMMON_H */
