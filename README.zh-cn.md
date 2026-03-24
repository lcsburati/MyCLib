# MyCLib

[English Version](README.md) | [Versão em Português](README.pt-br.md)

## 重要提示 (Important Notice)

> **本 README 由 AI 生成，因我仍在学习普通话，尚未达到能够独立撰写的能力。内容已经过我的审核和批准。**
>
> 本项目仅用于学习目的。
>
> `src/` 和 `include/` 中的所有文件均由**手动编写，未使用 AI 辅助**，**唯一的例外是 `ui.h`、`ui.c` 和 `main.c`**，它们是使用 AI 代理创建的，旨在提供一个视觉界面来演示后端功能。

---

**目标：** 展示我对 C 语言的了解及其特性。

这个项目是我自己实现的库和算法集合，旨在理解底层的工作原理。

你可以使用 **CMake** 编译项目：

```bash
mkdir build && cd build
cmake ..
make
./main
```

## 函数

### 字符串工具 (String Utils)

*   **my_strlen**: 使用指针运算返回字符串的长度。
*   **my_tolower**: 将字符串转换为小写（返回新分配的字符串）。
*   **my_toupper**: 将字符串转换为大写（返回新分配的字符串）。
*   **my_strcmp**: 字典序比较两个字符串。如果相等返回 `0`，如果 str1 较大返回 `1`，如果 str2 较大返回 `-1`。
*   **my_strcmp_percent**: 使用 Levenshtein 距离算法计算字符串相似度。返回百分比 (0-100)。

### 数学工具 (Math Utils)

*   **min**: 返回三个整数中的最小值（用于 `my_strcmp_percent` 的动态规划）。
