

# PZEM-004T Systemd service linux.
## Monitoring & log
#### `Version: 1.0b`

Сервис читает данные по ModBus с PZEM-004T v4.0 анализирует полученные данные и записывает в лог.
Обрабатывает пороговые значения для определения повышенного или пониженного напряжения, тока или частоты. 
>
Примерно один опрос и анализ занимает от 100 до 200мс, что позволяет опрашивать до 5 раз в секунду.
>
Создает FIFO в tmp, для других сервисов. В отличии от лога, данные туда передаються при каждом изменении.
>
Может быть запущенно несколько экземпляров, например для разных фаз.
>
Все параметры указываються в файле конфигурации.
>


## Installation / Установка

```bash
  # Установить необходимую библиотеку
    sudo apt-get install libmodbus-dev

  # Прейти в каталог с проектом
    cd pzem-monitor

  # Собрать программу и создать шаблоны
    make

  # Только создать шаблоны
    make templates

  # Установить в систему
    sudo make install

  # Полная очистка (включая шаблоны)
    make allclean

  # Обычная очистка (только объектные файлы)
    make clean

  # Помощь
    make help
```
    
## Config / Конфигурация
```sh
# PZEM-004T Default Configuration

# Serial port settings
tty_port = /dev/ttyS1
baudrate = 9600
# Адрес MODBUS PZEM
slave_addr = 1
# Период опроса в ms
poll_interval_ms = 500

# Logging settings
log_dir = /var/log/pzem

# Sensitivity settings
# На сколько должны измениться данные 
# от предыдушего замера, чтобы зафиксировать изменения
voltage_sensitivity = 0.1
current_sensitivity = 0.001
frequency_sensitivity = 0.1
power_sensitivity = 1.0

# Voltage thresholds (0 = disabled)
voltage_high_alarm = 245
voltage_high_warning = 240
voltage_low_warning = 210
voltage_low_alarm = 200

# Current thresholds (0 = disabled)
current_high_alarm = 0
current_high_warning = 0
current_low_warning = 0
current_low_alarm = 0

# Frequency thresholds (0 = disabled)
frequency_high_alarm = 52
frequency_high_warning = 51
frequency_low_warning = 49
frequency_low_alarm = 48
```
## Usage / Использование


#### Запустить сервис c default
```sh
sudo systemctl start pzem@default
```
#### Создать дополнительную конфигурацию
```sh
sudo cp /etc/pzem/default.conf /etc/pzem/garage.conf
```
отредактировать garage.conf и
Запустить сервис c garage
```sh
sudo systemctl start pzem@garage
```
#### Просмотр логов 
(при необходимости default заменить на свою конфигурацию)
```sh
sudo journalctl -u pzem@default -f
```
#### Полностью удалить
Перейти в каталог с проектом
```sh
sudo make uninstall
```
## Authors

- [@AKA_ZejroN](https://github.com/akarnaukh)

