/**
 * @file Mylog.h
 * @brief Logging module for debugging and informational messages.
 *
 * This module provides functions for logging messages with different logging levels.
 * It supports formatted and plain-text logging.
 *
 * @author Moktar SELLAMI
 * @date 2025-02-16
 */
#ifndef MY_LOG_H_
#define MY_LOG_H_



#include <Arduino.h>

/**
 * @enum LogLevel
 * @brief Defines different levels of logging severity.
 */
typedef enum {
    INFO,   /**< Informational messages */
    ERROR,  /**< Error messages */
    DEBUG   /**< Debugging messages */
} LogLevel;

/**
 * @brief Logs a message with a given log level.
 *
 * @param level The severity level of the log message.
 * @param message The message to log.
 */
void MyLog(LogLevel level, const char* message);

/**
 * @brief Logs a formatted message with a given log level.
 *
 * This function supports formatted output similar to printf.
 *
 * @param level The severity level of the log message.
 * @param format The format string (printf-style).
 * @param ... Additional arguments for formatting.
 */
void MyLogF(LogLevel level, const char* format, ...);

#endif // MY_LOG_H_
