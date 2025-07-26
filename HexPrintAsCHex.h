#ifndef HEX_PRINT_UTILS_H
#define HEX_PRINT_UTILS_H

#include <Arduino.h> // 包含 Arduino 核心库，用于 Serial 通信
#include <string.h>  // 包含用于 strlen() 的字符串操作函数

void printAsCHex(const char* str) {
    if (str == nullptr) { // 处理空指针情况
        Serial.print("(null)");
        return;
    }

    Serial.print("\""); // 打印字符串的起始双引号

    // 遍历字符串中的每一个字节
    for (size_t i = 0; i < strlen(str); ++i) {
        Serial.print("\\x"); // 打印转义前缀 "\x"

        // 将当前字节转换为无符号字符，以确保正确处理负值（char 可能是 signed）
        unsigned char byte = (unsigned char)str[i];

        // 如果字节值小于 16 (0x10)，需要在前面补一个 '0'，确保是两位十六进制数
        if (byte < 0x10) {
            Serial.print("0");
        }
        // 以十六进制格式打印字节
        Serial.print(byte, HEX);
    }

    Serial.println("\""); // 打印字符串的结束双引号和换行符
}

#endif // HEX_PRINT_UTILS_H
