/*

The URL can be anywhere your device can reach:
FOTA_url: 
"http://192.168.1.10/firmwares/fota.json"
Current version example:
"1.0.0"

The response is a JSON object. If there are no updates available 
it will be empty (that is: '{}'). Otherwise it will contain info 
about where to find the new firmware binaries:

{   
    'fwVersion': '0.1.1',
    'fwFile': 'myapp-0.1.1.bin',
    'fsVersion': '1.1.1',
    'fsFile': 'myapp-0.1.1-fs.bin'
}

Binaries URLs (for the firmware and the FS file system) are relative 
to the server URL, so following with the example, the device will first 
download the SPIFFS binary from:

http://192.168.1.10/firmwares/myapp-0.1.1-fs.bin

flash it and if everything went fine it will download the firmware from:
http://192.168.1.10/firmwares/sonoff-0.1.1.bin

*/

#pragma once

#include <functional>
#include <Stream.h>
#include <ArduinoJson.h>
#ifdef ESP8266
#include <ESP8266httpUpdate.h>
#elif defined(ESP32)
#include <httpUpdate.h>
#endif

typedef enum {
    FOTA_START,
    FOTA_NO_RESPONSE_ERROR,
    FOTA_PARSE_ERROR,
    FOTA_UPTODATE,
    FOTA_NEW_FIRMWARE_FOUND,
    FOTA_NEW_FILESYSTEM_FOUND,
    FOTA_UPDATING_FIRMWARE,
    FOTA_UPDATING_FILESYSTEM,
    FOTA_FIRMWARE_UPLOADED,
    FOTA_FILESYSTEM_UPLOADED,    
    FOTA_FIRMWARE_UPDATE_ERROR,
    FOTA_FILESYSTEM_UPDATE_ERROR,    
    FOTA_RESET,
    FOTA_END
} fota_t;

#define HTTP_TIMEOUT    10000
#define HTTP_USERAGENT  "ESPUpdateClient"

class FOTAClientClass {

  public:

    typedef std::function<void(fota_t, char *)> TMessageFunction;
    void setFOTAParameters(String FOTA_url, String jsonFile, String currentFwVersion, String currentFsVersion);
    bool checkUpdates();
    bool updateFirmware(String FOTA_bin_url);
    bool updateFileSystem(String FOTA_fs_url);
    void checkAndUpdateFOTA(bool reboot);
    
    String getNewFirmwareVersionNumber();
    String getNewFirmwareURL();
    bool newFirmwareFound();

    String getNewFileSystemVersionMumber();
    String getNewFileSystemURL();
    bool newFileSystemFound();
    int getErrorNumber();
    String getErrorString();

    void onMessage(TMessageFunction fn);

  private:

    String _FOTA_url;
    String _jsonFile;
    char* _str(String txt);
    bool _newFirmware   = false;
    bool _newFileSystem = false;

    String _currentFwVersion;
    String _newFwVersion;

    String _currentFsVersion;
    String _newFsVersion;
    
    String _newFwURL;
    String _newFsURL;

    int _errorNumber;
    String _errorString;

    TMessageFunction _callback = NULL;

    String _getPayload();
    
    void _doUpdate();
    void _doCallback(fota_t message, char * parameter = NULL);

};

extern FOTAClientClass FOTAClient;
