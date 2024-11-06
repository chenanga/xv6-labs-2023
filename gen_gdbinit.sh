#!/bin/bash
exit

# 为了解决vscode gdb调用gdbinit时候路径错误的问题
workspace=$(pwd)  # 获取当前工作目录的绝对路径

echo "set confirm off" > .gdbinit
echo "set architecture riscv:rv64" >> .gdbinit
# echo "@REM target remote 127.0.0.1:26000" >> .gdbinit
echo "symbol-file $workspace/kernel/kernel" >> .gdbinit
echo "set disassemble-next-line auto" >> .gdbinit

for file in user/_*; do
    echo "add-symbol-file $workspace/$file 0x00000000" >> .gdbinit
done

