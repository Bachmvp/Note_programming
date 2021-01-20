# C++ primer section11

## 关联容器

前面我们学的都是顺序容器，**顺序容器中的元素是按它们在容器中的位置来保存和访问的**。接下来这一章我们要学习**关联容器：关联容器中的元素是按关键字来保存和访问的。** 
关联容器的很多行为与顺序容器相同，但那些不同之处反映了关键字的作用。

**关联容器可以处理更复杂的问题**

#### 使用关联容器

大多数程序员都很熟悉vector这一套，但很多人可能从未用过关联容器，接下来让大家看看关联容器在某些场合是多么牛逼。

##### 使用map

我们要统计每个单词在输入中出现的次数：

```c++
map<string, size_t> word_count;
string word;
while(cin >> word)
{
    ++word_count[word]; //你还能用顺序容器写出更简单的程序吗
}
```

##### 上一个程序的一个合理拓展是：忽略常见单词，如the，and等，我们可以用set来保存想忽略的单词：

```
map<string, size_t> word_count;
set<string> exclude = {"the", "and", "but"};
while(cin >> word)
{
    if(exclude.find(word) == exclude.end())
    {
        ++word_count[word];
    }
}
```

**set和map的find函数还是很好用的，如果失败会返回指向end()的迭代器，这个老是忘！！！**

#### 关联容器概述

**关联容器都支持一般的普通容器操作，但是不支持顺序容器的位置相关操作**，例如push_front，**因为关联容器中元素是根据关键字存储的。**

##### 定义关联容器

```c++
set<string> exclude = {"1", "2"};
map<string, string> authors = {
    {"a", "haha"},
    {"b", "hehe"}
}
```

##### multi差别

```c++
vector<int> ivec = {1, 1, 2, 2};
set<int> iset(ivec.cbegin(), ivec.cend());
multiset<int> miset(ivec.cbegin(), ivec.cend());
cout << iset.size() << endl; //2
cout << miset.size() << endl; //4
```

#### 关键字类型的要求

关键字就是键，值就是值，它俩一起就是**键值对**，对于键的要求就是：**键必须定义元素比较的方法**，这个要求很好理解嘛。我们前面用到的键都是**string或int之类的，元素比较的方法都已经帮我们写好了，所以我们可以很轻松地不用管**，接下来就要介绍，**如果键的类型是自定义的一些数据类型，那我们在定义比较方法的时候有哪些注意的呢**？

**必须定义一个严格弱序**(strict weak ordering)，什么意思呢，它必须具备如下性质：

- **若k1<=k2，则k2不能<=k1** （这个跟我们数学学的不一样啊） 
- **若k1<=k2, 且k2<=k3,则k1<=k3，这个好理解，传递性** 
- **若k1不小于等于k2, k2也不小于等于k1，则k1 == k2** 

接下来我们就来举个例子。定义Sales_data的multiset，显然我们不能直接这样写：

```c++
multiset<Sales_data> a; //错的，因为Sales_data没有<运算符
```

所以，我们要自定义一个比较操作，这个函数其实我们之前写过：

```c++
bool compareIsbn(const Sales_data &a, const Sales_data &b)
{
    return a.isbn() < b.isbn(); //这里能用<，还是因为isbn是string，string有<
}
```

好了，现在已经自定义好比较操作了，那我们怎么使用它，或者说怎么让编译器知道它呢，先抛出代码：

```c++
multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
```

这里和sort是一样的啊

这个定义很长，里面还有很多老朋友，我们来仔细看看（下面这段很啰嗦，但值得你仔细看看）：
要使用自定义的操作，我们在定义multiset时就要在键后面加上比较操作类型，这个比较操作类型是**函数指针**，指向我们定义的比较操作函数，这样我们就解释了前面的类型部分；后面bookstore加括号，意思是我们要调用compareIsbn来初始化bookstoe，这个怎么初始化呢？它和之前的初始化意思不太一样，它的意思是当我们添加元素时，用compareIsbn来为这些元素[排序]()（就是说，它初始化不是提供元素，是提供元素[排序]()的方式），这个**小括号里的compareIsbn类型是函数指针**，**别忘了我们在使用函数名的时候，它会自动转化为指针，那么问题来了，为什么前面那个compareIsbn还要加 *表示指针呢**？**因为decltype比较特殊啊，它得出的类型就是函数类型，所以要加* 号表示指针**。

#### pair类型

接下来介绍一个map的好基友，pair，我们会经常用到它，**它定义在头文件utility中**。 一个pair保存两个数据成员：

```c++
pair<string, string> a;
pair<string, size_t> b;
pair<string, vector<int>> c; //反正什么都能装，类似容器
```

pair的默认构造函数对数据成员进行值初始化，我们也可以提供初始化器来显式初始化：

```c++
pair<string, string> d{"张无忌", "赵敏"};
```

pair有个很特殊的规定它很大方，**它的数据成员都是public的**，**而且两个成员分别命名为first和second**，这样就方便所有人来访问，我家大门常打开。**记住没括号啊！！！**

