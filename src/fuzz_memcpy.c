#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char input[MAX_BUF_SIZE];
    int bytes = read(0, input, MAX_BUF_SIZE);
    if (bytes < 4) {
        // 输入太短，无法取得参数
        return 1;
    }
    // 解析输入参数
    uint16_t n = (input[0] << 8) | input[1];  // 前2字节作为拷贝长度n
    unsigned char src_offset = input[2];      // 第3字节作为源偏移
    unsigned char dest_offset = input[3];     // 第4字节作为目标偏移

    // 准备源和目标缓冲区
    unsigned char source[128];
    unsigned char dest[128];
    memset(source, 0, sizeof(source));
    memset(dest, 0, sizeof(dest));
    // 将输入剩余部分填充到source（尽量填充，如果不足source长度，其余部分保持0）
    int data_len = bytes - 4;
    if (data_len > 0) {
        int copy_len = data_len < 128 ? data_len : 128;
        memcpy(source, input + 4, copy_len);
    }

    // 计算实际可用的最大n，防止无符号16位n太大
    uint16_t max_n = 128;
    if (src_offset > 128) src_offset = 128;
    if (dest_offset > 128) dest_offset = 128;
    if (n > max_n) n = max_n;
    // 这里故意不严格检查src_offset + n或dest_offset + n是否超过128，
    // 让潜在的越界发生，从而测试memcpy在越界情况下的表现

    // 执行内存拷贝
    memcpy(dest + dest_offset, source + src_offset, n);

    // 简单校验：如果发生重叠(在本例中不会出现，因为我们用独立数组)或越界，可能已经导致崩溃
    // 可以根据实际需要添加检查，比如:
    // if (src_offset + n > 128 || dest_offset + n > 128) { ... }

    return 0;
}