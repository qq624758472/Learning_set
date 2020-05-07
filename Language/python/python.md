### 方法 类方法 静态方法 私有方法 内置方法 属性方法(@property)

#### 类方法: @classmethod
类方法是将类本身作为对象进行操作的方法。他和静态方法的区别在于：不管这个方式是从实例调用还是从类调用，它都用第一个参数把类传递过来。
    

```python
class Person(object):
    count = 0
    
    @classmethod
    def how_many(cls):
        return cls.count
    def init(self, name):
        self.name = name
        Person.count = Person.count + 1
print Person.how_many()
p1 = Person('Bob')
print Person.how_many()
```


#### 静态方法 :  @staticmethod
静态方法是类中的函数，不需要实例。和函数一样。
```python
import time
class TimeTest(object):
    def __init__(self,hour,minute,second):
        self.hour = hour
        self.minute = minute
        self.second = second
    @staticmethod   
    def showTime():      
        return time.strftime("%H:%M:%S", time.localtime())


print TimeTest.showTime()   
t = TimeTest(2,10,10)
nowTime = t.showTime()
print nowTime
```


#### 私有方法 ： 
	两个下划线开头，声明该属性为私有，不能在类外部被使用或直接访问。

#### 内置方法 ： 
	内置方法__del__()   __init__（）这些已经写好的方法。

#### 属性方法 ：
	把一个方法变成一个静态属性。
创建属性方法： @property
修改属性方法:  @eat.setter
删除属性方法： @eat.deleter
```python
class Dog(object):
    def __init__(self,name):
        self.name = name
        self.__food = None

    @property #把一个方便变为静态属性
    def eat(self):
        print("%s is eating %s" %(self.name,self.__food))
    
    @eat.setter #属性方法修改，添加元素
    def eat(self,food): #创建相同名称函数的方法名
        print("set to food:",food)
        self.__food = food
    
    @eat.deleter #属性方法删除
    def eat(self):
        del self.__food
        print("删完了")

d = Dog("ChenRonghua")  #实例化
d.eat  #不添加元素，输出
d.eat = "baozi" #赋值元素，需要两个同名函数
d.eat   #赋值后输出
#输出结果
#ChenRonghua is eating None
#set to food: baozi
#ChenRonghua is eating baozi
#删完了
```

#### 请简述类方法和静态方法有哪些区别？
装饰器不同 @staticmethod @classmethod
类方法中有一个默认的参数cls,而静态方法没有


=======================属性=======================
###### **私有属性：**  
两个下划线开头，声明该属性为私有，不能在类外部被使用或直接访问。

###### 静态属性：  
类的属性都是静态属性。

###### 动态属性：  
创建了一个class的实例后，我们可以给该实例绑定任何属性和方法，这就是动态语言的灵活性。

```python
In [174]: class Student(object):
.....:     pass
.....: 

In [175]: s = Student()

In [176]: s.name = 'alex'

In [177]: print(s.name)
alex
```
它的动态属性是怎么来的呢？其实，它真正的过程是这样的

```python
In [178]: class Student(object):
    .....:     pass
    .....: 
```

```python
In [179]: s = Student()

In [180]: s.__setattr__("name","alex")

In [181]: s.__getattribute__("name")
Out[181]: 'alex'
```
由此可见，__setattr__与__getattribute__方法才是实例获得动态属性的真正原因。


#### 类属性和对象属性：
```python
class People(object):
类属性是指定义在类的内部而且在方法的外部的属性
	money = 10000

    def __init__(self,name,age,gender=1):
    # 对象属性是指定义在方法的内部的属性，例如本例中
    # name，age和gender都是对象属性
    self.name = name
    self.age = age
    self.gender = gender
```

#### 请简述新式类和经典类的区别？
#在py3里 所有的多继承问题 都符合 广度优先算法
#python3中的所有类 都默认继承object

#如果一个类 继承了object 这个类就被称为新式类
#没有继承object类 就被称为经典类

#深度优先 广度优先 都是一种遍历算法,把这个图中所有的项都走一遍,且不会重复

#经典类 遵循 深度优先算法 且没有mro方法  python2
#新式类 遵循 广度优先算法 有mro方法  py2 py3
#py3 super
#单继承中  super就是找父类
#多继承    super寻找的轨迹是根据mro(广度优先)顺序的







#### 线程与进程的区别可以归纳为以下4点：

