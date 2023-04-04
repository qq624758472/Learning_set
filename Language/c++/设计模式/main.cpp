/*
 * @Description: 工厂模式
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-04-03 17:06:07
 * @LastEditors: ruog__
 * @LastEditTime: 2023-04-04 09:40:35
 */
#include <iostream>

using namespace std;

// 产品接口
class Animal
{
public:
    //virtual void makeSound() = 0;
    virtual void makeSound() { cout << "咕咕咕" << endl; };
};

// 产品实现类，狗
class Dog : public Animal
{
public:
    void makeSound() override { cout << "汪汪汪" << endl; }
};

// 产品实现类，猫
class Cat : public Animal
{
public:
    void makeSound() override { cout << "喵喵喵" << endl; }
};

// 工厂接口
class AnimalFactory
{
public:
    virtual Animal *createAnimal() = 0;
};

// 具体工厂实现类，狗工厂
class DogFactory : public AnimalFactory
{
public:
    Animal *createAnimal() override { return new Dog(); }
};

// 具体工厂实现类，猫工厂
class CatFactory : public AnimalFactory
{
public:
    Animal *createAnimal() override { return new Cat(); }
};

// 客户端
int main()
{
    AnimalFactory *factory = new DogFactory();
    Animal *animal = factory->createAnimal();
    animal->makeSound();

    delete factory;
    delete animal;

    factory = new CatFactory();
    animal = factory->createAnimal();
    animal->makeSound();

    delete factory;
    delete animal;

    //======
    Dog d;
    Animal *an = new Dog();
    an->makeSound();
    an = new Cat();
    an->makeSound();

    an = &d;
    an->makeSound();

    return 0;
}
