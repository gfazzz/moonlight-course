# Mission Brief: First Tool

## Objective
Создать профессиональный инструмент `moonlight-decoder`, объединяющий функционал Episodes 01-03.

## Project Structure
```
artifacts/
├── decoder.c       # Main program
├── crypto.h        # Crypto module header
├── crypto.c        # Crypto implementation  
├── utils.h         # Utilities header
├── utils.c         # Utilities implementation
└── Makefile        # Build automation
```

## Tasks

### 1. Создать crypto модуль
**crypto.h:**
- [ ] Header guards
- [ ] Объявления функций:
  - `xor_decrypt()`
  - `xor_encrypt()`
  - `validate_message()`

**crypto.c:**
- [ ] Реализация всех функций
- [ ] Обработка ошибок

### 2. Создать utils модуль
**utils.h:**
- [ ] Объявления:
  - `read_file()`
  - `write_file()`
  - `print_stats()`

**utils.c:**
- [ ] Реализация работы с файлами
- [ ] Статистика (буквы, цифры, символы)

### 3. Главная программа
**decoder.c:**
- [ ] Парсинг аргументов командной строки
- [ ] Использование crypto и utils модулей
- [ ] Обработка ошибок
- [ ] Help сообщение

### 4. Makefile
- [ ] Компиляция всех модулей
- [ ] Линковка в единый исполняемый файл
- [ ] Targets: all, clean, help

## Success Criteria
- ✅ Компилируется без warnings
- ✅ Работает как единый инструмент
- ✅ Модули правильно разделены
- ✅ Файлы читаются/записываются корректно
- ✅ Обрабатывает аргументы командной строки
- ✅ Автотесты проходят

## Compile & Run
```bash
# С Makefile
make
./build/decoder -i test.dat -o result.txt -k 0x42

# Вручную
gcc -c crypto.c -o crypto.o
gcc -c utils.c -o utils.o
gcc -c decoder.c -o decoder.o
gcc *.o -o decoder

# Tests
make test
```

## Usage Examples
```bash
# Basic decryption
./decoder -i encrypted.dat -o decrypted.txt -k 0x42

# With verbose output
./decoder -i data.dat -o result.txt -k 0x42 -v

# Help
./decoder -h
```

## Time Estimate
- Crypto module: 45 min
- Utils module: 45 min
- Main program: 60 min
- Makefile: 20 min
- Testing: 30 min
- Total: ~3.5 hours

## Hints
<details>
<summary>Hint 1: Header guards</summary>

```c
#ifndef CRYPTO_H
#define CRYPTO_H

// Declarations...

#endif
```
</details>

<details>
<summary>Hint 2: File I/O</summary>

```c
FILE *fp = fopen(filename, "rb");
if (fp == NULL) {
    perror("Error opening file");
    return NULL;
}

// Read/write...

fclose(fp);
```
</details>

<details>
<summary>Hint 3: Command line args</summary>

```c
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            input_file = argv[++i];
        }
        // ...
    }
}
```
</details>

## What You'll Learn
- ✅ Multi-file projects
- ✅ Header files
- ✅ Modular architecture
- ✅ File I/O
- ✅ Command line parsing
- ✅ Professional tool development

---

**Build your first professional tool, agent!** 🛠️
