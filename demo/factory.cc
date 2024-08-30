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

class Animal {
public:
    virtual void name() = 0;
};
class Lamp : public Animal {
public:
    void name() override { std::cout << "I am a Lamp" << std::endl; }
};
class Dog : public Animal {
public:
    void name() override { std::cout << "I am a Dog" << std::endl; }
};

#if _easy_factory_mode
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

#if _factory_method_mode
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
#endif

int main() {
#ifdef _easy_factory_mode
    std::shared_ptr<Fruit> fruit = FruitFactory::create("apple");
    fruit->name();
    fruit = FruitFactory::create("banana");
    fruit->name();
#endif
#if _factory_method_mode
    std::shared_ptr<FruitFactory> ff(new AppleFactory());
    std::shared_ptr<Fruit> fruit = ff->create();
    fruit->name();
    ff.reset(new BananaFactory());
    fruit = ff->create();
    fruit->name();
#endif
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
};