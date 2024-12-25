#ifndef IR_UTILS_H 
#define IR_UTILS_H 
// #include<Arduino.h>



// Function prototypes
bool IR_COMMAND_EXISTS(); 
void IR_RECEIVE_COMMAND(bool store_data) ;
// void IR_SEND_COMMAND(int8_t* buff,size_t s); 
void IR_SEND_COMMAND(uint8_t* buff,size_t s );
void setupReceiver();
void setupSender();
void Test_IR_Receive();
void test_receive_and_send_IR();

#endif 