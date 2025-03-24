#include "common.h"

int main(int argc, char *argv[]) {
    /* 读取标准输入内容到buffer */
    char buf[MAX_BUF_SIZE];
    ssize_t bytes_read = read(0, buf, MAX_BUF_SIZE - 1);
    if (bytes_read < 0) {
        return 1;  // 读取失败，退出
    }
    /* 确保buffer以'\0'结尾，避免strlen越界 */
    buf[bytes_read] = '\0';

    // 调用strlen并获取结果
    size_t len = strlen(buf);

    // 为了避免影响AFL效率，默认不打印，可选打印用于调试
    // printf("Input length: %zu\n", len);

    return 0;
}