# 基于AFL的白盒模糊测试

## 项目概述

本项目旨在演示如何针对多个标准C库函数进行**白盒模糊测试**（Fuzzing），使用C语言编写测试程序并集成 AFL（American Fuzzy Lop） 进行模糊测试。项目以 Linux x86_64 平台为目标，包含完整的代码、测试用例、构建脚本和文档，以教学友好的方式展示模糊测试的流程。通过本项目，可以学习：

- 如何构建针对标准库函数的模糊测试**驱动程序**（Harness）。
- 如何使用 AFL 工具进行模糊测试，包括输入生成、覆盖率检测和崩溃检测等机制。
- 如何设计测试用例涵盖函数的边界条件、无效输入以及缓冲区溢出等潜在问题。
- 完整的项目结构和 Makefile，让读者可以实际编译并运行 AFL 进行测试。
- 代码和文档中提供了详细的中文注释，便于教学和理解。

## 项目结构

项目采用层次清晰的目录结构，源代码、测试用例、构建脚本和文档分别放置在不同目录中。下面是项目的目录和文件结构：

```
cppCopyC_Fuzz_Project/                   // 项目根目录
├── README.md                    // 使用说明文档（中文）
├── Makefile                     // 构建脚本，编译所有测试程序
├── src/                         // 源码目录，包含各模糊测试驱动程序
│   ├── common.h                 // 公共头文件（包含常用引用和辅助函数声明）
│   ├── fuzz_strlen.c            // 针对 strlen 函数的测试驱动
│   ├── fuzz_strcpy.c            // 针对 strcpy 函数的测试驱动
│   ├── fuzz_strcat.c            // 针对 strcat 函数的测试驱动
│   ├── fuzz_strcmp.c            // 针对 strcmp 函数的测试驱动
│   ├── fuzz_atoi.c              // 针对 atoi 函数的测试驱动
│   ├── fuzz_abs.c               // 针对 abs 函数的测试驱动
│   ├── fuzz_div.c               // 针对除法操作的测试驱动 (测试除零)
│   ├── fuzz_memcpy.c            // 针对 memcpy 函数的测试驱动
│   ├── fuzz_memmove.c           // 针对 memmove 函数的测试驱动
│   ├── fuzz_free.c              // 针对 malloc/free 函数使用的测试驱动 (测试重释漏洞)
│   └── fuzz_file.c              // 针对文件操作函数的测试驱动 (如 fopen/fgets)
├── testcases/                   // 初始测试用例目录（AFL将从这里读取初始输入）
│   ├── strings/                 // 字符串函数相关的初始测试用例
│   │   ├── empty.txt            // 空输入测试
│   │   ├── small.txt            // 短字符串测试
│   │   └── long.txt             // 较长字符串测试
│   ├── math/                    // 数学函数相关的初始测试用例
│   │   ├── zero.txt             // 包含0的输入
│   │   ├── maxint.txt           // 非常大的数字输入
│   │   └── negative.txt         // 负数输入
│   ├── memory/                  // 内存操作相关的初始测试用例
│   │   ├── pattern1.txt         // 特定字节模式输入
│   │   └── pattern2.txt         // 另一种字节模式输入
│   └── file/                    // 文件操作相关的初始测试用例
│       ├── basic.txt            // 基本文件名输入
│       └── special.txt          // 特殊字符文件名输入
└── docs/
    └── usage.md                 // 可选的详细使用文档（也可以合并在README.md）
```

**说明：**