　　1）地址空间和其它资源（如打开文件）：进程间相互独立，同一进程的各线程间共享。某进程内的线程在其它进程不可见。
　　2）通信：进程间通信IPC，线程间可以直接读写进程数据段（如全局变量）来进行通信——需要进程同步和互斥手段的辅助，以保证数据的一致性。
　　3）调度和切换：线程上下文切换比进程上下文切换要快得多。
　　4）在多线程操作系统中，进程不是一个可执行的实体。



### 多线程：

- start() 方法是启动一个子线程，线程名就是我们定义的name
- run() 方法并不启动一个新线程，就是在主线程中调用了一个普通函数而已。





#### 广播
广播域是网络中能接收任一台主机发出的广播帧的所有主机集合。也就是说，如果广播域内的其中一台主机发出一个广播帧，同一广播域内所有的其它主机都可以收到该广播帧。
广播域的计算
如何知道一台主机是属于哪一个广播域呢？其实计算很简单，只要用主机的IP地址与子网掩码进行与运算即可知道该主机属于哪一个广播域。例如：一台主机的IP地址为192.168.23.150，子网掩码为255.255.255.0，那么它所属的广播域就是192.168.23.150&255.255.255.0=192.168.23.0。那么其它的在广播域192.168.23.0内的所有主机就可以到该设备发送的广播包。如果把子网掩码改为255.255.0.0，那么它所属的广播域就是192.168.23.150&255.255.0.0=192.168.0.0。那么其它的在广播域192.168.0.0内的所有主机都可以收到该设备发送的广播包。


#### 方法重写(重载)：
如果从父类继承的方法不能满足子类的需求，可以对其进行改写，这个过程叫方法的覆盖（override），也称为方法的重写。

#### 数据成员：
类变量或者实例变量, 用于处理类及其实例对象的相关的数据。
```python
class A(object):
	i = 0  #数据成员   也是类变量


    def __init__(self):
        self.a = 1 #数据成员   也是实例变量
        self.b = 2 #数据成员

    def test(self):
        for i,j in vars(self).items():
        	print j
a = A()
a.test()
```



#### 判断一个变量是否是某个类型可以用isinstance()判断和type()函数：
```python
>>> isinstance(a, list)
>>> True
>>> isinstance(b, Animal)
>>> True
>>> isinstance(c, Dog)
>>> True

=============
>>> type(123)
>>> <class 'int'>
>>> type('str')
>>> <class 'str'>
>>> type(None)
>>> <type(None) 'NoneType'>
```

#### 类的多态：  
	多态性是指具有不同功能的函数可以使用相同的函数名，这样就可以用一个函数名调用不同内容的函数。 
	前提：有多个形态的子类继承一个父类。然后重写了某个方法。  

#### 类的继承： 
	继承是一种创建类的方法，在python中，一个类可以继承来自一个或多个父类。原始类称为基类或超类。
	可能特殊的本类又有其他特殊的地方，又会定义一个类，其下也可能再定义类，这样就会造成继承的那条线越来越长，使用继承的话,任何一点小的变化也需要重新定义一个类,很容易引起类的爆炸式增长,产生一大堆有着细微不同的子类. 所以有个“多用组合少用继承”的原则。

#### 类的组合： 
	组合是一个类有另一个类的关系。
	就是在一个类中定义了一个实例属性，这个实例属性是另外一个类。

#### 类的派生： 
     类的继承和派生其实是一回事，子类继承了父类，父类派生了子类。继承是从子类的角度来讲的，派生是从父类的角度来讲的。 

////////////////////////////////////////////////////////////////////





#### PYTHON的可变类型与不可变类型： 
    Python的每个对象都分为可变和不可变，
    主要的核心类型中，数字、字符串、元组是不可变的，列表、字典是可变的。
    对不可变类型进行赋值的时候其实是创建了一个新的不可变对象，并将指向原来不可变对象的变量指向这个新的不可变对象，这个时候原来的不可变对象就失去了指向它的变量 ，如果没有新的变量指向它的话，这个不可变对象就会被回收
    可变类型的话，以list为例。list在append之后，还是指向同个内存地址，因为list是可变类型，可以在原处修改。

////////////////////////////////////////////////////////////////////
#### 给实例绑定一个属性:
```python
    >>> s = Student()
    >>> s.name = 'Michael' # 动态给实例绑定一个属性
    >>> print(s.name)
    >>> Michael
```
#### 给实例绑定一个方法:
给一个实例绑定的方法，对另一个实例是不起作用的：
```python
    >>> def set_age(self, age): # 定义一个函数作为实例方法
    >>> ...     self.age = age
    >>> ...
    >>> from types import MethodType
    >>> s.set_age = MethodType(set_age, s) # 给实例绑定一个方法
    >>> s.set_age(25) # 调用实例方法
    >>> s.age # 测试结果
    >>> 25
```
给class绑定方法后，所有实例均可调用：
```python
    >>> def set_score(self, score):
    >>> ...     self.score = score
    >>> ...
    >>> Student.set_score = set_score
```


