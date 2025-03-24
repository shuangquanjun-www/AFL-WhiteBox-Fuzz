#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char input[MAX_BUF_SIZE];
    int bytes = read(0, input, MAX_BUF_SIZE - 1);
    if (bytes <= 0) return 1;
    input[bytes] = '\0';  // 在整体输入末尾加终止符，便于处理

    // 计算分割点：一半位置
    int mid = bytes / 2;
    // 保证两部分各自以'\0'终止
    unsigned char *s1 = input;
    unsigned char *s2 = input + mid;
    input[mid] = '\0';  // 第一部分结尾
    // 第二部分的结尾已经是先前添加的终止符（或也可设置input[bytes]='\0'）

    // 进行字符串比较
    int result = strcmp((char*)s1, (char*)s2);

    // 可根据result做不同处理，以产生不同路径
    if (result == 0) {
        // s1 == s2
        // ...（这里不输出任何东西，仅用于分支）
    } else if (result < 0) {
        // s1 < s2
    } else {
        // s1 > s2
    }

    return 0;
}