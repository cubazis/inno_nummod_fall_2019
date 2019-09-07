### Cmake Options (ctrl+alt+s)
```
-DCMAKE_BUILD_TYPE=Debug
-DBUILD_TESTS=YES
```

### Сборка статической библиотеки

```
> mkdir build && cd build
> cmake ..
> sudo make && sudo make install
```

### При успешной сборке файлы лежат по путям

- **include** in `/usr/local/include`
- **lib** in `/usr/local/lib`
- **cmake configs** in `/usr/local/lib/cmake`

### Тест статической либы

проект **just_to_test**

- открываем
- читаем README.md
- билдим
- запускаем


## Гайд по запуску

1) Открываем решение в Visual studio
2) Билдим проект unum
3) Билдим проект с тестами
4) Запускаем тесты и вуаля! Все работает