#### 使用__slots__限制实例的属性
只允许对Student实例添加name和age属性。
```python
class Student(object):
    __slots__ = ('name', 'age') # 用tuple定义允许绑定的属性名称
```


使用__slots__要注意，__slots__定义的属性仅对当前类实例起作用，对继承的子类是不起作用的：
```python
>>> class GraduateStudent(Student):
>>> ...     pass
>>> ...
>>> g = GraduateStudent()
>>> g.score = 9999
>>> 除非在子类中也定义__slots__，这样，子类实例允许定义的属性就是自身的__slots__加上父类的__slots__。
```


### Python中的列表（list），元组（Tuple），字典（Dict）和集合（Set）
#### 列表（list）：
	定义一个列表使用一对中(方)括号”[ ] “。
	是一个有序集合，意思是对比两个list，元素相同，位置不同的list，也是不同的。也就是说是划分位置的，此为有序。
下面我们来看看Python中的列表的一些基本操作。 
1.可以使用len()函数获得列表的长度（字典中的元素的个数）； 
2.可以使用索引来引用列表中的元素，注意：列表中的索引是从０开始的，并且在列表中还支持负索引； 
3.list是一个可以变的有序列表，因此可以往你自己的列表中添加和删除元素：在末尾添加元素用的是append()方法，在指定的位置插入元素使用的是insert()方法； 
4.在列表中删除元素：删除末尾的元素使用的是pop()方法，删除指定位置的元素使用pop(i),其中i是索引下标； 
5.若想替换list中的某个元素，可以直接把该元素赋值给对应的元素下标即可； 
6.在一个list中可以有不同的数据类型，可以有字符串类型，整型，或者bool等，并且list的元素中也可以有另外一个list，就相当于一个循环的嵌套一样。



#### 元组（Tuple）：
​	定义一个元组使用一对小(圆)括号”( ) “。
​	和列表类似，元组也是一种有序列表，虽然tuple和list非常之类似，但是list初始化之后使可以改变的，但是，元组一旦初始化之后就不可以改变。这点与Python中的字符串类似，所以我们说元组和字符串都是不可变的序列。
​	现在tuple不能变了，它也没有append()，insert()这样的方法。其他获取元素的方法和list是一样的，你可以正常地使用tuple[0]，tuple[-1]，但不能赋值成另外的元素。 
​	不可变的tuple有什么意义？因为tuple不可变，所以代码更安全。如果可能，能用tuple代替list就尽量用tuple。

1.当你定义一个tuple时，在定义的时候，tuple的元素就必须被确定下来； 
2.定义只有一个元素的Tuple的时候，需要这样: 
tuple1 = (123,) 
tuple2 = (123) ＃如果你这样定义你定义的将是１２３这个元素，而不是一个元组。 
3.元组中的元素值是不允许修改的，但我们可以对元组进行连接组合：
4.元组中的元素值是不允许删除的，但我们可以使用del语句来删除整个元组。
	元组的内置函数： 
1.比较两个元组元素：cmp(tuple1,tuple2)相等的话返回0，不相等返回1； 
2.计算元组的长度：len(tuple)
3.返回元组中的最大值最小值：max(tuple),min(tuple)； 
4.将列表转换成元组：Tuple = tuple(list)。



#### 字典（Dict）:
​	定义 Dictionary 使用一对大(花)括号” { } ”。 
​	字典(Dictionary) 是 Python 的内置数据类型之一，它定义了键和值之间一对一的关系,但它们是以无序的方式储存的。字典中的“值”通过键来引用。
​	与列表区别：字典是无序的，在字典中通过键来访问成员。 
​	字典是可变的，可以包含任何其他类型
常用字典操作： 
dic.clear()清空字典 
dic.keys()获得键的列表 
dic.values()获得值的列表 
dic.copy()复制字典 
dic.pop(k)删除键k 
dic.get(k)获得键k的值 
dic.update()更新成员，若成员不存在，相当于加入 
dic.items()获得由键和值组成的列表



