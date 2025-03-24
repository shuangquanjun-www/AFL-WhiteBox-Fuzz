#include "common.h"

int main(int argc, char *argv[]) {
    unsigned char flag;
    if (read(0, &flag, 1) < 1) return 1;

    // 分配两块内存
    char *p = malloc(64);
    char *q = malloc(64);
    if (!p || !q) return 1; // 内存分配失败则退出

    // 初始化内存以避免未使用警告（非必需）
    memset(p, 0, 64);
    memset(q, 0, 64);

    // 根据flag的不同位执行不同的释放策略
    if (flag & 0x1) {
        // 如果最低位为1，执行double free
        free(p);
        free(p);  // 第二次释放同一指针，错误
    }
    if (flag & 0x2) {
        // 第二位为1，释放未分配的指针
        char *r = p + 32;
        free(r);  // r并非malloc返回的起始指针，错误
    }
    if (flag & 0x4) {
        // 第三位为1，正常释放两次不同指针（正确用法，用于有正常路径）
        free(p);
        free(q);
        // 将指针置NULL防止悬挂指针 (仅作为示范，实际程序到这里结束)
        p = NULL;
        q = NULL;
    }

    // 不论上述哪个分支，程序结束前我们也确保未释放的指针释放，避免内存泄漏
    // 但如果程序已经因为错误free异常终止，下面代码可能不会执行
    if ((flag & 0x4) == 0) {
        // 若正常释放路径未执行，则我们在结束前释放尚未释放的指针
        free(p);
        free(q);
    }

    return 0;
}