/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

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
#ifdef _easy_factory_mode
class FruitFactory {
public:
    static std::shared_ptr<Fruit> create(const std::string& name) {
        if (name == "apple")
            return std::make_shared<Apple>();
        else
            return std::make_shared<Banana>();
    }
};
#endif
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
#ifdef _easy_factory_mode
    std::shared_ptr<Fruit> fruit = FruitFactory::create("apple");
    fruit->name();
    fruit = FruitFactory::create("banana");
    fruit->name();
#endif
    std::shared_ptr<FruitFactory> ff(new AppleFactory());
    std::shared_ptr<Fruit> fruit = ff->create();
    fruit->name();
    ff.reset(new BananaFactory());
    fruit = ff->create();
    fruit->name();
    return 0;
}