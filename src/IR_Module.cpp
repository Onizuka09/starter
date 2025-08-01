#include <IRremote.hpp>
#include "IR_Module.h"

//  /// Defines the IR send pin.
//  #define IR_SEND_PIN
 
 /// Enables decoding of NEC protocol (includes Apple and Onkyo). Comment to enable all protocols.
 #define DECODE_NEC
 
 /// Defines the time per tick in microseconds.
 #define _MICROS_PER_TICK 50
 
 /// Uses hardware PWM for sending carrier signal generation.
 #define SEND_PWM_BY_TIMER
 


String file_names[] = {"/settings.txt", "/settings.json"};

void IRHandler::setRecievedData(MyIRData &irData)
{
    receivedData = irData;
}
uint8_t IRHandler::getTVStatus(uint16_t _delay)
{

    delay(_delay);
    uint8_t tmp = digitalRead(_tvpin);
    MyLogF(INFO, "Reading TV status %d  ", tmp);
    if (tmp == 0)
    {
        MyLog(INFO, "THE TV IS ON   ");
    }
    else
    {
        MyLog(INFO, "THE TV IS OFF  ");
    }

    return tmp;
}

void IRHandler::PrintMyIRData(MyIRData &data)
{
    Serial.println("------ MyIRData ------");
    Serial.print("Data Stored: ");
    Serial.println(data.data_stored ? "Yes" : "No");

    Serial.print("Protocol: ");
    Serial.println(static_cast<int>(data.protocol));

    Serial.print("Number of Bits: ");
    Serial.println(data.numberOfBits);

    Serial.print("IR Address: ");
    Serial.println(data.IR_Adress);

    Serial.print("IR Command: ");
    Serial.println(data.IR_Command);

    Serial.println("Decoded Raw Data Array:");
    for (int i = 0; i < RAW_DATA_ARRAY_SIZE; i++)
    {
        Serial.print("  [");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(data.DecodedRawDataArray[i]);
    }

    Serial.println("Distance Width Timing Info:");
    Serial.print("  Header Mark Micros: ");
    Serial.println(data.timingInfo.HeaderMarkMicros);

    Serial.print("  Header Space Micros: ");
    Serial.println(data.timingInfo.HeaderSpaceMicros);

    Serial.print("  One Mark Micros: ");
    Serial.println(data.timingInfo.OneMarkMicros);

    Serial.print("  One Space Micros: ");
    Serial.println(data.timingInfo.OneSpaceMicros);

    Serial.print("  Zero Mark Micros: ");
    Serial.println(data.timingInfo.ZeroMarkMicros);

    Serial.print("  Zero Space Micros: ");
    Serial.println(data.timingInfo.ZeroSpaceMicros);

    Serial.println("----------------------");
}
void IRHandler::dump_data_stored(const char *filename)
{

    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (!file.available())
    {
        Serial.println(" no available content Content:");
    }
    Serial.println("Content:");
    while (file.available())
    {
        Serial.write(file.read());
        Serial.flush();
    }
    Serial.println("\n---------------------");
    file.close();
}
void IRHandler::init_littleFS()
{

    if (!LittleFS.begin())
    {
        MyLog(ERROR, "Failed to initialize LittleFS");
        return;
    }
    else
    {
        MyLog(INFO, "initialize LittleFS");
    }
}
IRHandler::IRHandler(uint8_t receivePin, uint8_t _sendPin, uint8_t tvpin)
    : receivePin(receivePin), _sendPin(_sendPin), _tvpin(tvpin)
{
    // Initialize pins
    // pinMode(receivePin, INPUT);
    // pinMode(sendPin, OUTPUT);
}

// Setup: Begin IRReceiver and IRSender
void IRHandler::begin()
{
    Serial.begin(115200);
    MyIrReceiver.begin(receivePin, ENABLE_LED_FEEDBACK); // Initialize receiver
    MyIrSender.begin(_sendPin);                          // Initialize sender
    pinMode(_tvpin, INPUT);
    Serial.println("IR Receiver and Sender Initialized");
}

