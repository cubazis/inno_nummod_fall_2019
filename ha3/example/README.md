### Сборка артефакта

1. Сборка силами Clion

- Edit configuration
- Add new configuration
- Cmake Application
- name: example
- target: example
- executable: example

- build
- запустили скрипт 
```
> python3.6 run.py cmake-build-debug
```

- Убедились что скрипт работает

---

2. Сборка силами `cmake`, `make`

- Собрали решение

```
> mkdir build && cd build
> cmake ..
> make
> cd ..
```

- запустили скрипт 
```
> python3.6 run.py build
```

- Убедились что скрипт работает

---

### Обо что пример

- О том как работать с blackbox.h
- О том как читать input
- О том как проверять что ваш артефакт отработал корректно силами python

---

### Ну и если совсем тяжко

Воспользуйтесь README.md уровнем выше `cd ..`