```c++
cout << d.first << d.second << endl;
```

关于pair上的操作以后碰到了再解释，比较简单的。

#### 创建返回值为pair对象的函数

```c++
pair<string, int> process(vector<string> &v)
{
    if(v.empty()){ return {v.back(), v.back().szie(); } //列表初始化，
    else {return pair<string, int>();} //调用默认构造函数构造临时对象返回
}
```

### 关联容器操作

这部分的内容较多，不过整体还是比较简单的，只要你顺序容器那部分掌握了，这里学一下很快的，触类旁通嘛。
先介绍一些为了方便关联容器使用而定义的类型别名：

| key_type    | 此容器类型的键类型                                           |
| ----------- | ------------------------------------------------------------ |
| mapped_type | 键关联的值类型，**只适用于map**                              |
| value_type  | 对于set，与key_type相同；对于map，为pair<**const** key_type, mapper_type> |

来几个例子（我反正还没觉得这些类型别名有多好用。。。）

```c++
set<string>::value_type v1; //v1是string
set<string>::key_type v2; //v2是string
map<string, int>::value_type v3; //v3是pair<const string, int>
map<string, int>::key_type v4; //v4是string
map<string, int>::mapped_type v5; //v5是int
```

**这是啥子意思**

接下来就要介绍各种操作了：

#### 关联容器迭代器

```c++
map<string, size_t> word_count; //上一节的单词计数程序，假装这个有内容
auto map_it = word_count.begin(); //map_it作为迭代器指向首元素
cout << map_it->first; //打印键
map_it->first = "new"; //错了：关键字是const的
```

**关联容器也可以begin()**

##### set的迭代器是const的

```c++
set<int> iset = {0, 1, 2, 3};
set<int>::iterator set_it = iset.begin();
if(set_it != iset.end())
{
    *set_it = 24; //错了：键是只读的，const的
}
```

**哦，对的set的value是不能被改变的！！其实就是key不能改变！！**

##### 遍历关联容器

```c++
auto map_it = word_count.cbegin();
while(map_it != word_count.cend())
{
    cout << map_it->first << " " << map_it->second << endl;
    ++map_it;
}
```

**我们通常不对关联容器使用泛型[算法]()**，**因为键是const这一特性就限制了很多泛型[算法]()**，所以要用[算法]()也只能用那些只读的[算法]()。

#### 添加元素

```c++
vector<int> ivec = {0, 1, 0, 1};
set<int> set2;
set2.insert(ivec.cbegin(), ivec.cend()); //第一种添加元素方式set2 = {0, 1}
set2.insert( {2, 2} ); //第二种添加元素方式set2={0, 1, 2}
set2.emplace(2); //不插入，因为2已存在
```

**直接添加元素就可以了，还可以迭代器加vector???长见识**

**insert和emplace都可以，后者更好，直接构造**

##### 向map添加元素

```c++
//向word_count插入word的四种方法
word_count.insert( {word, 1} );
word_count.insert( make_pair(word, 1) );
word_count.insert( pair<string, size_t>(word, 1) );
word_count.insert( map<string, size_t>::value_type(word, 1) );
```

##### 检测insert的返回值

添加单一元素的**insert和emplace版本返回一个pair**，**pair的first是一个迭代器，指向具有关键字的元素；second成员是一个bool值，告诉我们插入成功还是已经存在于容器中**。

我们来重写单词计数程序作为例子：

```c++
map<string, size_t> word_count;
string word;
while(cin >> word)
{
    auto ret = word_count.insert({word, 1}); //不在里面，值就是1
    if(!ret.second){ ++( (ret.first)->second ); } //已在里面，递增计数器
}
```

**map单key只能对应一个value的，所以会有这种判定**

##### 向multiset或multimap添加元素

我们的单词计数程序依赖于这个事实：**一个给定的关键字只能出现一次。**
来想象这样一个场景，我们要建立作者到他作品的映射，这时候，一个作者可能有多个作品，所以啊，我们应该用**multimap，关键字不必唯一，调用insert总会插入一个元素：**

```c++
multimap<string, string> authors;
authors.insert( {"金庸", "飞狐外传"} );
authors.insert( {"金庸", "雪山飞狐"} );
```

#### 删除元素

| c.eraser(k)    | 从c中删除每个关键字为k的元素，返回一个size_type值，即删除元素的数量 |
| -------------- | ------------------------------------------------------------ |
| c.eraser(p)    | 从c中删除迭代器p指定的元素（必须存在），返回p之后元素的迭代器 |
| c.eraser(b, e) | 删除迭代器b和e之间的元素，返回e                              |

#### map的下标操作

只有map有下标**，set类型没有是因为人家就一个键，没有值，下标谁去**，**multimap没有是因为可能有多个值跟键相关**。有两种方式c[k]和c.at(k)，都是有就返回值（类型为mapped_type），没有就插入，举例：

```c++
map<string, size_t> word_count;
word_count["Anna"] = 1;
```

