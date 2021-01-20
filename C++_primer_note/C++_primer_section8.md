# C++ primer section8

## C++ 标准库

随着C++版本的一次次修订，标准库也越来越强大，这部分我们要介绍标准库中一些大家都应该知道的比较常用的东西。
到底什么叫标准库呢？**其实标准库的核心就是很多容器类和一族泛型算法（数据结构加算法）**，这些东西能帮助我们编写简洁高效的程序，我们只要把精力放在求解的问题上就好了。

### IO库

C++是通过标准库中的IO库来处理输入输出的，本章就是来介绍这个IO的，之前也学了一些，就是**cin**之类的。接下来就系统地学一下，彻底搞定这个IO。
IO就是**Input和Output。**

不要认为这一章很简单，它里面有很多东西要学的，我开始看的时候觉得这一章没啥，现在看来还是要好好理解下的：

我们先来看看我们已经学到过的一些IO库设施：

- **istream输入流类，提供输入操作** 
- **cin, istream对象，从标准输入读取数据** 
- **运算符>>，用来从istream对象中读取输入数据** 
- **getline函数，从一个给定的istream读取一行数据，存入一个给定的string对象中** 

------

- **ostream输出流类型，提供输出操作** 
- **cout，ostream对象，向标准输出写入数据** 
- **cerr，ostream对象，用于输出程序错误信息，写入到标准错误** 
- **运算符<<，用来向一个ostream对象写入数据**

**这里讲的都是基于控制台的IO，只不过之前总结的时候没加上cout和cin,cerr**

#### IO类

为了支持不同种类的IO处理操作，除了iostream之外，C++还定义了两个类，我们索性三个一起说了：

1. **iostream定义了用于读写流的基本类型** 

2. **fstream定义了读写命名文件的类型** 

3. **sstream定义了读写内存string对象的类型**

   ```
   graph LR
   iostream-->istream-ostream
   ```

   ```
   graph LR
   fstream-->ifstream-ofstream
   ```

   ```
   graph LR
   sstream-->istringstream-ostringstream
   ```

   **ifstream和istringstream都继承自istream**，先不用管继承什么意思，只要记住我们怎么用istream，就怎么用其他两个。

##### IO对象不能拷贝或赋值

**不能拷贝其实也就是不能赋值的，之前我们总说的拷贝初始化和赋值初始化其实很相似**

这是规定，所以我们一般都用引用，来几个错误例子示范：

```c++
ofstream o1, o2;
o1 = o2; //错误：不能赋值

ofstream print(ofstream); //错误：这里是一个print声明，
//后一个ofstream会被初始化，就是拷贝，不行
//前一个是返回类型，返回的时候也会拷贝，不行
```

记住两个不能：

1. **不能拷贝IO对象，所以不能把它们用作形参或返回类型，一般我们就用引用** 
2. **读写IO对象会改变它的状态，所以它的引用不能是const的 总之，只能是通过普通引用的方式来使用它。**

##### 条件状态

IO操作一个与生俱来的问题就是错误，因为你的用户不会那么听话。IO类定义了一些函数和标志来帮我们处理这个问题。我们慢慢介绍（原书在P279-P280）
来个错误示范：

```c++
int a;
cin >> a;
```

这个代码是没问题的，但是，用户没有输入数字，而是输入了字母。**这样cin就会进入错误状态**，一旦一个流发生错误，后续的所有Io操作都会失败，所以啊，代码通常应该在使用一个流之前检查它是否处于良好状态，比较简单的做法是把它当作条件：

```c++
while(cin >> a)
{
    ...//读操作成功后再执行
}
```

**可以看到，标准库的函数都是很高级的，如果错误他会自动返回错误**

**查询流的状态**

我们前面讲流作为条件使用，只能告诉我们一个结果，无法告诉我们具体发生了什么。

IO库定义了一个与机器无关的**iostate**类型，它提供了表达流状态的完整功能。具体的用到再查把，只要记住它有一个iostate来表示遇到的问题。

**管理条件状态**

来一段代码看看如何管理条件状态

```c++
auto old _state = cin.rdstate(); //调用rdstate记住cin的当前状态
cin.clear(); //调用clear函数使cin有效
process_input(cin); //使用cin
cin.setstate(old_state); //将cin置为原有状态
```

**clear()会清除所有错误标志位（复位）**。

clear还有一个接受一个参数的版本，下面的代码只复位failbit和badbit：

