#include "common.h"

int main(int argc, char *argv[]) {
    char data[MAX_BUF_SIZE];
    ssize_t bytes = read(0, data, MAX_BUF_SIZE-1);
    if (bytes < 0) return 1;
    data[bytes] = '\0';  // 确保数据以'\0'结尾，便于作为文本

    // 使用fmemopen将数据作为内存文件
    FILE *fp = fmemopen(data, bytes, "r");
    if (fp == NULL) {
        return 1; // 如果失败（理论上不会，因为内存分配应该成功）
    }

    // 从内存文件读取一行数据
    char line[128];
    if (fgets(line, sizeof(line), fp) != NULL) {
        // 读取成功，可对line进行一些处理或检查
        // 我们可以检查line长度或内容以形成不同的分支
        size_t l = strlen(line);
        if (l == 0) {
            // 行为空
        } else if (l == sizeof(line)-1 && line[l-1] != '\n') {
            // 一行被截断（没有读到换行且缓冲用满）
        } else {
            // 正常读取到一行，包括换行符或文件结束
        }
    } else {
        // 如果fgets返回NULL，可以检查feof和ferror
        if (feof(fp)) {
            // 文件结束
        }
        if (ferror(fp)) {
            // 发生读取错误
        }
    }

    fclose(fp);

    // 额外：测试fopen对随机文件名（较短）的行为
    char fname[32];
    int namelen = bytes < 31 ? bytes : 31;
    memcpy(fname, data, namelen);
    fname[namelen] = '\0';
    FILE *fp2 = fopen(fname, "w");
    if (fp2 != NULL) {
        // 文件名也许有效，我们简单写入一些数据再关闭
        fputs("test", fp2);
        fclose(fp2);
        // 删除该文件避免遗留（可选）
        remove(fname);
    } else {
        // 打开失败，可能文件名不合法或包含不可用字符
        // 这里不做特别处理
    }

    return 0;
}