- **src/** 目录下每个 `fuzz_*.c` 文件都是一个独立的测试程序（模糊测试**驱动**），用于测试特定的标准库函数或操作。每个文件都包含一个 `main` 函数，使用 `stdin` 或文件输入（`@@`）从 AFL 获取数据，然后对目标函数进行调用测试。代码内含丰富的中文注释解释工作原理和边界情况处理。
- **common.h** 提供一些公共的包含（如 `<stdio.h>`、`<string.h>` 等标准库）、宏定义或辅助函数声明，方便在多个测试文件中复用。
- **testcases/** 目录提供AFL所需的初始输入种子，每个子目录对应一类测试（字符串、数学、内存、文件），里面放置一些代表性输入文件。AFL将基于这些初始种子进行变异（mutation）来生成新的测试输入。
- **Makefile** 定义了编译每个测试程序的规则，以及 `make all` 构建所有测试程序，`make clean` 清理可执行文件等。Makefile 默认使用 AFL 提供的编译器（如 `afl-gcc` 或 `afl-clang`）进行编译，以插桩代码获取覆盖率信息。
- **README.md**（即本说明文档）详细说明了项目的使用方法，包括如何构建、如何运行 AFL 进行模糊测试，以及如何查看结果和发现的崩溃。

## 环境准备

1. **安装 AFL**：确保已经安装 AFL 工具。例如在Ubuntu上可以通过`sudo apt-get install afl`获取，或者从[AFL官方仓库](https://github.com/google/AFL)编译安装。安装后应有 `afl-fuzz` 可执行文件，以及 `afl-gcc`/`afl-clang-fast` 等编译器脚本。

2. 编译测试程序

   ：进入项目根目录，执行 

   ```
   make CC=afl-gcc
   ```

   。这会使用 afl-gcc 编译所有目标。如果想使用 clang 版本，可以 

   ```
   make CC=afl-clang-fast
   ```

   . 编译完成后，当前目录下会出现多个 

   ```
   fuzz_*
   ```

    可执行文件。

   - 可选：为了捕获更多内存错误，您也可以在编译时启用AddressSanitizer：`make CFLAGS="-g -O2 -fsanitize=address"`. AFL支持与ASan联用，但注意不要与栈映射（-fsanitize=address与afl-gcc默认的栈随机有冲突，AFL会自动处理）。使用ASan会减慢执行速度，但能检测出例如缓冲区溢出的位置。

3. **准备初始测试用例**：本项目已经在 `testcases/` 目录下提供了一些初始输入文件。您可以根据需要增加更多案例。初始测试用例并不需要覆盖所有情况，只要提供不同类型的数据样本即可。AFL会基于这些样本自动生成其他测试输入。

## 使用 AFL 进行模糊测试

AFL 对每个测试目标（即我们的每个 `fuzz_*.c` 生成的程序）分别进行模糊测试。基本命令格式为：

```bash
afl-fuzz -i <初始测试用例目录> -o <输出结果目录> -- <待测程序> <程序命令行参数>
```

这里 `<程序命令行参数>` 对于我们的测试程序而言，大多数是读取stdin输入的，因此不需要参数；如果程序需要从文件读取输入，可用`@@`符号代表AFL提供的文件名。

下面针对几个测试目标给出运行示例：

- **测试 strlen**:

  ```
  afl-fuzz -i testcases/strings -o findings/strlen -- ./fuzz_strlen
  ```

  该命令让 AFL 从 `testcases/strings` 目录读取初始种子，模糊测试 `fuzz_strlen` 程序。结果将保存在 `findings/strlen` 目录下。

- **测试 strcpy**:

  ```
  afl-fuzz -i testcases/strings -o findings/strcpy -- ./fuzz_strcpy
  ```

  由于 strcpy 测试同样是针对字符串输入，我们重用 `testcases/strings` 作为种子。您也可以为它创建专门的种子集。AFL会尝试各种长度的输入，很可能发现让程序崩溃的输入（因为超过dest大小会溢出）。

- **测试 strcmp**:

  ```
  afl-fuzz -i testcases/strings -o findings/strcmp -- ./fuzz_strcmp
  ```

  strcmp 测试也使用字符串种子。AFL将探索使两半字符串相同或不同的各种情形。

- **测试 atoi**:

  ```
  afl-fuzz -i testcases/math -o findings/atoi -- ./fuzz_atoi
  ```

  初始输入可以包含数字和字符，AFL会生成各种字符串来测试 atoi 转换。

- **测试 abs**:

  ```
  afl-fuzz -i testcases/math -o findings/abs -- ./fuzz_abs
  ```

  由于 abs 测试直接读取4字节整数，提供的种子文件其实可以是任意4字节的值。`testcases/math` 下准备的文件如 `maxint.txt` 等可能包含一些边界二进制值。

- **测试除法**:

  ```
  afl-fuzz -i testcases/math -o findings/div -- ./fuzz_div
  ```

  AFL将尝试各种8字节输入以偶然产生除数为0的情况，从而触发异常。一般来说，不需要很久就会找到崩溃样本。

- **测试 memcpy**:

  ```
  afl-fuzz -i testcases/memory -o findings/memcpy -- ./fuzz_memcpy
  ```

  内存操作的输入是二进制格式，不妨提供一些随机的初始文件。AFL将寻找导致越界的参数组合。

- **测试 memmove**:

  ```
  afl-fuzz -i testcases/memory -o findings/memmove -- ./fuzz_memmove
  ```

  同上。

- **测试 free (内存释放)**:

  ```
  afl-fuzz -i testcases/memory -o findings/free -- ./fuzz_free
  ```

  这个测试程序输入仅1字节，所以初始文件可以随意。AFL会尝试翻转位元，极快就会找到使程序崩溃的flag值。如 flag=1 或 2 会导致崩溃。

- **测试 文件操作**:

  ```
  afl-fuzz -i testcases/file -o findings/file -- ./fuzz_file
  ```

  文件操作测试主要不是为了找崩溃，而是测试不同行为。AFL仍然会运行，但可能不会产生crashes（除非我们的代码有bug）。可以观察到输出目录下的queue文件，看看AFL生成了哪些特别的输入。

**运行监控**：AFL启动后，会打开一个交互界面，显示当前状态，包括执行速度、路径发现数量、崩溃和挂起次数等。您可以按 `Ctrl+C` 停止测试，AFL会安全地保存中间结果。长时间运行（数分钟到数小时）有助于找到更多边界情况。

## 查看结果和分析

AFL 将测试结果保存到 `-o` 指定的输出目录中。里面重要的子目录包括：

- `queue/`：存储所有独特路径的输入案例。AFL不断生成新输入，如果执行走到了新的代码路径，就将该输入保存下来。可以查看这些文件以了解哪些输入被认为具有新颖性。

- ```
  crashes/
  ```

  ：存储导致程序崩溃的输入文件。当程序异常退出（如段错误、断言失败、被abort等），AFL会将对应输入放入这里。

  对于本项目，我们期望一些测试会产生崩溃样本

  ，例如:

  - fuzz_strcpy 会有溢出导致的崩溃输入（通常是某个长度刚超64的字符串）。
  - fuzz_div 当除数为0时会记录一个crash。
  - fuzz_free 对应 flag=1 或 2 的输入字节值文件会在这里。
  - 其他如 fuzz_memcpy, fuzz_memmove 在参数不当时也会崩溃。

- `hangs/`：存储导致挂起（超时）的输入。如果某个输入让程序长时间卡住（比如进入死循环），AFL会把它归类为hangs。我们代码中没有明显的死循环，因此一般不会有hangs。如果出现，值得检查对应输入是否引发了极端的慢操作。

**分析崩溃样本**：拿到崩溃的输入文件后，可以用调试工具分析问题。例如，使用 gdb:

```bash
gdb --args ./fuzz_strcpy crashes/id_000000...  # 用实际崩溃文件名替换
```

然后在gdb中运行(`run`)程序，这样就能复现当时的崩溃现场，检查调用栈(`bt`命令)和变量，以了解问题所在。在我们的例子中，大多是我们故意引入的错误，例如 strcpy 溢出，double free 等。分析这些崩溃有助于理解错误的成因，以及如何修复（比如调整缓冲区大小或添加检查）。

如果使用了AddressSanitizer编译，运行崩溃样本时ASan会给出更详细的错误报告，包括哪一行发生溢出、非法释放等信息，便于教学讲解。