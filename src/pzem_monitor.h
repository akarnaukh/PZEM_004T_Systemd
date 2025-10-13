#ifndef PZEM_MONITOR_H
#define PZEM_MONITOR_H

#include <stdio.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>
#include <fcntl.h>      // для O_NONBLOCK

#define PZEM_FIFO_PATH "/tmp/pzem_data_%s"  // %s будет заменен на config_name

// Структура для хранения конфигурации
typedef struct {
    char tty_port[64];
    int baudrate;
    int slave_addr;
    int poll_interval_ms;
    char log_dir[256];
    int log_buffer_size;  // Добавляем размер буфера
    
    // Чувствительность изменений
    float voltage_sensitivity;
    float current_sensitivity;
    float frequency_sensitivity;
    float power_sensitivity;
    
    // Пороги напряжения
    float voltage_high_alarm;
    float voltage_high_warning;
    float voltage_low_warning;
    float voltage_low_alarm;
    
    // Пороги тока
    float current_high_alarm;
    float current_high_warning;
    float current_low_warning;
    float current_low_alarm;
    
    // Пороги частоты
    float frequency_high_alarm;
    float frequency_high_warning;
    float frequency_low_warning;
    float frequency_low_alarm;
} pzem_config_t;

// Структура для хранения данных
typedef struct {
    float voltage;
    float current;
    float frequency;
    float power;  // Добавляем мощность
    int status;
    int first_read;
    
    // Состояния порогов
    char voltage_state;  // 'H' - высокое, 'L' - низкое, 'N' - норма
    char current_state;  // 'H' - высокое, 'L' - низкое, 'N' - норма  
    char frequency_state; // 'H' - высокое, 'L' - низкое, 'N' - норма
} pzem_data_t;

// Структура для буферизации логов
typedef struct {
    char **buffer;
    int size;
    int capacity;
    int write_index;
    char log_dir[256];
    char config_name[64];
} log_buffer_t;

// Глобальные переменные
extern modbus_t *ctx;
extern volatile int keep_running;
extern log_buffer_t log_buffer;
extern pzem_config_t global_config;
extern char *service_name;
extern char config_name[64];

// Функции конфигурации
int load_config(const char *config_file, pzem_config_t *config);
int create_directory_if_not_exists(const char *path);
int validate_config(const pzem_config_t *config);
void extract_config_name(const char *config_path);

// Функции для FIFO
int init_data_fifo(const char *fifo_path);
int write_to_fifo(const char *fifo_path, const char *data);
void cleanup_fifo(const char *fifo_path);

// Функции работы с логами
int init_log_buffer(log_buffer_t *buffer, int initial_capacity, const char *log_dir);
int add_to_log_buffer(log_buffer_t *buffer, const char *log_entry);
int flush_log_buffer(log_buffer_t *buffer);
void free_log_buffer(log_buffer_t *buffer);
void get_current_date(char *date_str, size_t size);
void get_current_time(char *time_str, size_t size);
void get_log_file_path(char *path, size_t size, const char *log_dir);
void prepare_log_entry(char *log_entry, size_t size, const pzem_data_t *data);
int should_flush_buffer(const log_buffer_t *buffer);

// Функции Modbus
int init_modbus_connection(const pzem_config_t *config);
int read_pzem_data(pzem_data_t *data);
void cleanup(void);
void safe_reconnect(const pzem_config_t *config);

// Функции обработки данных
int values_changed(const pzem_data_t *current, const pzem_data_t *previous, const pzem_config_t *config);
void update_threshold_states(pzem_data_t *data, const pzem_config_t *config);
int threshold_states_changed(const pzem_data_t *current, const pzem_data_t *previous);

// Сигналы
void signal_handler(int sig);

#endif