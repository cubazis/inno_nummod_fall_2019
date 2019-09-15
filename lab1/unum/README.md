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
