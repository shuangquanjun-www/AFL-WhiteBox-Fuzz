#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char buf[256];
    int bytes = read(0, buf, sizeof(buf));
    if (bytes < 3) return 1;
    unsigned char offset1 = buf[0];
    unsigned char offset2 = buf[1];
    unsigned char n = buf[2];
    // 为简单，n使用1字节0-255
    // 可以扩展为2字节类似memcpy测试

    // 计算指针
    unsigned char *src_ptr = buf + offset1;
    unsigned char *dst_ptr = buf + offset2;
    // 这里直接在同一buf上做，将输入本身后续数据视为初始内容

    memmove(dst_ptr, src_ptr, n);

    return 0;
}