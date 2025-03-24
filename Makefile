# 定义使用的编译器和标志
CC ?= afl-gcc          # 默认使用afl-gcc进行编译，如已安装AFL，可将CC设为afl-clang-fast等
CFLAGS = -g -O2        # 生成调试信息(-g)，优化级别O2。AFL建议不要使用-O3过高优化。
# 若需要AddressSanitizer，可添加: CFLAGS += -fsanitize=address
LDLIBS =                # 如需链接特殊库，在此添加

# 列出所有模糊测试目标程序
TARGETS = fuzz_strlen fuzz_strcpy fuzz_strcat fuzz_strcmp \
          fuzz_atoi fuzz_abs fuzz_div \
          fuzz_memcpy fuzz_memmove fuzz_free fuzz_file

# 默认目标：编译所有测试程序
all: $(TARGETS)

# 通过模式规则编译 .c 源文件
fuzz_%: src/fuzz_%.c src/common.h
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

# 清理目标
clean:
	rm -f $(TARGETS)