#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char buf[4];
    int bytes = read(0, buf, 4);
    if (bytes <= 0) return 1;
    // 如果读取字节不足4，剩余字节补0
    for (int i = bytes; i < 4; ++i) {
        buf[i] = 0;
    }
    // 将4字节解释为int
    int x;
    memcpy(&x, buf, sizeof(x));
    // 计算绝对值
    int y = abs(x);

    // 检查特殊边界情况
    if (x == INT_MIN) {
        // 记录INT_MIN的处理（abs(INT_MIN)未定义，很多实现返回INT_MIN本身）
        // 可以打印提示或标记
        // printf("abs(INT_MIN) called, result = %d\n", y);
    }
    if (y < 0) {
        // 理论上abs结果不应为负，如果为负说明遇到了INT_MIN情况
        // 这里可以人为触发崩溃以标记此问题，例如:
        abort();
    }

    return 0;
}