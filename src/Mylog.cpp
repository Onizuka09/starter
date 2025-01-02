#include <Arduino.h>
#include "Mylog.h"
#define LOG_COLOR_RESET  "\033[0m"    // Reset color
#define LOG_COLOR_BLUE   "\033[34m"  // Blue for INFO
#define LOG_COLOR_RED    "\033[31m"  // Red for ERROR
#define LOG_COLOR_GREEN  "\033[32m" // Green for DEBUG

static const char* getColor(LogLevel level) ;
static void printLogLevel(LogLevel level) ;
 void MyLog(LogLevel level, const char* message) {
    const char* color = getColor(level);
    Serial.print(color);
    printLogLevel(level);
    Serial.print(message);
    Serial.println(LOG_COLOR_RESET); // Reset color after printing
}
 void MyLogF(LogLevel level, const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    const char* color = getColor(level);
    Serial.print(color);
    printLogLevel(level);
    Serial.print(buffer);
    Serial.println(LOG_COLOR_RESET);
}


static const char* getColor(LogLevel level) {
    switch (level) {
        case INFO:  return LOG_COLOR_BLUE;
        case ERROR: return LOG_COLOR_RED;
        case DEBUG: return LOG_COLOR_GREEN;
        default:    return LOG_COLOR_RESET;
    }
}
static void printLogLevel(LogLevel level) {
    switch (level) {
        case INFO:  Serial.print("[INFO] "); break;
        case ERROR: Serial.print("[ERROR] "); break;
        case DEBUG: Serial.print("[DEBUG] "); break;
    }
}
