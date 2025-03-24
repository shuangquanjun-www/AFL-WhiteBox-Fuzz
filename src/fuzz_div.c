#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char buf[8];
    int bytes = read(0, buf, 8);
    if (bytes <= 0) return 1;
    // 不足8字节时填充0
    for (int i = bytes; i < 8; ++i) buf[i] = 0;
    int a, b;
    memcpy(&a, buf, sizeof(a));
    memcpy(&b, buf+4, sizeof(b));

    // 进行除法运算
    int result = a / b;   // 如果b为0，此处会引发异常(SIGFPE)

    // 根据结果简单分类（增加分支覆盖率）
    if (result == 0) {
        // 商为0的情况
    } else if (result < 0) {
        // 商为负
    } else {
        // 商为正
    }

    return 0;
}