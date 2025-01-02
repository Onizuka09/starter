// /*
//  * =====================================================================================
//  *
//  *       Filename:  JsonFileManager.cpp
//  *
//  *    Description:  
//  *
//  *        Version:  1.0
//  *        Created:  04/17/2024 05:50:39 AM
//  *       Revision:  none
//  *       Compiler:  gcc
//  *
//  *         Author:  YOUR NAME (), 
//  *   Organization:  
//  *
//  * =====================================================================================
//  */

// #include "mystorage.h"


// String file_names[] = {"/settings.txt"};



// void init_littlefs(){

// 	  if (!LittleFS.begin()) {
//     Serial.println("failed to init littleFS");
//     return;
//   }

// }


// void stop_littlefs(){
//      LittleFS.end(); 
// }
// bool isDataAvailable(Files f) {
//     File file = LittleFS.open(file_names[f], "r");
//     if (!file) {
//         Serial.println("Failed to open file for reading.");
//         return false;
//     }

//     uint8_t flag;
//     file.read(&flag, 1);
//     file.close();

//     return flag == 1;
// }
// // String readFile( Files f) {
// //   File file = LittleFS.open(file_names[f], "r");
// //   if (!file) {
// //     Serial.println("Failed to open file for reading");
// //     file.close();
// //     return "";
// //   }

// //   String jdata = file.readString();
// //   file.close();

// //  Serial.printf("Reading Data %s\n\r",jdata);

// //   return jdata;
// // }

// bool write_file(uint8_t* data, size_t size, Files f) {
//   File file = LittleFS.open(file_names[f], FILE_WRITE);
//   if (!file) {
//     Serial.println("Failed to open file for reading");
//     file.close();
//     return false;
//   }

//     uint8_t flag = 1; // 1 indicates data exists
//     file.write(&flag, 1);

//     // Write the buffer size (optional for dynamic sizes)
//     file.write((uint8_t*)&size, sizeof(size));

//     // Write the actual data
//     file.write(data, size);
//     file.close();
//     Serial.println("Data saved successfully.");
//   return false;
// }

// bool readData(uint8_t* buffer,size_t* size,Files f) {
//     File file = LittleFS.open(file_names[f], "r");
//     if (!file) {
//         Serial.println("Failed to open file for reading.");
//         return false;
//     }
//   // size_t fileSize;
//     // Read the flag (skip)
//     uint8_t flag;
//     file.read(&flag, 1);

//     // Read the buffer size
//     // size_t size;
//     file.read((uint8_t*)size, sizeof(*size));

//     // Read the actual data
//     file.read(buffer,*size);
//     file.close();
//     return true;
// }

// // bool readData(uint16_t* buffer, size_t* size, Files f) {
// //     File file = LittleFS.open(file_names[f], "r");
// //     if (!file) {
// //         Serial.println("Failed to open file for reading.");
// //         return false;
// //     }

// //     // Read the flag (skip it for now, assuming it is 1 byte)
// //     uint8_t flag;
// //     if (file.read(&flag, 1) != 1) {
// //         Serial.println("Failed to read flag.");
// //         file.close();
// //         return false;
// //     }

// //     // Read the buffer size
// //     if (file.read((uint8_t*)size, sizeof(*size)) != sizeof(*size)) {
// //         Serial.println("Failed to read buffer size.");
// //         file.close();
// //         return false;
// //     }

// //     // Read the actual data
// //     if (file.read((uint8_t*)buffer, *size * sizeof(uint16_t)) != *size * sizeof(uint16_t)) {
// //         Serial.println("Failed to read buffer data.");
// //         file.close();
// //         return false;
// //     }

// //     file.close();
// //     return true;
// // }


// void dump_file(Files f ) {
//   Serial.println("Dumping data...");

//   uint8_t temp[255] = {0};
//   size_t size = sizeof(temp); // Ensure size matches buffer capacity

//   if (!readData(temp, &size,settings)) {
//     Serial.println("Failed to read data.");
//     return;
//   }
//   Serial.println("Data available");

//   int j = 0;
//   for (size_t i = 0; i < 51; i++) {
//     Serial.print("  ");
//     Serial.print(temp[i]*50);
//     Serial.print("  ");
//     j++;
//     if (j >= 4) {
//       j = 0;
//       Serial.println();
//     }
//   }

//   Serial.println(); // Optional: Add newline after dumping file content
 
// }

// void resetFile(Files f) {
//     File file = LittleFS.open(file_names[f], "w"); // Open in write mode to clear content
//     if (file) {
//         file.close(); // Closing immediately clears the file content
//         Serial.println("File reset successfully.");
//     } else {
//         Serial.println("Failed to reset the file.");
//     }
// }