```c++
cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit); //括号内为位运算
```

**流的一些相关函数平时用的真的不多**

##### 管理输出缓冲

**每个输出流都管理一个缓冲区**，用来保存程序读写的数据，例如：

```c++
os << "sss";
```

文本串可能立即打印，也可能***作系统保存在缓冲区中随后再打印。有了这么一个**缓冲机制**，操作系统就可以将程序的多个输出操作组合成单一的系统级写操作。

**导致缓冲刷新**（就是数据真正写到输出设备或文件中）的原因有很多：

- **程序正常结束，作为main函数的return操作的一部分，缓冲刷新被执行** 
- 缓冲区满了，所以要刷新缓冲，以便后来的数据能继续写入缓冲 
- 使用例如**endl**的操作符来显式刷新缓冲区 
- 在每个输出操作后，我们可以用unitbuf设置流的内部状态来清空缓冲区。对于cerr来说，unitbuf是默认设置的，因此写到cerr的内容都是立即刷新的 
- 一个输出流关联到另一个流时，当读写被关联的流时，关联到的流的缓冲区自动刷新。例如，默认情况下，cin和cerr都关联到cout，因此读cin或者写cerr都会导致cout的缓冲区被刷新

**只有输出流才有缓冲区**

**刷新输出缓冲区**

有三个显式刷新的符：

```c++
cout << "你大爷" << endl; //输出你大爷和换行，然后刷新缓冲区
cout << "你大妈" << flush; //输出你大妈，然后刷新缓冲区
cout << "你大伯" << ends; //输出你大伯和一个空字符，然后刷新缓冲区
```

**神器一直刷新缓冲区-unitbuf操作符**

看代码就懂系列：

```c++
cout << unitbuf;
//接下来的所有输出都立即刷新，无缓冲
cout << nounitbuf //恢复正常的缓冲方式
```

**关联输入和输出流**

```c++
cin >> a;
```

这句话会刷新缓冲区，为啥呢，前面说过，**标准库将cout和cin关联起来**，**所以从cin中读取数据会刷新cout缓冲区**。

这是系统设置的关联流，下面介绍一下我们如何自己设置关联流，要借助一个函数tie。

```c++
ostream *old_tie = cin.tie(nullptr);
//原来cin与cout是默认关联，现在cin不再与其他流关联

cin.tie(&cerr); //cin现在与cerr关联，读取cin会刷新cerr而不是cout了

cin.tie(old_tie); //恢复常态
```

**每个流同时最多关联到一个流，但一个ostream可以同时被多个流关联**。

**可以看到流的骚操作还是很多的，如果合理应用还是很有效率的**

## 文件输入输出

头文件fstream定义了三个类型来支持文件IO：

1. **ifstream：从一个给定文件读取数据** 
2. **ofstream：向一个给定文件写入数据** 
3. **fstream：读写给定文件**
   因为fstream是继承自iostream的，所以它拥有iostream所有的行为，而且它还定义了一些新的成员来管理与流关联的文件。下面我们会详细介绍。

#### 使用文件流对象

当我们想要读写一个文件时，可以定义一个文件流对象，把对象和文件关联起来：

```c++
ifstream in(ifile); //定义输入流in，初始化它为从文件中读取数据，文件名是ifile
```

##### 用fstream代替iostream&

我们来用原来书店老板那个Sales_data类试试fstream：

```c++
ifstream input(record); //打开销售记录文件，关联到input
ofstream output(out); //打开输出文件

Sales_data total; //保存销售额总量
if(read(input, total)) //读取第一条销售记录,基类可以接受继承类，我们说过后两个io都是继承他的，这就是多态
{
    Sales_data trans; //保存下一条
    while(read(input, trans))
    {
        if(total.isbn() == trans.isbn())
        {
            total.combine(trans); //同一条，加上去
        }
        else
        {
            print(output, total) << endl; //不同的，输出前一条，刷新缓存
            total = trans; //准备搞下一条
        }
    }
    print(output, total) << endl; //打印最后一条
}
else
{
    cerr << "没数据啊老板" << endl;
}
```

**大家是不是觉得这段程序跟之前那段非常像，我想告诉的大家的就是，我们用ftream可以像用iostream一样，非常方便。**

##### 成员函数open和close

