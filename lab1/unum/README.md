### Cmake Options (ctrl+alt+s)
```
-DCMAKE_BUILD_TYPE=Debug
-DBUILD_TESTS=YES
```

### Сборка статической библиотеки
Чтобы собрать и запустить:
```
> mkdir build && cd build
> cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=YES  ..
> sudo make
```