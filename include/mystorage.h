/**
 * @file mystorage.h
 * @brief Storage module for handling file operations using LittleFS.
 *
 * This module provides functions to interact with the file system, including listing directories, 
 * dumping file contents, and basic file operations.
 *
 * @author Moktar SELLAMI
 * @date 2025-02-16
 */
#ifndef MY_STORAGE_H_ 
#define MY_STORAGE_H_



#include <LittleFS.h>
#include <Arduino.h>
#include <FS.h>

/**
 * @brief Lists the contents of a directory.
 *
 * This function prints the files and directories present in the specified directory.
 *
 * @param fs The file system instance (e.g., LittleFS).
 * @param dirname The path of the directory to list.
 * @param levels The depth level for recursive listing.
 */
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

/**
 * @brief Dumps the contents of a file to the serial output.
 *
 * This function reads the specified file and prints its content for debugging purposes.
 *
 * @param fs The file system instance (e.g., LittleFS).
 * @param path The file path to dump.
 */
void Dump_file(fs::FS &fs, const char *path);

#endif // MY_STORAGE_H_