// Receive IR signal and identify protocol
bool IRHandler::receiveIR(bool saveData)
{
    MyIrReceiver.start();
    while (true)
    {
        if (MyIrReceiver.decode())
        {
            MyIrReceiver.printIRResultShort(&Serial);
            Serial.println();
            MyIrReceiver.printIRResultRawFormatted(&Serial);
            Serial.println();
            MyIRData irData;
            irData.numberOfBits = MyIrReceiver.decodedIRData.numberOfBits;
            irData.protocol = identifyProtocol(irData);
            // if ( )
            // Store the raw data based on the protocol
            if (irData.protocol == PULSE_DISTANCE)
            {
                memcpy(irData.DecodedRawDataArray, MyIrReceiver.decodedIRData.decodedRawDataArray, sizeof(irData.DecodedRawDataArray));
                irData.timingInfo = MyIrReceiver.decodedIRData.DistanceWidthTimingInfo;
                // irData.data_Read= true;
            }
            else
            {
                irData.IR_Adress = MyIrReceiver.decodedIRData.address;
                irData.IR_Command = MyIrReceiver.decodedIRData.command;
                // irData.data_Read= true;
                // Handle Samsung or LG protocol
                // Add specific code to handle Samsung and LG data formats
            }
            if (saveData)
            {
                storeData(irData);
                MyLog(INFO, "DATA Stored ...");
            }
            MyIrReceiver.resume(); // Ready to receive next data
            MyIrReceiver.stop();
            return true;
        }
    }
}

// Identify the protocol type of the received IR signal
decode_type_t IRHandler::identifyProtocol(MyIRData &irData)
{
    // Here you can compare against known protocols (e.g., Pulse Distance, Samsung, LG)
    // Serial.println(MyIrReceiver.decodedIRData.protocol)
    if (MyIrReceiver.decodedIRData.protocol == PULSE_DISTANCE)
    {
        return PULSE_DISTANCE;
    }
    else if (MyIrReceiver.decodedIRData.protocol == SAMSUNG)
    {
        return SAMSUNG;
    }
    else if (MyIrReceiver.decodedIRData.protocol == LG)
    {
        return LG;
    }
    else if (MyIrReceiver.decodedIRData.protocol == NEC)
    {
        return NEC;
    }
    else
    {
        return UNKNOWN;
    }
}

// Store received IR data in memory
void IRHandler::storeData(MyIRData &irData)
{
    // Store IR data in receivedData (you can expand this to use a more advanced storage mechanism)
    receivedData = irData;
    Serial.println("Stored new IR data.");
    receivedData.data_stored = true;
    storeMyIRDataJSON(f, receivedData);
}

// Send the stored IR data based on protocol
void IRHandler::sendIR()
{
    MyIrReceiver.stop();
    Serial.println("Seding data ");

    if (receivedData.protocol == PULSE_DISTANCE)
    {
        Serial.println("SENDING PULSE DISTANCE PROTOCOLE ");
        // MyIrSender.sendPulseDistanceWidthFromArray(38, &sDistanceWidthTimingInfo, &sDecodedRawDataArray[0], sNumberOfBits,PROTOCOL_IS_LSB_FIRST, 100, 0);
        MyIrSender.sendPulseDistanceWidthFromArray(38, &receivedData.timingInfo, &receivedData.DecodedRawDataArray[0], receivedData.numberOfBits, PROTOCOL_IS_LSB_FIRST, 100, 0);
    }
    else if (receivedData.protocol == NEC)
    {
        MyIrSender.sendNEC(receivedData.IR_Adress, receivedData.IR_Command, 38);
    }
    else if (receivedData.protocol == SAMSUNG)
    {
        Serial.println("SENDING SAMSUNG PROTOCOLE ");
        MyIrSender.sendSamsung(receivedData.IR_Adress, receivedData.IR_Command, 38);
        // MyIrSender.sendRaw(MyIrReceiver.decodedIRData.rawDataPtr->rawbuf,MyIrReceiver.decodedIRData.rawlen,38);
        // MyIrSender.write(MyIrReceiver.decodedIRData);
        // Send Samsung IR command (implement logic for Samsung)
    }
    else if (receivedData.protocol == LG)
    {
        Serial.println("SENDING PULSE LG ");
        MyIrSender.sendLG(receivedData.IR_Adress, receivedData.IR_Command, 38);

        // Send LG IR command (implement logic for LG)
    }
    else
    {
        Serial.println("Unknown protocol, cannot send, Protocole not supported .");
        // Serial.printf()
        MyIrReceiver.printIRResultShort(&Serial);
    }
    MyIrReceiver.start();
}

