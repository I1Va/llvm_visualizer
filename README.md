![alt text](images/fact.png)


В этом репозитории представлены две утилиты:
1) llsda (LLVM Static and Dynamic Analysis)- для сбора статической и динамической информации
2) lldoter - утилита для генерации dot файла, состоящего из статической информации (flow/data граф) и динамической (количество исполнений базовых блоков, значения call, количество вызовов функций)

Как пользоваться:
1) git clone https://github.com/I1Va/llvm_visualizer_pass
2) ./build.sh
3) llsda/llsda -i c_examples/fact.cpp
4) ./fact.out 25
5) lldoter/build/lldoter static_info.bin dynamic_info.bin graph.dot
6) xdot ./graph.dot

Как работает llsda
Под капотом llsda собирается динамическая библиотека llvm пасса. 
Далее библиотека подключается при компиляции клангом профилируемого файла с помощью опции -fpass-plugin="${PLUGIN}"

Рассмотрим структуру исходников llvm_sda_pass




