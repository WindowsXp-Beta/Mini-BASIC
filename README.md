# A minimal BASIC interpreter
> Project of SE-2322.
>
> Created by Qt creator.

## 一些不太优雅的地方

> 给自己和后来人立个flag
> 有空就改.jpg
> 想起来就改.jpg

- [ ] 使用QString的格式化函数arg，而不是一些加法之类的东西。 
- [ ] 无法解析 `- variable_name` 这样的格式。
- [ ] INPUT仅仅打印错误，实际上还是会给变量赋值。比如num会赋0，string会赋空。这样之后其他命令还可以调用这个变量。
- [ ] 写一个utilty头文件存所有的判断函数。
- [x] <del>用一个token类来进行QString的读写，而不是在每个命令中遍历数组</del>。改啦

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

INPUT逻辑：execute调一个mainwindow的函数 => 这个函数在cmdlineedit里打印`?`并发信号让execute的程序停下来（进入一个qeventloop循环） => on_return_pressed => 通过信号把读到的参数传出来并且让execute退出qeventloop循环（一个信号绑两个slot） => execute接受这个信号设置参数

### 语法树

你应该用中缀表示法建立一棵语法树，你不必真的画一棵树，你只需要使用缩进来展示一棵树。

注意：相同层的叶子节点在缩进表示法中处于同一列。

>  一些具体事例参考Basic-doc.pdf

### 错误处理
#### parse
1. 非法指令。
2. 变量名不能是关键字。
3. parseExp时，exp不完整，比如少了数字，括号之类的。
4. 行号必须处于 $[0,1000000]$ 间。
5. IF THEN语句缺少元素。
6. 语句元素过多。（比如画蛇添足）
#### execute
expression
1. 除0。
2. 变量未定义。
3. 运算符未定义。

statement
1. INPUT输入非整数。
2. GOTO 到了一个不存在的行。或者GOTO到一个非整数。
## Qt philosophy

### 信号-槽机制

1. https://www.jianshu.com/p/d6fc0bb0689a
2. https://blog.csdn.net/zkl99999/article/details/53787044

> **信号就是一个个的函数名，返回值是 void（因为无法获得信号的返回值，所以也就无需返回任何值），参数是该类需要让外界知道的数据。信号作为函数名，不需要在 cpp 函数中添加任何实现。**

关于信号的参数，这个参数可以被槽函数接受。

所以两者声明必须满足一定条件。

> 如果信号的参数有默认值，那么传递给`SIGNAL()`宏的参数个数不能少于`SLOT()`宏的参数个数。
>
> 信号槽要求信号和槽的参数一致，所谓一致，是参数类型一致。如果不一致，允许的情况是，槽函数的参数可以比信号的少，即便如此，槽函数存在的那些 ，参数的顺序也必须和信号的前面几个一致起来。这是因为，你可以在槽函数中选择忽略信号传来的数据（也就是槽函数的参数比信号的少），但是不能说信号根本没有这个数据，你就要在槽函数中使用（就是槽函数的参数比信号的多，这是不允许的）。

- **发送者和接收者都需要是QObject的子类（当然，槽函数是全局函数、Lambda 表达式等无需接收者的时候除外）；**

  > 这意味着你自己写的类必须继承QObject

- **使用 signals 标记信号函数，信号是一个函数声明，返回 void，不需要实现函数代码；**

- **槽函数是普通的成员函数，作为成员函数，会受到 public、private、protected 的影响；**

  > 如果信号是 private 的，这个信号就不能在类的外面连接，也就没有任何意义。

- **使用 emit 在恰当的位置发送信号；**

- **使用QObject::connect()函数连接信号和槽。**

- **任何成员函数、static 函数、全局函数和 Lambda 表达式都可以作为槽函数**

### 事件循环

类：`QEventLoop`

方法：`exec()`来启动一个事件循环，在这个循环期间，可以调用`exit()`来强制使`exct()`返回。

**子层事件循环具有父层事件循环的所有功能，所以当在主线程中启动各种exec()（比如QEventLoop::exec()）时，虽然会打断main函数中的QApplication::exec()，但是Gui界面还是可以正常响应，不会出现卡住的现象。这与用while来循环是不一样的。**

使用信号槽机制

### 自己写的类继承 QObject

Golden Rules

1. Make sure the `Q_OBJECT` macro is present in the definition of all `QObject`-derived classes.

2. Make sure you declare your `QObject`-derived classes in your header files **only**.

3. Make sure all of your header files are listed in your .pro file in the `HEADERS=` list.

4. Run `qmake` every time you add `Q_OBJECT` to one of your classes or modify your `.pro` file.

   > In Qt creator, run qmake by `right click` the project then select `run qmake`

## v2.0

### 字符串变量处理