// Play back stored IR data (e.g., when button is pressed)
void IRHandler::playBackData(const MyIRData &irData)
{
    sendIR();
}
void IRHandler::clear_file(const char *filename)
{
    MyIRData d = {0};
    d.data_stored = false;
    storeMyIRDataJSON(filename, d);
}
void IRHandler::storeMyIRDataJSON(const char *filename, const MyIRData &data)
{

    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    // Create a JSON object
    StaticJsonDocument<512> doc;
    doc["stored"] = data.data_stored;
    doc["protocol"] = (int)data.protocol;
    doc["numberOfBits"] = data.numberOfBits;
    doc["IR_Adress"] = (uint32_t)data.IR_Adress;
    doc["IR_Command"] = (uint32_t)data.IR_Command;

    // Add raw_data as an array

    for (int i = 0; i < RAW_DATA_ARRAY_SIZE; i++)
    {
        Serial.println(data.DecodedRawDataArray[i]);
    }
    JsonArray array = doc.createNestedArray("raw_data");

    for (uint64_t number : data.DecodedRawDataArray)
    { // Assuming DecodedRawDataArray is iterable

        array.add(number);
    }

    // Add DistanceWidthTimingInfoStruct as a nested object
    JsonObject timingInfoJson = doc.createNestedObject("timing_info");
    timingInfoJson["HeaderMarkMicros"] = data.timingInfo.HeaderMarkMicros;
    timingInfoJson["HeaderSpaceMicros"] = data.timingInfo.HeaderSpaceMicros;
    timingInfoJson["OneMarkMicros"] = data.timingInfo.OneMarkMicros;
    timingInfoJson["OneSpaceMicros"] = data.timingInfo.OneSpaceMicros;
    timingInfoJson["ZeroMarkMicros"] = data.timingInfo.ZeroMarkMicros;
    timingInfoJson["ZeroSpaceMicros"] = data.timingInfo.ZeroSpaceMicros;

    // Serialize the JSON object
    serializeJsonPretty(doc, Serial);

    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Failed to write JSON data");
    }
    else
    {
        Serial.println("JSON data stored successfully.");
    }

    file.close();
}

bool IRHandler::IsIRDataavailable(const char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        file.close();
        return false;
    }

    file.close();
    // Populate the MyIRData struct
    bool x = doc["stored"].as<bool>();
    if (x)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Read JSON data and deserialize it to MyIRData
bool IRHandler::DumpMyIRDataJSON(const char *filename)
{

    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    String s = "";
    Serial.println("- read from file:");

    // Deserialize the JSON object
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        file.close();
        return false;
    }
    serializeJsonPretty(doc, Serial);

    // Populate the structur

    file.close();
    Serial.println("JSON data read successfully.");
    return true;
}

bool IRHandler::readMyIRDataJSON(const char *filename, MyIRData &data)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        file.close();
        return false;
    }

    // Populate the MyIRData struct
    data.data_stored = doc["stored"].as<bool>();
    if (!data.data_stored)
    {
        Serial.print("NO  JSON data stored ");
        file.close();
        return false;
    }
    data.protocol = static_cast<decode_type_t>(doc["protocol"].as<int>());
    data.numberOfBits = doc["numberOfBits"].as<uint8_t>();
    data.IR_Adress = doc["IR_Adress"].as<uint16_t>();
    data.IR_Command = doc["IR_Command"].as<uint16_t>();

    // Parse raw_data array
    JsonArray rawDataArray = doc["raw_data"];
    int i = 0;
    for (uint64_t value : rawDataArray)
    {
        if (i < RAW_DATA_ARRAY_SIZE)
        {
            data.DecodedRawDataArray[i++] = value;
        }
        else
        {
            break; // Prevent overflow
        }
    }

    // Parse timing_info object
    JsonObject timingInfoJson = doc["timing_info"];
    data.timingInfo.HeaderMarkMicros = timingInfoJson["HeaderMarkMicros"].as<uint16_t>();
    data.timingInfo.HeaderSpaceMicros = timingInfoJson["HeaderSpaceMicros"].as<uint16_t>();
    data.timingInfo.OneMarkMicros = timingInfoJson["OneMarkMicros"].as<uint16_t>();
    data.timingInfo.OneSpaceMicros = timingInfoJson["OneSpaceMicros"].as<uint16_t>();
    data.timingInfo.ZeroMarkMicros = timingInfoJson["ZeroMarkMicros"].as<uint16_t>();
    data.timingInfo.ZeroSpaceMicros = timingInfoJson["ZeroSpaceMicros"].as<uint16_t>();

    Serial.println("Data successfully read from JSON file:");
    serializeJsonPretty(doc, Serial);

    file.close();
    return true;
}
