# Pre-technical supplement

- [简体中文](./supplement.md)
- [English](./supplement-en.md)

***
- [Pre-technical supplement](#pre-technical-supplement)
  - [Undetermined parameters](#undetermined-parameters)
    - [Macros with indefinite parameters](#macros-with-indefinite-parameters)
    - [C-style undefined parameter function](#c-style-undefined-parameter-function)
    - [C++ style undefined parameter function](#c-style-undefined-parameter-function-1)
  - [Design Patterns](#design-patterns)
    - [Six Principles](#six-principles)
    - [Singleton pattern](#singleton-pattern)
    - [Factory Pattern](#factory-pattern)
    - [Builder pattern](#builder-pattern)
    - [Proxy mode](#proxy-mode)

All relevant code for this document is stored in `./demo`.

## Undetermined parameters

### Macros with indefinite parameters

Use as shown below.

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
> Here `##` means that if there is no undefined parameter, the preceding comma is omitted.

### C-style undefined parameter function

As shown in the code:

```c
// C-style undefined parameter function
void print_arg_num(int count, ...) {
    va_list ap;
    va_start(ap, count); // Get the starting address of the specified parameter, which is the starting address of the first parameter after getting the count parameter
    for (int i = 0; i < count; ++i) {
        int num = va_arg(ap, int); // Here we need to tell va_arg what type of data it is so that the compiler knows how much space to get on the stack.
        printf("param[%d]:%d\n", i, num);
    }
    va_end(ap); // Set the ap pointer to null
}
void test2() {
    print_arg_num(2, 666, 777);
    print_arg_num(5, 1, 2, 3, 4, 5);
}
```

Then let's simulate the implementation of the `printf` function.

```c
// Simulate printf
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

### C++ style undefined parameter function

The specific usage is as follows.

```cpp
void xprintf() {
    std::cout << std::endl;
}
template <typename T, typename... Args> // Args is equivalent to a parameter pack
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

## Design Patterns

> [!NOTE]
> Design patterns are a summary of the code development experience of predecessors and a series of routines for solving specific problems. It is not a grammatical rule, but a set of solutions to improve code reusability, maintainability, readability, robustness and security.

### Six Principles

**Single Responsibility Principle:**
- The responsibility of a class should be single, and a method should do only one thing. The division of responsibilities is clear, and each change is made to the smallest unit of enlargement or class

- Simply put, two completely different functions should not be placed in the same class. A class should be a group of highly related functions and data encapsulation

**Open Closed Principle:**
- Open to extension, closed to modification
- Usage suggestion: For changes to software entities, the final method is to use extension rather than modification

**Liskov Substitution Principle:**
- In layman's terms, as long as the parent class can appear, the child class can appear, and replacing it with the child class will not cause any errors or exceptions
- When inheriting a class, be sure to rewrite all methods in the parent class, especially the `protected` method of the parent class. The child class should try not to expose its own `public` method for external calls
- Usage suggestion: The child class must fully implement the parent class method, and the child class can have its own personality. When overriding or implementing the parent class method, the input parameters can be enlarged and the output can be reduced.

**Dependence Inversion Principle:**
- High-level modules should not depend on low-level modules, both should depend on their abstractions. Indivisible atomic logic is the assembly of low-level mode sub-logic into high-level modules
- Dependencies between modules occur through abstractions (interfaces), and concrete classes do not directly depend on each other
- Each class should have an abstract class as much as possible, and no class should be derived from a concrete class. Try not to override the methods of the base class. Use in conjunction with the Liskov Substitution Principle.

**Law of Demeter:**
- Minimize the interaction between objects to reduce the coupling between classes. An object should have the least knowledge of other objects. Clear requirements for low coupling of classes are put forward
- Only communicate with direct friends, and there is also a distance between friends. What is yours is yours (if a method is placed in this class, it neither increases the relationship between classes nor has a negative impact on this class, then place it in this class).

**Interface Segregation Principle:**
- The client should not rely on the interface it needs, and the dependencies between classes should be based on the smallest interface
- Usage suggestions: Keep the interface design as simple as possible, but do not expose interfaces that have no practical significance.

> [!TIP]
> **For each design principle, there is a corresponding note:**
> - The single responsibility principle tells us that the implementation class should have a single responsibility;
> - The Liskov Substitution Principle tells us not to destroy the inheritance system;
> - The Dependency Inversion Principle tells us to program to interfaces;
> - The Interface Segregation Principle tells us to be concise when designing interfaces;
> - The Law of Demeter tells us to reduce coupling;
> - The Open/Closed Principle is the general outline, telling us to be open to extensions and closed to modifications.

### Singleton pattern

A class can only create one object, that is, the singleton pattern. This design pattern can ensure that there is only one instance of the class in the system and provide a global access point to access it. The instance is shared by all program modules. For example, in a server program, the configuration information of the server is stored in a file. These configuration data are read uniformly by a singleton object, and then other objects in the service process obtain these configuration information through this singleton object. This method simplifies configuration management in complex environments.

**Singleton mode has two implementation modes: hungry mode and lazy mode**

For details, please see the blog: https://blog.csdn.net/Yu_Cblog/article/details/131787131


### Factory Pattern

See this part of the code: `demo/factory.cc`

The factory pattern is a creational design pattern that provides an optimal way to create objects. In the factory pattern, we do not expose the creation logic to the upper layer when creating objects, but use a common structure to point to the newly created object to achieve the separation of creation and use.

**The factory pattern can be divided into:**
- Simple factory pattern: The simple factory pattern is implemented by an instance. Assume that a factory object is created by type determination. A factory can produce fruit. When a customer needs to receive the product provided by the user, the factory will clearly tell the factory what kind of fruit to produce, and the factory will add the production method of the new product internally.

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

In fact, this is the meaning of the simple factory model. However, this structure has poor scalability. If you want to add a new fruit, you have to modify the factory code, which violates the open-closed principle.

**So the second model, the factory method model, is proposed:**
- Add multiple factories and multiple products in the simple factory model, and each product corresponds to a factory. Assuming that there are two products A and B now, two factories are opened. Users only know the factory name but not the specific product information. The factory does not need to receive the customer's product category and is only responsible for producing products.

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

This is the simple code of the factory method.

Every time a product is added to the factory method pattern, a specific product class and factory class need to be added, which will multiply the number of classes in the system and increase the coupling of the system to a certain extent.

**The third way: abstract factory pattern**

The factory method pattern solves the problem of too heavy responsibilities of the factory class in the simple factory pattern by introducing the factory hierarchy. However, since each factory in the factory method pattern only produces one type of product, it may lead to a large number of factory classes in the system, which will inevitably increase the system overhead. At this time, we can consider grouping some related products into a product family (a family of functionally related products in different product hierarchy structures) and produce them uniformly by the same factory. This is the basic idea of ​​the abstract factory pattern.


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


### Builder pattern

The Builder pattern is a creational design pattern that uses multiple simple objects to build a complex object step by step. It can separate the construction of a complex object from its representation and provide an optimal way to create objects.

The Builder pattern is mainly implemented based on 4 core classes:

- Abstract product class

- Specific product class

- Abstract `Builder` class: an abstract interface for each component required to create a product object

- Specific product `Builder` class: implements the abstract interface and builds each component

- Director `Director` class: unifies the assembly process and provides it to the caller, and constructs the product through the director

The sample code is as follows:
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

### Proxy mode

The concept of this proxy is the same as the concept of the proxy in the network, so I won’t go into details here.