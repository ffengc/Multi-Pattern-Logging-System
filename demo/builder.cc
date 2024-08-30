/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

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