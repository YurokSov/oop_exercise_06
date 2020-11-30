// Шараковский Юрий. М8О-106Б-19
// 
// Создать шаблон динамической коллекцию, согласно варианту задания:
// 1.	Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr). Опционально использование std::unique_ptr;
// 2.	В качестве параметра шаблона коллекция должна принимать тип данных - фигуры;
// 3.	Реализовать forward_iterator по коллекции;
// 4.	Коллекция должны возвращать итераторы begin() и  end();
// 5.	Реализовать аллокатор, который выделяет фиксированный размер памяти (количество блоков памяти – является параметром шаблона аллокатора). Внутри аллокатор должен хранить указатель на используемый блок памяти и динамическую коллекцию указателей на свободные блоки. Динамическая коллекция должна соответствовать варианту задания (Динамический массив, Список, Стек, Очередь);
// 6.	Коллекция должна использовать аллокатор для выделения и освобождения памяти для своих элементов.
// 7.	Аллокатор должен быть совместим с контейнерами std::map и std::list(опционально – vector).
// 8.	Коллекция должна содержать метод доступа:
//      o	Стек – pop, push, top;
//      o	Очередь – pop, push, top;
//      o	Список, Динамический массив – доступ к элементу по оператору [];
// 9.	Реализовать программу, которая:
//      o	Позволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять в коллекцию;
//      o	Позволяет удалять элемент из коллекции по номеру элемента;
//      o	Выводит на экран введенные фигуры c помощью std::for_each;
//      o	Выводит на экран количество объектов, у которых площадь меньше заданной (с помощью  std::count_if);
// 
// Вариант 29: Ромб, Динамический массив, Динамический массив
//

#include <algorithm>
#include <iostream>
#include "allocator.hpp"
#include "rhombus.hpp"
#include "vector.hpp"
#include <vector>
#include <new>

int main() {

  int size = 10;
  std::unique_ptr<int[]> fact(new int[size]);

  lab::Vector<lab::rhombus<double>, lab::allocator<double>> v;
  lab::rhombus<double> r;

  std::cout << "1: insert <x>\n2: erase <x>\n3: print\n4: count area <x>" << std::endl;
  int option = 0;
  size_t x;
  while (std::cin >> option) {
    switch (option) {
    case 1:
      std::cin >> x;
      std::cout << "Enter rhombus (center and two adjacent vertices):" << std::endl;
      std::cin >> r;
      v.insert({ v, x }, r);
      break;
    case 2:
      std::cin >> x;
      v.erase({ v, x });
      break;
    case 3:
      std::for_each(v.begin(), v.end(), [](auto& r) { std::cout << r << "\n\tArea: " << r.getArea() << '\n'; });
      break;
    case 4:
      std::cin >> x;
      std::cout << "Total: " << std::count_if(v.begin(), v.end(), [x](auto& r) { return r.getArea() < x; }) << std::endl;
      break;
    default:
      std::cout << "I don\'t know that command!\n" << std::endl;
    }
  }
}