#### 集合（Set）:
是一个无序不重复元素集, 基本功能包括关系测试和消除重复元素.
```python
    #定义一个集合
    set1 = {1, 2, 3, 4, 5}
    # 或者使用 set 函数
    list1 = [6, 7, 7, 8, 8, 9]
    set2 = set(list1)
    set2.add(10) # 添加新元素
    print set2 # set([8, 9, 6, 7]) 去掉重复内容,而且是无序的
    set3 = frozenset(list1)    #固定集合
    set3.add(10) # 固定集合不能添加元素
    #方法（所有的集合方法）：
    s.issubset(t) #如果s是t的子集,返回True，否则返回False
    s.issuperset(t) #如果s是t的超集,返回True，否则返回False
    s.union(t) #返回一个新集合, 该集合是s和t的并集
    s.intersection(t) #返回一个新集合, 该集合是s和t的交集
    s.difference(t) #返回一个新集合, 该集合是s的成员, 但不是t的成员, 即返回s不同于t的元素
    s.symmetric_defference(t) #返回所有s和t独有的(非共同拥有)元素集合
    s.copy() #返回一个s的浅拷贝, 效率比工厂要好
    #方法（仅适用于可变集合）:以下方法参数必须是可哈希的
    s.update(t) #用t中的元素 修改s，即s现在包含s或t的成员
    s.intersection_update(t) #s中的成员是共同属于s和t的元素
    s.difference_update(t) #s中的成员是属于s但不包含在t中的元素
    s.symmetric_difference_update(t) #s中的成员更新为那些包含在s或t中，但不是s和t共有的元素
    s.add(obj) #在集合s中添加对象obj
    s.remove(obj) #从集合s中删除对象obj，如果obj不是集合s中的元素（obj not in s）,将引发keyError错误
    s.discard(obj) #如果obj是集合s中的元素，从集合s中删除对象obj
    s.pop() #删除集合s中得任意一个对象，并返回它
    s.clear() #删除集合s中的所有元素
    ## 集合有并集，交集，求差操作
    ## 并集：intersection() 方法返回一个新集合，包含在两个集合中同时出现的所有元素。
    ## 交集：union() 方法返回一个新集合，包含在两个 集合中出现的元素。
    ## 差集：difference() 方法返回的新集合中，包含所有在 集合A出现但未在集合B中的元素。
    ## symmetric_difference() 方法返回一个新集合，包含所有只在其中一个集合中出现的元素。
    # 删除元素
    set2.discard(6) # 当元素不存在时,不会引发异常
    set2.remove(6) # 与discard的区别在于，如果没有要删除的元素，remove会引发一个异常
    set2.pop() # 因为set是无序的，所以pop会随机的从set中删除一个元素
```




////////////////////////////////////////////////////////////////////
#### 练习
如果list中既包含字符串，又包含整数，由于非字符串类型没有lower()方法，所以列表生成式会报错
使用内建的isinstance函数可以判断一个变量是不是字符串：
返回True 或 False

```python
L1 = ['Hello', 'World', 18, 'Apple', None]
[L2.lower() for L2 in L1 if isinstance(L2, str) == True]
```






/////////////////////////////////////////////////////////////////////////////////////////////
#### 迭代(Iteration): 

如果给定一个list或tuple，我们可以通过for循环来遍历这个list或tuple，这种遍历我们称为迭代（Iteration）。
	可以使用for in  来进行循环操作的叫迭代。

#### 列表生成式 (List Comprehensions)： 

列表生成式即List Comprehensions，是Python内置的非常简单却强大的可以**用来创建list的生成式**。
```python
>>> [x * x for x in range(1, 11) if x % 2 == 0]
>>> [4, 16, 36, 64, 100]
```

#### 生成器（generator） ： 

​	通过列表生成式，我们可以直接创建一个列表。但是，受到内存限制，列表容量肯定是有限的。而且，创建一个包含100万个元素的列表，不仅占用很大的存储空间，如果我们仅仅需要访问前面几个元素，那后面绝大多数元素占用的空间都白白浪费了。

​	所以，如果列表元素可以按照某种算法推算出来，那我们是否可以在循环的过程中不断推算出后续的元素呢？这样就不必创建完整的list，从而节省大量的空间。在Python中，这种一边循环一边计算的机制，称为生成器：generator。
创建生成器： 
1. 第一种方法很简单，只要把一个列表生成式的[]改成()，就创建了一个generator：
```python
>>> L = [x * x for x in range(10)]
>>> L
>>> [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
>>> g = (x * x for x in range(10))
>>> g
>>> <generator object <genexpr> at 0x1022ef630>
```

