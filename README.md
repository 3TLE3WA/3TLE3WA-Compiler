# C2RV Compiler by 3TLE3WA

# 整体架构说明

```
          +-----------+
          | sysy file |
          +-----------+
                |
                v
  +------------------------------+
  | antlr4 pre genernated parser |
  +------------------------------+
                |
                |
                v
      +----------------------+
      | frontend gen llvm ir |
      +----------------------+
                |
                |
                v
        +-----------------+
        | pass on llvm ir | <--- cfg node now using shared_ptr and have many copy/destroy
        +-----------------+      while optimizing
                |                this consumes most of compile time
                |
                v
      +---------------------+
      | backend distributer | <--- the backend only take around 0.8% of compile time
      +---------------------+      no need for parallelization
          |     |     |  
          |     |     | <--- compile tasks allocated to multi-threads if parallel issued
          |     |     |      serialize issue or parallel issue
          |    ...   ...
          |
          v
  +-------------------+
  | risc-lang ir      |
  | just Uop and VReg |  <-- schedule on ir, assign register, plan stack map
  +-------------------+
            |
            |
            |
            v            +--------------+
      +-------------+    | other thread |
      |  riscv asm  |    +--------------+
      +-------------+           v
            |                   v
            |                   |                  +--------------+
            +-------------------+------------- <<< | other thread |
                                |                  +--------------+
                                |
                                | <--- do trick on generation
                                |
                                v
                +---------------------------------+
                | asm optimization and generation |
                +---------------------------------+
                                |
                                | <--- schedule on asm and reduce redundant codes
                                v
                          +----------+
                          | asm file |
                          +----------+
```

# Makefile 说明

> 注意，本项目的 Makefile 可能随时变动

| target       | 目的                                                              | 参数和说明                                                             |
| ------------ | ----------------------------------------------------------------- | ---------------------------------------------------------------------- |
| `pyll`       | 使用 `runtest.py` 测试 llvm ir 正确性                             | 无                                                                     |
| `pyasm`      | 使用 `runtest.py` 测试 riscv asm 正确性                           | 无                                                                     |
| `release`    | 编译出 release 版本的编译器，一般会定义 NDEBUG                    | 无                                                                     |
| `debug`      | 编译出 debug 版本的编译器，用于调试                               | 无                                                                     |
| `build`      | 按照依赖编译编译器                                                | 无                                                                     |
| `run`        | 单测                                                              | `DEMO` 选择测例，$(DEMO)*.sy；`SMODE` 单测模式，可选 `MODE` 里的内容； |
| `ll` `rv`    | 分别以生成 llvm ir 和 riscv asm 为目的进行单测，可用 `run` 的参数 | 无                                                                     |
| `qemu-dbg`   | 开启 qemu 调试，目前在端口 1234                                   | 无                                                                     |
| `pys`        | `runtest.py` 单独测试 asm                                         | 无                                                                     |
| `diff`       | 对比 `runtest.py` 单独测试最终 res 和正确输出                     | 无                                                                     |
| `perf`       | 测试编译用时情况                                                  | 无                                                                     |
| `clean*`     | 各种清理                                                          | 无                                                                     |
| `format-all` | 用 clang-format 整理所有文件格式                                  | 无                                                                     |
| `all`        | 旧 shell 脚本测试                                                 | 无                                                                     |
| `cp`         | 模拟测评机逻辑，测试是否能够编译                                  | 无                                                                     |

# CMake VSCode Json Setting

``` json
// .vscode/settings.json
{
  "cmake.debugConfig": {
    "args": [
      "-S", "-o", "main.s", "-d", "main.ir.s", "-l", "main.ll", "../main.sy"
    ]
  }
}
```

# RV ToolChain

手动编译建议编译用编译 riscv64-linux 的 gcc。也是目前各种 Linux 发行版会使用的交叉编译器。使用该版本是因为其和 qemu 配合比较方便，可以方便测试浮点数。

如果使用 spike + pk 和 riscv64-elf 的 gcc，可能会在浮点数方面遇上难题。但不得不说，spike 对于调试前期的整数程序帮助很大。

``` bash
./configure --prefix="$RISCV" --with-arch=rv64gc --with-abi=lp64d
```

# Antlr 建议

高于 4.12 版本，使用包管理器下载。
