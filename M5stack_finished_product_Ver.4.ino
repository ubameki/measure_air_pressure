#include <M5CoreS3.h>
#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <bsec2.h>
#include <M5Unified.h>
#include <esp_sntp.h>

#define WIFI_SSID     "TP-Link_BBDD"
#define WIFI_PASSWORD "71340555"
#define NTP_TIMEZONE  "JST-9"
#define NTP_SERVER1   "0.pool.ntp.org"
#define NTP_SERVER2   "1.pool.ntp.org"
#define NTP_SERVER3   "2.pool.ntp.org"

#define SD_SPI_SCK_PIN  36
#define SD_SPI_MISO_PIN 35
#define SD_SPI_MOSI_PIN 37
#define SD_SPI_CS_PIN   4

void checkBsecStatus(Bsec2 bsec);
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

File csvFile;
Bsec2 envSensor;
float lastPressure = 0.0;
bool newDataAvailable = false;
char fileName[50];

void setup(void) {
    M5.begin();
    M5.Lcd.clear(); 
    Serial.begin(115200);
    Wire.begin(2, 1);
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);

    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        Serial.println("SD card initialization failed!");
        while (1);
    }

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("\nWiFi Connected.");
    configTzTime(NTP_TIMEZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);

    while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println("\nNTP Connected.");

    if (!envSensor.begin(BME68X_I2C_ADDR_HIGH, Wire)) {
        checkBsecStatus(envSensor);
    }

    bsecSensor sensorList[] = {BSEC_OUTPUT_RAW_PRESSURE};
    if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_ULP)) {
        checkBsecStatus(envSensor);
    }
    envSensor.attachCallback(newDataCallback);

    if (!SD.exists("/Data")) {
        SD.mkdir("/Data");
    }

    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);
    strftime(fileName, sizeof(fileName), "/Data/pressure_log_%Y%m%d_%H%M%S.csv", tm_info);

    Serial.printf("Logging to file: %s\n", fileName);

    csvFile = SD.open(fileName, FILE_WRITE);
    if (csvFile) {
        csvFile.println("Date,Time,Pressure(hPa)");
        csvFile.close();
    }
}

void loop(void) {
    if (!envSensor.run()) {
        checkBsecStatus(envSensor);
    }

    if (newDataAvailable) {
        time_t now = time(nullptr);
        struct tm *tm_info = localtime(&now);
        char dateStr[11], timeStr[9];
        strftime(dateStr, sizeof(dateStr), "%Y/%m/%d", tm_info);
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", tm_info);

        M5.Lcd.clear(); 
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("%s %s\nPressure: %.2f hPa", dateStr, timeStr, lastPressure);

        csvFile = SD.open(fileName, FILE_APPEND);
        if (csvFile) {
            csvFile.printf("%s,%s,%.2f\n", dateStr, timeStr, lastPressure);
            csvFile.close();
            newDataAvailable = false;
        }
    }
    delay(1000);
}

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec) {
    if (!outputs.nOutputs) {
        return;
    }

    for (uint8_t i = 0; i < outputs.nOutputs; i++) {
        const bsecData output = outputs.output[i];
        if (output.sensor_id == BSEC_OUTPUT_RAW_PRESSURE) {
            lastPressure = output.signal;
            newDataAvailable = true;
        }
    }
}

void checkBsecStatus(Bsec2 bsec) {
    if (bsec.status < BSEC_OK) {
        Serial.printf("BSEC error code: %d\n", bsec.status);
    } else if (bsec.status > BSEC_OK) {
        Serial.printf("BSEC warning code: %d\n", bsec.status);
    }
    if (bsec.sensor.status < BME68X_OK) {
        Serial.printf("BME68X error code: %d\n", bsec.sensor.status);
    } else if (bsec.sensor.status > BME68X_OK) {
        Serial.printf("BME68X warning code: %d\n", bsec.sensor.status);
    }
}