2. 如果一个函数定义中包含`yield`关键字，那么这个函数就不再是一个普通函数，而是一个generator： 下边代码为一个菲波那切数列：

```python
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        yield b
        a, b = b, a + b
        n = n + 1
    return 'done'
```

#### 迭代器（Iterator）

一类是集合数据类型，如`list`、`tuple`、`dict`、`set`、`str`等；

一类是`generator`，包括生成器和带`yield`的generator function。

这些可以直接作用于`for`循环的对象统称为**可迭代对象：`Iterable`**。

可以使用`isinstance()`判断一个对象是否是`Iterable`对象：

​	生成器都是`Iterator`对象，但`list`、`dict`、`str`虽然是`Iterable`，却不是`Iterator`。

把`list`、`dict`、`str`等`Iterable`变成`Iterator`可以使用`iter()`函数：

```python
>>> isinstance(iter([]), Iterator)
True
>>> isinstance(iter('abc'), Iterator)
True
```

**小结 ：**

凡是可作用于`for`循环的对象都是`Iterable`类型；

凡是可作用于`next()`函数的对象都是`Iterator`类型，它们表示一个惰性计算的序列；

集合数据类型如`list`、`dict`、`str`等是`Iterable`但不是`Iterator`，不过可以通过`iter()`函数获得一个`Iterator`对象。

Python的`for`循环本质上就是通过不断调用`next()`函数实现的，例如：

```python
for x in [1, 2, 3, 4, 5]:
    pass
```

实际上完全等价于：

```python
# 首先获得Iterator对象:
it = iter([1, 2, 3, 4, 5])
# 循环:
while True:
    try:
        # 获得下一个值:
        x = next(it)
    except StopIteration:
        # 遇到StopIteration就退出循环
        break
```



### 高阶函数

1. #### map()函数

   接收两个参数，一个是函数，一个是Iterable，map将传入的函数依次作用到序列的每个元素，并把结果作为新的Iterator返回。

   ```python
   >>> list(map(str, [1, 2, 3, 4, 5, 6, 7, 8, 9]))
   ['1', '2', '3', '4', '5', '6', '7', '8', '9']
   ```

2. #### `reduce`函数

   把一个函数作用在一个序列`[x1, x2, x3, ...]`上，这个函数必须接收两个参数，`reduce`把结果继续和序列的下一个元素做累积计算.

   效果就是：

   ```
   reduce(f, [x1, x2, x3, x4]) = f(f(f(x1, x2), x3), x4)
   ```

   比方把序列`[1, 3, 5, 7, 9]`变换成整数`13579`

   ```python
   >>> from functools import reduce
   >>> def fn(x, y):
   ...     return x * 10 + y
   ...
   >>> reduce(fn, [1, 3, 5, 7, 9])
   13579
   ```

   如果考虑到字符串`str`也是一个序列，对上面的例子稍加改动，配合`map()`，我们就可以写出把`str`转换为`int`的函数：

   ```python
   >>> from functools import reduce
   >>> def fn(x, y):
   ...     return x * 10 + y
   ...
   >>> def char2num(s):
   ...     digits = {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}
   ...     return digits[s]
   ...
   >>> reduce(fn, map(char2num, '13579'))
   13579
   ```

3. #### lambda函数 也叫匿名函数

   ```python
   a=lambda x:x*x   #一个参数
   p = lambda x,y:x+y   #两个参数
   ```

   匿名函数lambda：是指一类无需定义标识符（函数名）的函数或子程序。
   lambda 函数可以接收任意多个参数 (包括可选参数) 并且返回单个表达式的值。

   什么是lambda函数？他有什么好处？ 

   lambda函数是匿名函数；使用lambda函数能够创建小型匿名函数。这种函数得名于省略了用def声明函数的标准步骤；

   **要点：**
   **1，lambda 函数不能包含命令，**

   **2，包含的表达式不能超过一个。**

   说明：一定非要使用lambda函数；任何能够使用它们的地方，都可以定义一个单独的普通函数来进行替换。我将它们用在需要封装特殊的、非重用代码上，避免令我的代码充斥着大量单行函数。

   lambda匿名函数的格式：冒号前是参数，可以有多个，用逗号隔开，冒号右边的为表达式。其实lambda返回值是一个函数的地址，也就是函数对象。

   ```python
   **a=lambda x:x\*x**
   **print(a)**
   **print(a(3))**
   ---->
   <function <lambda> at 0x0000000002093E18>
   ```