```c++
string ifile = "f";
ifstream in(ifile); //构建一个ifstream并打开f文件
ofstream out;
out.open(ifile + "1"); //打开指定文件f1，out与f1关联
if(out) //如果打开成功（文件不能被连续打开）
{
    in.close(); //关闭文件f
    in.open(ifile + "2"); //打开f2文件
}
else
{
    cerr << "文件不存在" << endl;
}
```

##### 自动构造和析构

**当文件流离开它的作用域时，它就自动被销毁了，与它关联的文件也会自动关闭。**

#### 文件模式

每个流都有一个关联的文件模式，用来指出如何使用文件：

| 常量             | 含义                     |
| ---------------- | ------------------------ |
| ios_base::in     | 打开文件，以便读取       |
| ios_base::out    | 打开文件，以便写入       |
| ios_base::ate    | 打开文件，并移到文件尾   |
| ios_base::app    | 追加到文件尾             |
| ios_base::trunc  | 如果文件存在，则截短文件 |
| ios_base::binary | 二进制文件               |

每个文件流类型都定义了一个默认的**文件模式**，**且一个文件流类型可以有多个文件模式**，例如：

| 文件流类型 | 文件模式 |
| ---------- | -------- |
| ifstream   | in       |
| ofstream   | out      |
| fstream    | in和out  |

##### 以out模式打开文件会丢弃已有数据

**阻止一个ofstream清空给定文件内容的唯一方法是显示指定app或in模式：**

```c++
ofstream app("file1", ofstream::app); 
ofstream app2("file1", ofstream::out | ofstream::app);
//这样就不会被清空了，定位到文件末尾了嘛
```

##### 每次调用open时都会确定文件模式

对于一个给定流，每次打开文件，都可以指定其文件模式：

```c++
ofstream out; //未指定
out.open("f"); //默认为out和trunc
//接下来改变它的模式
out.close(); //先关闭
out.open("f", ofstream::app); 模式为out和app
```

 **每次打开文件，都要设置文件模式，未指定时就使用默认值，你都要清楚的。**

**文件模式很重要啊，因为写log文件是很重要的！！！！**

## string流

| 类型          | 作用               |
| :------------ | :----------------- |
| istringstream | 从string中读取数据 |
| ostringstream | 向string写入数据   |
| stringstream  | 都行               |

#### 使用istringstream

这次我来个***：整理通讯录

事情是这样的，我们有一个通讯录，列出了人名和手机号码，某些人的手机号码可能有多个，大概像下面这个样子

- 石破天 13525684953 

- 石中玉 13624586352 15632459865 我们先定义一个简单的类来描述输入数据：

  ```c++
  struct PersonInfo
  {
    string name;
    vector<string> phones;
  };
  ```

  我们的程序会读取数据文件，并创建一个PersonInfo的vector，在一个循环中处理输入数据，每个循环步读取一条记录，提取出一个人名和若干电话号码：

  ```c++
  string line, word;
  vector<PersonInfo> people;
  while(getline(cin, line))
  {
    PersonInfo info;
    istringstream record(line); //将记录绑定到刚读入的行
    record >> info.name; //读取名字
    while(record >> word) //读取这个人所有电话号码
    {
        info.phones.push_back(word);
    }
    people.push_back(info); //把这个人的信息装进通讯录
  }
  ```

  其实就是你的输入肯定是 abc 1111111 22222 33333 这个第一个人名给了name，后面就循环push进vector了

#### 使用ostringstream

好的，现在我们把刚刚构建好的通讯录输出，因为我们不希望输出号码有错误的人，所以啊，对于每一个人来说，我们要验证他所有的号码都有效才可以输出，于是，很自然的，我们就想到先把输出内容写入到一个内存ostringstream中：

```c++
for(const auto &entry : people) //遍历
{
    ostingstream goodNums, badNums; //每步循环创建对象
    for(const auto &nums : entry.phones)
    {
        if(!valid(nums)) //如果号码不合法（我们假定有这个valid函数）
        {
            badNums << " " << nums;
        }
        else
        {
            goodNums << " " << nums;
        }
    }
    if(badNums.str().empty()) //全对，没有错误号码
    {
        os << entry.name << " " << goodNums.str() << endl; 
    }
    else
    {
        cerr << "有错误号码" << entry.name << badNums.str() << endl;
    }
}
```

这样，我们的通讯录功能就完成了。

**这里也很简单，就是都扔到输出流里，空格对流来说很重要，不然没法区分啊**