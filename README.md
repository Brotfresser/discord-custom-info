# Custom discord game-info

## Установка
#### Требования
* установленная пакетным менеджером библиотека [spdlog](https://github.com/gabime/spdlog/blob/v1.x/README.md) 
* динамический оконный менеджер hyprland под linux

#### Компиляция
```console
git clone https://github.com/Brotfresser/discord-custom-info.git
cd discord-custom-info && mkdir build && cd build
cmake .. && cmake --build .
```

## Принцип работы
  Каждые 100ms вызывает команду hyprctl activewindow -j, получая информацию о текущем окне, 
после чего в соответствии с правилами, определёнными в update_discord_mgr, через discord_game_sdk
в вашем профиле отображается нужная информация