4. #### `filter()`函数

   和`map()`类似，`filter()`也接收一个函数和一个序列。和`map()`不同的是，`filter()`把传入的函数依次作用于每个元素，然后根据返回值是`True`还是`False`决定保留还是丢弃该元素。

   把一个序列中的空字符串删掉，可以这么写：

   ```python
   def not_empty(s):
       return s and s.strip()  #strip() 方法用于移除字符串头尾指定的字符（默认为空格或换行符）或字符序列。
   
   list(filter(not_empty, ['A', '', 'B', None, 'C', '  ']))
   # 结果: ['A', 'B', 'C']
   ```

5. #### `sorted()`函数

   `sorted()`函数就可以对list进行排序.

   ```python
   >>> sorted([36, 5, -12, 9, -21])
   [-21, -12, 5, 9, 36]
   ```

   它还可以接收一个`key`函数来实现自定义的排序，例如按绝对值大小排序：

   ```python
   >>> sorted([36, 5, -12, 9, -21], key=abs)
   [5, 9, -12, -21, 36]
   ```

6. #### 返回函数(闭包)

   #### 

7. #### 偏函数

   通过设定参数的默认值，可以降低函数调用的难度。

   将一些参数固定。

```python
>>> import functools
>>> int2 = functools.partial(int, base=2)
>>> int2('1000000')
64
>>> int2('1010101')
85
```

8. #### *args and **kwargs

   *args代表位置参数，它会接收任意多个参数并把这些参数作为元组传递给函数。**kwargs代表的关键字参数，允许你使用没有事先定义的参数名，另外，位置参数一定要放在关键字参数的前面。







### Python是如何进行内存管理的

一、垃圾回收：python不像C++，Java等语言一样，他们可以不用事先声明变量类型而直接对变量进行赋值。对Python语言来讲，对象的类型和内存都是在运行时确定的。这也是为什么我们称Python语言为动态类型的原因（这里我们把动态类型可以简单的归结为对变量内存地址的分配是在运行时自动判断变量类型并对变量进行赋值）。

二、引用计数：Python采用了类似Windows内核对象一样的方式来对内存进行管理。每一个对象，都维护这一个对指向该对对象的引用的计数。当变量被绑定在一个对象上的时候，该变量的引用计数就是1，(还有另外一些情况也会导致变量引用计数的增加),系统会自动维护这些标签，并定时扫描，当某标签的引用计数变为0的时候，该对就会被回收。

三、内存池机制Python的内存机制以金字塔行，-1，-2层主要有操作系统进行操作，

　　    第0层是C中的malloc，free等内存分配和释放函数进行操作；

　　    第1层和第2层是内存池，有Python的接口函数PyMem_Malloc函数实现，当对象小于256K时有该层直接分配内存；

　　    第3层是最上层，也就是我们对Python对象的直接操作；

在 C 中如果频繁的调用 malloc 与 free 时,是会产生性能问题的.再加上频繁的分配与释放小块的内存会产生内存碎片. Python 在这里主要干的工作有:

如果请求分配的内存在1~256字节之间就使用自己的内存管理系统,否则直接使用 malloc.

这里还是会调用 malloc 分配内存,但每次会分配一块大小为256k的大块内存.

经由内存池登记的内存到最后还是会回收到内存池,并不会调用 C 的 free 释放掉.以便下次使用.对于简单的Python对象，例如数值、字符串，元组（tuple不允许被更改)采用的是复制的方式(深拷贝?)，也就是说当将另一个变量B赋值给变量A时，虽然A和B的内存空间仍然相同，但当A的值发生变化时，会重新给A分配空间，A和B的地址变得不再相同

 





## Python 值传递和引用传递

值传递类型例如以下：

上面的代码仅仅是将a的值作为參数传递进去。程序运行完之后a的值并未改变

```python
a = 1
def change_integer(a):
    a = a + 1
    return a
print change_integer(a)
print a
```

结果：

2

1

引用传递的类型例如以下：

```python
b = [1, 2, 3]
def change_list(b):
    b[0]= b[0]+1
    return b
 
print change_list(b)
print b

```

结果：

[2, 2, 3]

[2, 2, 3]

怎样解决 变量 ： 由 引用传递 变为 值传递？？

方案例如以下：

```python
import copy
 
b = [1, 2, 3]
def change_list(b):
    b[0]= b[0]+1
    return b
 
print change_list(copy.deepcopy(b))
print b
```

结果：

[2, 2, 3]

