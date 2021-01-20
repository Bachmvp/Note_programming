# C++ primer section5

## 语句

#### default标签

 如果没有任何一个case标签能匹配上switch表达式的值，程序将执行紧跟在default标签后面的语句，例如：

```c++
unsigned cnt = 0, other = 0;
char ch;
while(cin >> ch)
{
  switch(ch)
  {
      case 'a':
      case 'e':
      case 'i':
      case 'o':
      case 'u': //几个case写在一起，中间没有break，因此，只要ch是其中一个，cnt都会++
          ++cnt;
          break;
      default:
          ++other;
          break;
  }
}
```



#### 范围for语句

我们之前也写过，这里举个例子吧（感觉这个新式的for语句也是比原来更偷懒的方法，不过它只能遍历所有的，当然遍历所有比较常用，所以C++设计者搞了这个语法）

```c++
//把vector的所有元素翻倍
vector<int> = {0, 1, 2, 3};
for(auto &r : v) //别忘了这里是引用哦，没有引用的就不会改原来容器中的值了
{
    r *= 2;
}
```

#### do while语句

 **do while和while的唯一区别是：do while语句先执行循环体后检查条件，不管条件如何，我们都至少执行一次循环对吧。** 值得注意的是，do while先执行语句后判断条件，所以**不允许在条件部分定义变量**。



### try语句块和异常处理

异常是指存在于**运行时的反常行为**，而且这些行为**超出了函数正常功能的范围**，就是说函数无法处理了，例如失去数据库连接以及遇到意外输入等。 C++有一套异常处理机制：

1. **throw表达式**，用于异常检测，表示程序遇到了无法处理的问题，throw就是**抛出异常** 
2. **try语句块**，用于异常处理，try语句块以关键字try开始，并以一个或多个catch子句结束。也就是说，try语句块中代码抛出的异常会被某个catch子句处理。 
3. **一套异常类（exception class）**，用于在throw和catch语句之间传递异常的具体信息

#### throw表达式

 我用原书的代码，检查两条数据是不是同一本书

```c++
if(item1.isbn() != item2.isbn())
{
 throw runtime_error("不是同一种书");
}
cout << item1 + item2;
```

当然你也可以用cout输出，但是在项目程序中，**对象相加的代码和用户交互的代码应该要分离开**，我们上面的代码就做到了这一点，一旦抛出异常后，**程序就把控制权转移给能处理该异常的代码**。 *类型runtime_error是标准库异常类型的一种*

#### try语句块

 接着上面的代码处理：

```c++
while(cin >> item1 >> item2)
{
 try //执行相加，不行抛异常
 {
     if(item1.isbn() != item2.isbn())
     {
         throw runtime_error("不是同一种书");
     }
     cout << item1 + item2;
 }
 catch(runtime_error err)
 {
     cout << err.what() << "再试一次？请输入yes或者 no" << endl;
     //what是库里的一个函数
     string str;
     cin >> str;
     if(!cin || str == "no") //如果没输入或者输入no，就跳出循环，结束程序
     {
         break;
     }
 }
}
```

如果没有找到匹配的**catch**子句，程序会转到**terminat**库函数，导致程序非正常退出。