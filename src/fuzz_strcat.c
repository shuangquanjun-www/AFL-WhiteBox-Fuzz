#include "common.h"

int main(int argc, char *argv[]) {
    char dest[80];
    char src[MAX_BUF_SIZE];
    // 初始化dest为一个固定字符串，作为拼接起点
    strcpy(dest, "FUZZ");  // dest开始内容为"FUZZ"
    size_t dest_len = strlen(dest);  // 当前dest长度(不包含\0)

    // 读取输入到src
    ssize_t bytes_read = read(0, src, MAX_BUF_SIZE - 1);
    if (bytes_read < 0) return 1;
    src[bytes_read] = '\0';

    /* 拼接 */
    strcat(dest, src);

    // 如果src太长，dest可能溢出
    return 0;
}