[1, 2, 3]







# 深浅拷贝

### c语言中的：

**浅拷贝:**拷贝指针变量的值

```c
//声明一个结构体
typedef struct Person {
  char name[20];
  int age;
  char *alias;
}Person;
 
//拷贝方法
void copyPerson(Person *from, Person *to){
 
  *to = *from;
}
//main函数
int main(int argc, const char * argv[]) {
 
  Person p1;
  p1.age = 11;
  strcpy(p1.name, "royce");
  p1.alias = "owen";
  Person p2;
 
  copyPerson(&p1, &p2);
 
  printf("p2:%p p1:%p\np2-alias:%p p1-alias:%p\n",&p2,&p1,p2.alias,p1.alias);
 
  return 0;
}
//打印
p2-alias:0x100000f80 p1-alias:0x100000f80
```

**深拷贝:**拷贝指针所指向内存空间	

```c
void copyPerson(Person *from, Person *to){
 
  *to = *from;
  to->alias = (char *)malloc(100);
  strcpy(to->alias, from->alias);
 
}
//打印
p2-alias:0x1003069a0 p1-alias:0x100000f82
```

### python中的：

**深拷贝:**拷贝指针所指向内存空间	

**浅拷贝:**拷贝指针变量的值

深浅拷贝有什么区别那？

​    1、如果用copy.copy、copy.deepcopy对一个全部都是不可变类型的数据进行拷贝，那么它们结果相同，都是引用指向；

​    2、如果拷贝的是一个拥有不可变类型的数据，即使元组是最顶层，那么deepcopy依然是深拷贝，而copy.copy还是指向

​    3、基本上只要不是我们自已手动调用的deepcopy方法都是浅拷贝，切片拷贝字典拷贝都是浅拷贝，而有些内置函数可以生成拷贝(list)，属于深拷贝:a = list(range(10))
b = list(a) 

三个区别：

**赋值**，只是创建一个变量，该变量指向原来内存地址，如下例：

```python
n4 = n3 = n2 = n1 = "123/'Wu'"
```

