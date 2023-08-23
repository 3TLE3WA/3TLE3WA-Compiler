# C2RV Compiler by 3TLE3WA

勿 cue，我也想知道为什么性能不如 current-best。

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

# TODO

## 中端

- [ ] 循环加法转乘法
- [x] 全局变量局部化
- [ ] 全局变量常量化，在第一次可能被修改前
- [ ] 尾递归以及可尾递归化的函数进行 inline，转换成循环
  - 目前后端使用 tail 指令的思想，使用 j 指令构成了循环，但是不能进一步对循环优化
- [ ] 利用乘法分配律，将多个同系数乘法转换成加法和一个乘法，除法涉及精度问题，不需要做
- [ ] 利用交换律，将同类型，同编号变量靠近，以在未来使用常量折叠和加法合并
- [ ] 布隆过滤器
- [ ] 浮点数 2 的幂次除法转乘法
  - 流程比较长，详细见下方说明
- [ ] 过大的局部变量转换为全局变量，注意，不要将需要重复生成的局部变量转换为全局变量，会出错
- [ ] 浮点数乘 2 转加法
- [ ] 位运算优化，将符合位运算的控制流模式转换为位运算
- [ ] 合并浮点数操作
  - [ ] fabs
  - [x] fneg
  - [ ] fmax, fmin
- [ ] rotl + rotr
- [x] 指令融合
- [ ] 除以浮点数转乘以其倒数（不一定有精度问题，需测试）
- [x] 解 phi
- [x] 移除多余的跳转
- [x] 合并可合并的基本块（为后期指令调度提供更大空间，同时减少跳转次数）
- [x] load 的 lvn
- [x] gep 的 gvn

## 后端

- [x] 窥孔优化
  - [x] 合并 `b` `icmp` （前端给的 use 有毒）
  - [x] 合并 `gep` 和 `load` 
  - [ ] ~~合并出 `fmadd`, `fnmadd`, `fmsub`, `fnmsub`~~（有精度问题）
  - [ ] 删除无用操作
    - [x] load 后不使用直接 store
    - [x] 空操作
    - [x] 获取 0 的操作换成 zero 寄存器
    - [x] 寄存器 + 偏移量寻址
    - [ ] 删除 fallthrough 的 jmp
- [ ] 贪心寄存器分配算法
  - [x] 按照权重选取寄存器
  - [ ] split 操作
  - [ ] 释放所有的寄存器，也即不再有特殊用处的寄存器，转换成迭代的算法
  - [x] 函数参数和返回值优先占据参数寄存器
- [ ] 指令调度
  - [x] 汇编指令调度
  - [ ] 解决内存别名问题
- [x] 关于 2 幂次的整数乘除模运算优化
- [x] 常用栈上变量按靠近 fp 和 sp 方式分布和计算
  - 提供了 4096 的栈范围，但是不连续
- [x] 关于非 2 幂次的整数除法优化，使用 hacker's delight 书中算法求解
- [x] gp 优化全局变量寻址
  - 由 linker 自动解决
- [x] 线程库设计

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
