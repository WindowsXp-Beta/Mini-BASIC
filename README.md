# A minimal BASIC interpreter
> Project of SE-2322.
>
> Created by Qt creator.

## 题目解读
> 若有歧义，以Basic-doc.pdf为准

### 输入
1. 每条指令前的数字确定了指令的执行顺序。
2. 输入同样的数字将替换这条指令。
3. 输入一个数字而不跟任何东西将删除这行。
4. 必须按照行号显示已经输入的代码。

### 表达式
1. 需要实现的运算：`+ - * / ()`，对于至少32位的有符号整数。
   > addtionally: 实现乘方`**`，优先级大于`* /`
2. 对于所有的表达式和声明，你需要能够处理**多余的空格**。
3. LET PRINT INPUT 可以被直接执行，不需要输入行号。
4. GOTO IF REM END 必须要输入行号才可执行。

表达式种类：
1. 常亮表达式。constant expression
2. 变量。identifier expression
3. 复合表达式。1 `E op E`  2 `(E)`

|指令|意义|
|:-:|:-:|
|REM|注释|
|GOTO n|转到第n行，若无n，报错|
|IF `exp1 op exp2` THEN n|op包括`> < =` 若正确跳转到第n行|
|INPUT var||
|LET `variablename = exp`|赋值|
|PRINT `exp`|打印exp的值和一个换行符|

### button
不能作为程序一部分的指令
1. RUN 从最小的数字开始执行程序
2. LOAD 加载一个文件。将文件展示出来并执行就好像它们是从控制台中输入的一样。
3. CLEAR 清空，让user可以开始一个新的。
4. HELP 提供一个简单的帮助信息。
5. QUIT 退出BASIC解释器。

整体逻辑：
如果命令行输入：有行号 -> 加入代码
无行号 -> 直接执行
点完run
逐行parse 并用一个`QMap<int line_number, statement>`保存程序
