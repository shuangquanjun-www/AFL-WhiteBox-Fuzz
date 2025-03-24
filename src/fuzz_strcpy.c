#include "common.h"

int main(int argc, char *argv[]) {
    char dest[64];
    char src[MAX_BUF_SIZE];
    // 将标准输入读取到src缓冲区
    ssize_t bytes_read = read(0, src, MAX_BUF_SIZE - 1);
    if (bytes_read < 0) return 1;
    // 确保源以'\0'终止
    src[bytes_read] = '\0';

    /* 调用strcpy进行复制 */
    strcpy(dest, src);

    // 如果输入过长，strcpy 将导致dest缓冲区溢出，引发崩溃
    // 我们在这里不进行额外检查，故意让潜在错误发生，交由AFL捕获

    return 0;
}