这样等价于把{"Anna", 1}插入了。 由于下标运算符可能插入新元素，所以我们只能对非const的map使用下标操作。

**哈希map经常用这个下标，原来map也可以用，但记住multimap不能用！！！**

#### 访问元素

C++提供了很多访问元素的方法，下面我们通过代码来看看：

```c++
set<int> iset = {0, 1, 2, 3, 4};
iset.find(1); //返回一个指向1的迭代器
iset.find(-1); //返回一个指向iset.end()的迭代器
iset.count(1); //返回1，返回该元素的数量
iset.count(11); //返回0

iset.lower_bound(2); //返回一个迭代器指向第一个不小于2的元素，就是2
iset.upper_bound(2); //返回一个指向第一个大于2的元素迭代器，3
iset.equal_range(2); //返回一个迭代器pair，表示键等于2的元素范围，没有就返回一对end
```

**这几个函数还都挺重要的.....**

**iset.lower_bound(2); //返回一个迭代器指向第一个不小于2的元素，就是2**
**iset.upper_bound(2); //返回一个指向第一个大于2的元素迭代器，3**

**都大于等于target啊，因为就是二分查找呗**

#### 一个单词转换的map

我们要写一个程序来使用本章学到的东西，这个程序的功能是这样的：给定一个string，将它转换为另一个string。程序的输入是两个文件，第一个文件保存的是一些规则，用来转换第二个文件中的文本，每条规则由两部分组成：

1. 一个可能出现在输入文件中的单词 
2. 一个用来替换它的短语 意思就是当第一个单词出现在输入中时，就把它替换为对应的短语，第二个文件就是输入文件，包含要转换的文本。看着还算简单吧，先来看个例子： 

```c++
//单词转换文件：
brb be right back
k okay?
y why
r are
u you
pic picture
thk thanks!
l8r later
//输入文本
where r uc
y dont u send me a pic
k thk l8r
//输出应该这样
where are you
why dont you send me a picture
okay? thanks! later
```

我们将这个任务分为三个函数来写：

1. 函数word_transform管理整个过程，绑定输入输出 
2. 函数buildMap读取转换规则文件，并创建一个map，用于保存每个单词到其转换内容的映射 
3. 函数transfor接受一个string，返回转换内容 

```c++
//函数word_transform
void word_transform(ifstream &map_file, ifstream &input)
{
    auto trans_map = buildMap(mapfile); 
    string text; //保存输入中的每一行
    while(getline(input, text))
    {
        istringstream stream(text); //读取每个单词,忽略空格
        string word;
        bool firstword = true; //控制是否打印空格
        while(stream >> word)
        {
            if(firstword){firstword = false;}
            else{cout << " ";}
            cout << transformm(word, trans_map); //打印输出
        }
        cout << endl; //完成一行
    }
}

//函数buildMap
map<string, string> buildMap(ifstream &map_file)
{
    map<string, string> trans_map;
    string key;
    string value;
    while(map_file >> key && getline(map_file, value)) //取一行，分别作为键值
    {
        if( value.size()>1 ) //确保有转换内容
        {
            trans_map[key] = value.substr(1); //跳过单词和转换内容之间的空格
        }
        else
        {
            throw runtime_error("no rule for " + key);
        }
    }
    return trans_map;
}

//函数transform
const string& transform(const string &s, const map<string, string> &m)
{
    auto map_it = m.find(s);
    if(map_it != m.cend())
    {
        return map_it->second;
    }
    else
    {
        return s;
    }
}
```

好了，大功告成，这是一个还蛮实用的程序，也算是个解密方法吧，可以好好看看。

**程序不错，之后可以好好看看**

### 无序容器

讲了那么多都是有序的关联容器，接下来我们来介绍下**无序的关联容器**，**这些容器不是使用比较运算符来组织元素的，而是使用一个哈希函数和关键字类型==运算符**。
**在关键字类型的元素没有明显的序关系的情况下，无序容器很好用**。
我们来用无序容器unordered_map重写最初的单词计数程序：

```c++
unordered_map<string, size_t> word_count;
string word;
while(cin >> word)
{
    ++word_count[word];
}
```

对于每个单词，我们还是得到相同的计数结果，但是不太可能按字典序输出了。

##### 管理桶

我的理解是这样，想象这儿有一排桶，每个桶本身都代表了键，桶里的东西代表值（桶里可以有一个或者多个东西），我们的无序容器提供了一些管理桶的函数，遇到再解释吧，我也记不住。

##### 无序容器对键类型的要求

我们知道它是使用==来比较元素的，那怎么比较元素呢？它并不是直接比较的，要使用一个hash<key_type>类型的对象为键生成一个哈希值，再去判等，标准库呢，已经为内置类型（包括指针）都实现了hash模板，还有string和以后要介绍的智能指针也定义了hash，这就是说，我们可以用无序容器直接去装这些类型的元素。
但是，如果要装自定义类型的元素，我们得提供自己的hash模板（以后再介绍怎么做到这一点。。。）