![img](https://images2015.cnblogs.com/blog/1008059/201609/1008059-20160906170515394-755865825.png)

关于赋值，再看一个字典的例子:

```python
n1 = {"k1": "wu", "k2": 123, "k3": ["alex", 456]}
n2 = n1
```

![img](https://images2015.cnblogs.com/blog/1008059/201609/1008059-20160906171450144-426640184.png)

**浅拷贝**，在内存中只额外创建第一层数据，如下图

```python
import copy
n1 = {"k1": "wu", "k2": 123, "k3": ["alex", 456]}
n3 = copy.copy(n1)
```

![img](https://images2015.cnblogs.com/blog/1008059/201609/1008059-20160906171507473-471130328.png)

**深拷贝**，在内存中将所有的数据重新创建一份（排除最后一层，即：python内部对字符串和数字的优化），如下图：

```python
import copy
n1 = {"k1": "wu", "k2": 123, "k3": ["alex", 456]}
n4 = copy.deepcopy(n1)
```

![img](https://images2015.cnblogs.com/blog/1008059/201609/1008059-20160906171958801-1461628350.png)



#### python中如何拷贝一个对象？（赋值，浅拷贝，深拷贝的区别）

答：

**赋值（=）　：**就是创建了对象的一个新的引用，修改其中任意一个变量都会影响到另一个。

**浅拷贝 　　：**创建一个新的对象，但它包含的是对原始对象中包含项的引用（如果用引用的方式修改其中一个对象，另外一个也会修改改变）{1,完全切片方法；2，工厂函数，如list()；3，copy模块的copy()函数}

**深拷贝　　：**创建一个新的对象，并且递归的复制它所包含的对象（修改其中一个，另外一个不会改变）{copy模块的deep.deepcopy()函数}



**内建函数**:  就是编译器内置的一些函数。

**工厂函数**：能够产生类实例的内建函数。工厂函数是指这些内建函数都是类对象， 当调用它们时，实际上是创建了一个类实例。

```python
python中的工厂函数举例如下：

1》int(),long(),float(),complex(),bool()

>>> a=int(9.9)
>>> a
9

>>> b=long(45)
>>> b
45L

>>> f=float(8)
>>> f
8.0

>>> c=complex(8)
>>> c
(8+0j)

>>> b1=bool(7.9)
>>> b1
True
>>> b2=bool(0.0)
>>> b2
False
>>> b3=bool([])
>>> b2
False
>>> b4=bool((34,5))
>>> b4
True

2》str(),unicode()

>>> s=str(9.9)
>>> s
'9.9'

>>> unicode(9.0)
u'9.0'

>>> unicode('love')
u'love'

3》list(),tuple()：生成列表或者元组

>>> l=list('python')
>>> l
['p', 'y', 't', 'h', 'o', 'n']

>>> t=tuple('python')
>>> t
('p', 'y', 't', 'h', 'o', 'n')

4》type()：查看类型   

>>> type(6)
<type 'int'>
>>> type('python')
<type 'str'>

>>> type(u'love')
<type 'unicode'>
>>> class A():
...     pass
... 
>>> a=A()
>>> type(a)
<type 'instance'>
>>> type(A)
<type 'classobj'>

5》dict()：生成一个字典

>>> dict()
{}
>>> dict(one=1,two=2)
{'two': 2, 'one': 1}

>>> dict(zip(('one','two'),(1,2)))
{'two': 2, 'one': 1}
>>> dict([('one',1),('two',2)])
{'two': 2, 'one': 1}

>>> dict([['one',1],['two',2]])
{'two': 2, 'one': 1}
>>> dict((('one',1),('two',2)))
{'two': 2, 'one': 1}

>>> dict((['one',1],['two',2]))
{'two': 2, 'one': 1}

6》set():   生产可变集合

>>> s=set('python')
>>> s
set(['h', 'o', 'n', 'p', 't', 'y'])
>>> s.add(825)#可变集合
>>> s
set(['h', 'o', 'n', 'p', 't', 'y', 825])

7》frozenset()：生成不可变集合

>>> s=frozenset('python')
>>> s
frozenset(['h', 'o', 'n', 'p', 't', 'y'])
>>> s.add()#不可变集合

AttributeError: 'frozenset' object has no attribute 'add'
```



## 正则表达式：

用python匹配HTML tag的时候，<.*>和<.*?>有什么区别？

1. <.*>匹配结果是：HTML tag；
2. <.*?>匹配到的结果是：""。 ？ 表示非贪婪，搜到符合的结果既返回





**有用过with statement吗？它的好处是什么？具体如何实现？**

with语句适用于对资源进行访问的场合，确保不管使用过程中是否发生异常都会执行必要的“清理”操作，释放资源，比如文件使用后自动关闭、线程中锁的自动获取和释放等。











## ＭＹＳＱＬ数据库

1. 有三张表分别存放省市（province），地市（city）和县区（area）的名称和编码，表名如下表格：

| 省份表      | Province | 地市表      | City   | 县表    | Area     |
| ----------- | -------- | ----------- | ------ | ------- | -------- |
| province_id | 省编号   | city_id     | 市编号 | area_id | 县区编号 |
| province    | 省       | city        | 城市名 | area    | 区县名称 |
|             |          | province_id | 省编号 | city_id | 市编号   |

请写出一段sql语句，挑选出江苏省的地市和区县。
输出这样:
  南京市 秦淮区
  南京市 玄武区
  ···
  徐州市 鼓楼区
  徐州市 沛县

```mysql
select City.city,Area.area from City inner Area on City.city_id = Area.city_id where 

City.province_id = (select province_id from Province where province = "江苏省"）

```







**装饰器** ：其实就是一个闭包，把一个函数当做参数然后返回一个替代版函数。

**偏函数**： 设定参数的默认值，可以降低函数调用的难度。



c++中三种方式。

**public:**可以被任意实体访问

**protected:**只允许子类及本类的成员函数访问

**private:**只允许本类的成员函数访问



__number是私有方法 _number是相当于c++的protect方法



## Python中的 _init__和 _new__的区别

__new__方法接受的参数虽然也是和__init__一样，但__init__是在类实例创建之后调用，而 __new__方法正是创建这个类实例的方法。





# python一些常用命令



##### 1.python安装requirements.txt命令：

```shell
pip install -r requirements.txt
```

##### 2. python使用虚拟环境

1. sudo apt-get install python-virtualenv或者 sudo pip install virtualenv
2. pip install virtualenvwrapper 
3. 安装完成之后，需要在用户根目录下（即/home/[username]）的.bashrc末尾加入：
       source /usr/local/bin/virtualenvwrapper.sh  然后执行source ~/.bashrc 
4. ​    创建并进入环境：mkvirtualenv env1
   ​    退出环境：deactivate
   ​    进入已存在的环境或者切换环境：workon env1或者env2
   ​    删除环境： rmvirtualenv env1












