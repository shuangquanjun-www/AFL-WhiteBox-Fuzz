#include "common.h"

int main(int argc, char *argv[]) {
    char str[MAX_BUF_SIZE];
    ssize_t bytes = read(0, str, MAX_BUF_SIZE - 1);
    if (bytes < 0) return 1;
    str[bytes] = '\0';  // 确保字符串终止

    // 调用 atoi 将字符串转换为整数
    int value = atoi(str);

    // 根据转换结果简单分类，以增加路径多样性
    if (value == 0) {
        // 转换结果为0，可能是输入不是数字或就是"0"
        // 可以检查输入是否真的数字开始
        if (bytes > 0 && (str[0] < '0' || str[0] > '9')) {
            // 非数字开头导致结果0
        } else {
            // 输入数字但结果为0（如 "0", "00", 或 "000123" 前导0）
        }
    } else if (value == INT_MAX || value == INT_MIN) {
        // 可能溢出或达到边界
    } else {
        // 一般中间值
    }

    return 0;
}