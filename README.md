# C Cli Lib

## Пост Скриптум

Зис лайбрери из креэйтэд ту мэйк оур лайвс лесс мизерабл бай ремувин нессесарити оф мейкин СЛИ бай оурселвес фром скрэтч 

Ю кэн си зэ экзампле оф юзаж оф зис лайбрери [хере](examples/calculator/main.c)

## Инсталлейшин

### Пре реквайрементс

* Debian/Ubuntu: ``sudo apt install libreadline-dev``

* macOS: ``brew install readline``

* Arch Linux: ``sudo pacman -S readline``

* Fedora: ``sudo dnf install readline-devel``

* Windows: край эбоут ит

### Билд(через СиМейк)

``git submodule add https://github.com/stepzak/c_cli_lib.git <extrnal>/c_cli_lib``

``#CMakeLists.txt``
```
add_subdirectory(<external>/c_cli_lib)

add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE c_cli_lib)
```
