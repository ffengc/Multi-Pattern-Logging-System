# 前置技术补充

- [简体中文](./supplement.md)
- [English](./supplement-en.md)

***
- [前置技术补充](#前置技术补充)
  - [不定参](#不定参)
    - [不定参宏](#不定参宏)
    - [C风格不定参函数](#c风格不定参函数)
    - [C++风格不定参函数](#c风格不定参函数-1)
  - [设计模式](#设计模式)
    - [六大原则](#六大原则)
    - [单例模式](#单例模式)
    - [工厂模式](#工厂模式)
    - [建造者模式](#建造者模式)
    - [代理模式](#代理模式)

本文档的所有相关代码都存放在 `./demo` 中。

## 不定参

### 不定参宏

使用如下所示。

```c
#include <stdio.h>
#define LOG(fmt, ...) printf("[%s, %d]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);
int main() {
    LOG("hello world\n");
    LOG("hello world: %d\n", 2);
    return 0;
}
```

> [!TIP]
> 这里的 `##` 表示，如果没有不定参数，则取消前面的逗号。

### C风格不定参函数

具体如代码所示:

```c
// C风格不定参函数
void print_arg_num(int count, ...) {
    va_list ap;
    va_start(ap, count); // 获取指定参数起始地址，这里就是获取count参数之后第一个参数的起始地址
    for (int i = 0; i < count; ++i) {
        int num = va_arg(ap, int); // 这里要告诉 va_arg 是什么类型的数据，这样编译器才知道要在栈上获取具体多少空间
        printf("param[%d]:%d\n", i, num);
    }
    va_end(ap); // 将ap指针置空
}
void test2() {
    print_arg_num(2, 666, 777);
    print_arg_num(5, 1, 2, 3, 4, 5);
}
```

然后我们来模拟实现一下 `printf` 函数。

```c
// 模拟实现 printf
void myprintf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char* res;
    int ret = vasprintf(&res, fmt, ap);
    if (ret != -1) {
        printf(res);
        free(res);
    }
    va_end(ap);
}
void test3() {
    myprintf("hello world: %d\n", 2);
}
```

### C++风格不定参函数

具体使用如下所示。

```cpp
void xprintf() {
    std::cout << std::endl;
}
template <typename T, typename... Args> // Args 相当于一个参数包
void xprintf(const T& v, Args&&... args) {
    std::cout << v << " ";
    if ((sizeof...(args)) > 0)
        xprintf(std::forward<Args>(args)...);
    else
        xprintf();
}
void test4() {
    xprintf("hello");
    xprintf("hello", "world");
    xprintf("hello", "world", 666, std::string("hi"));
}
```

## 设计模式

> [!NOTE]
> 设计模式是前辈们对代码开发经验的总结，是解决特定问题的一系列套路。它不是语法规定，而是一套用来提高代码可复用性、可维护性、可读性、稳健性以及安全性的解决方案。

### 六大原则

**单一职责原则（Single Responsibility Principle）:**
- 类的职责应该单一，一个方法只做一件事。职责划分清晰了，每次改动到最小单位的放大或类
- 简单来说就是两个完全不一样功能不应该放在一个类中，一个类中应该是一组相关性很高的函数、数据的封装

**开闭原则（Open Closed Principle）：**
- 对扩展开放，对修改封闭
- 使用建议：对软件实体的改动，最后是采用扩展的方式，而不是修改的方式

**里氏替换原则（Liskov Substitution Principle）：**
- 通俗讲，就是只要父类能出现的地方，子类就可以出现，而且替换成子类也不会产生任何错误和异常
- 在继承类的时候，务必重写父类中的所有方法，尤其需要注意父类的 `protected` 方法，子类尽量不要暴露自己的 `public` 方法供外界调用
- 使用建议：子类必须完全实现父类的方法，孩子类可以有自己的个性。覆盖或者实现父类的方法的时候，输入参数可以被放大，输出可以被缩小。

**依赖倒置原则（Dependence Inversion Principle）：**
- 高层模块不应该依赖低层模块，两者都应该依赖其抽象. 不可分割的原子逻辑就是低层模式子逻辑组装成的就是高层模块
- 模块间依赖通过抽象（接口）发生，具体类之间不直接依赖
- 每个类都尽量有抽象类，任何类都不应该从具体类派生。尽量不要重写基类的方法。结合里氏替换原则使用。

**迪米特法则（Law of Demeter）：**
- 尽量减少对象之间的交互，从而减小类之间的耦合。一个对象应该对其他对象有最少的了解。对类的低耦合提出了明确的要求
- 只和直接的朋友交流，朋友之间也是有距离的。自己的就是自己的（如果一个方法放在本类中，既不增加类间关系，也对本类不产生负面影响，那就放置在本类中）。

**接口隔离原则（Interface Segregation Principle）：**
- 客户端不应该以来它需要的接口，类间的依赖关系应该建立在最小的接口上
- 使用建议：接口设计尽量精简单一，但是不要对外暴露没有实际意义的接口

> [!TIP]
> **到每一条设计原则，则对应一条注意事项：**
> - 单一职责原则告诉我们实现类要职责单一；
> - 里氏替换原则告诉我们不要破坏继承体系；
> - 依赖倒置原则告诉我们要面向接口编程；
> - 接口隔离原则告诉我们在设计接口的时候要精简单一；
> - 迪米特法则告诉我们要降低耦合；
> - 开闭原则是总纲，告诉我们要对扩展开放，对修改关闭。


### 单例模式

一个类只能创建一个对象，即单例模式，该设计模式可以保证系统中该类只有一个实例，并提供一个访问它的全局访问点，该实例被所有程序模块共享。比如在某个服务器程序中，该服务器的配置信息存放在一个文件中，这些配置数据由一个单例对象统一读取，然后服务进程中的其他对象再通过这个单例对象获取这些配置信息，这种方式简化了在复杂环境下的配置管理。

**单例模式有两种实现模式：饿汉模式和懒汉模式**

具体可见博客: https://blog.csdn.net/Yu_Cblog/article/details/131787131


### 工厂模式

这部分代码见: `demo/factory.cc`

工厂模式是一种创建型设计模式，它提供了一种创建对象的最佳方式。在工厂模式中我们创建对象时不会对上层暴露创建逻辑，而是通过使用一个共同结构来指向新创建的对象，以此实现创建-使用的分离。

**工厂模式可以分为:**
- 简单工厂模式：简单工厂模式实现由一的实例。假设个工厂对象通过类型决定创建出来指定产品类有个工厂能生产出水果，当客户需要产需要接收用户提品的时候明确告知工厂生产哪类水果，工厂厂内部去添加新产品的生产方式。供的类别信息，当新增产品的时候，工厂内部去添加新产品的生产方式。

```cpp
#include <iostream>
#include <memory>

class Fruit {
public:
    virtual void name() = 0;
};
class Apple : public Fruit {
public:
    void name() override { std::cout << "I am an Apple" << std::endl; }
};
class Banana : public Fruit {
public:
    void name() override { std::cout << "I am a Banana" << std::endl; }
};
class FruitFactory {
public:
    static std::shared_ptr<Fruit> create(const std::string& name) {
        if (name == "apple")
            return std::make_shared<Apple>();
        else
            return std::make_shared<Banana>();
    }
};
int main() {
    std::shared_ptr<Fruit> fruit = FruitFactory::create("apple");
    fruit->name();
    fruit = FruitFactory::create("banana");
    fruit->name();
    return 0;
}
```
其实这个就是简单工厂模式的意思。但是这种结构扩展性比较差，如果要新增一种水果，就要修改工厂的代码，违背了开闭原则。

**所以提出第二种模式，工厂方法模式：**
- 再简单工厂模式下新增多个工厂，多个产品，每个产品对应一个工厂。假设现在有A、B两种产品，则开两个工厂，用户只知道工厂名字但是不知道具体的产品信息，工厂也不需要再去接收客户的产品类别，只负责生产产品。

```cpp
class FruitFactory {
public:
    virtual std::shared_ptr<Fruit> create() = 0;
};
class AppleFactory : public FruitFactory {
public:
    AppleFactory() = default;
    std::shared_ptr<Fruit> create() override { return std::make_shared<Apple>(); }
};
class BananaFactory : public FruitFactory {
public:
    BananaFactory() = default;
    std::shared_ptr<Fruit> create() override { return std::make_shared<Banana>(); }
};
int main() {
    std::shared_ptr<FruitFactory> ff(new AppleFactory());
    std::shared_ptr<Fruit> fruit = ff->create();
    fruit->name();
    ff.reset(new BananaFactory());
    fruit = ff->create();
    fruit->name();
    return 0;
}
```
这个就是工厂方法的简单代码。

工厂方法模式每次增加一个产品时，都需要增加一个具体产品类和工厂类，这会使得系统中类的个数成倍增加，在一定程度上增加了系统的耦合度。

**第三种方式：抽象工厂模式**

工厂方法模式通过引入工厂等级结构，解决了简单工厂模式中工厂类职责太重的问题，但由于工厂方法模式中的每个工厂只生产一类产品，可能会导致系统中存在大量的工厂类，势必会增加系统的开销。此时，我们可以考虑将一些相关的产品组成一个产品族（位于不同产品等级结构中功能相关联的产品组成的家族），由同一个工厂来统一生产，这就是抽象工厂模式的基本思想。


```cpp
class Factory {
public:
    virtual std::shared_ptr<Fruit> getFruit(const std::string& name) = 0;
    virtual std::shared_ptr<Animal> getAnimal(const std::string& name) = 0;
};
class FruitFactory : public Factory {
public:
    std::shared_ptr<Animal> getAnimal(const std::string& name) { return std::shared_ptr<Animal>(); }
    std::shared_ptr<Fruit> getFruit(const std::string& name) {
        if (name == "apple")
            return std::make_shared<Apple>();
        else
            return std::make_shared<Banana>();
    }
};
class AnimalFactory : public Factory {
public:
    std::shared_ptr<Fruit> getFruit(const std::string& name) { return std::shared_ptr<Fruit>(); }
    std::shared_ptr<Animal> getAnimal(const std::string& name) {
        if (name == "dog")
            return std::make_shared<Dog>();
        else
            return std::make_shared<Lamp>();
    }
};
class FactoryProducer {
public:
    static std::shared_ptr<Factory> create(const std::string& name) {
        if (name == "fruit")
            return std::make_shared<FruitFactory>();
        else
            return std::make_shared<AnimalFactory>();
    }
};
int main() {
    std::shared_ptr<Factory> ff = FactoryProducer::create("fruit");
    std::shared_ptr<Fruit> fruit = ff->getFruit("apple");
    fruit->name();
    fruit = ff->getFruit("banana");
    fruit->name();

    ff = FactoryProducer::create("animal");
    std::shared_ptr<Animal> animal = ff->getAnimal("dog");
    animal->name();
    animal = ff->getAnimal("lamp");
    animal->name();
    return 0;
}
```


### 建造者模式

建造者模式是一种创建型设计模式，使用多个简单对象一步一步构建成一个复杂对象，能够将一个复杂的对象的构建与它的表示进行分离，提供了一种创建对象的最佳方式。

建造者模式主要基于4个核心类实现：
- 抽象产品类
- 具体产品类
- 抽象 `Builder` 类：创建一个产品对象所需的各个部件的抽象接口
- 具体产品的 `Builder` 类：实现抽象接口，构建各个部件
- 指挥者 `Director` 类：统一组建过程，提供给调用者使用，通过指挥者来构造产品

示例代码如下所示:
```cpp
#include <iostream>
#include <memory>
#include <string>

class Computer {
protected:
    std::string __board;
    std::string __display;
    std::string __os; //
public:
    Computer() = default;
    void setBoard(const std::string& name) { __board = name; }
    void setDisplay(const std::string& name) { __display = name; }
    virtual void setOS() = 0;
    std::string toString() {
        std::string param = "Computer Paramaters: \n";
        param += "\tBoard: " + __board + "\n";
        param += "\tDisplay: " + __display + "\n";
        param += "\tOs: " + __os + "\n";
        return param;
    }
};
class Macbook : public Computer {
public:
    void setOS() override { __os = "Mac OS X12"; }
};
class Builder {
public:
    virtual void buildBoard(const std::string& board) = 0;
    virtual void buildDisplay(const std::string& display) = 0;
    virtual void buildOS() = 0;
    virtual std::shared_ptr<Computer> build() = 0;
};
class MacbookBuilder : public Builder {
private:
    std::shared_ptr<Computer> __computer; //
public:
    MacbookBuilder()
        : __computer(new Macbook()) { }
    virtual void buildBoard(const std::string& board) { __computer->setBoard(board); }
    virtual void buildDisplay(const std::string& display) { __computer->setDisplay(display); }
    virtual void buildOS() { __computer->setOS(); }
    std::shared_ptr<Computer> build() { return __computer; }
};
class Director {
private:
    std::shared_ptr<Builder> __builder; //
public:
    Director(Builder* builder)
        : __builder(builder) { }
    void construct(const std::string& board, const std::string& display) {
        __builder->buildBoard(board);
        __builder->buildDisplay(display);
        __builder->buildOS();
    }
};
int main() {
    Builder* builder = new MacbookBuilder();
    std::unique_ptr<Director> director(new Director(builder));
    director->construct("Huashuo", "Apple");
    std::shared_ptr<Computer> computer = builder->build();
    std::cout << computer->toString();
    return 0;
}
```

### 代理模式

这个代理的概念和网络里面的代理概念是相同的，这里不赘述了。