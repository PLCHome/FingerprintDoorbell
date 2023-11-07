#include "SettingsManager.h"
#include <Crypto.h>

bool SettingsManager::loadWifiSettings() {
    Preferences preferences;
    if (preferences.begin("wifiSettings", true)) {
        wifiSettings.ssid = preferences.getString("ssid", wifiSettings.ssid);
        wifiSettings.password = preferences.getString("password", wifiSettings.password);
        wifiSettings.hostname = preferences.getString("hostname", wifiSettings.hostname);
        preferences.end();
        return true;
    } else {
        return false;
    }
}

bool SettingsManager::loadAppSettings() {
    Preferences preferences;
    if (preferences.begin("appSettings", true)) {
        appSettings.mqttServer = preferences.getString("mqttServer", appSettings.mqttServer);
        appSettings.mqttUsername = preferences.getString("mqttUsername", appSettings.mqttUsername);
        appSettings.mqttPassword = preferences.getString("mqttPassword", appSettings.mqttPassword);
        appSettings.mqttRootTopic = preferences.getString("mqttRootTopic", appSettings.mqttRootTopic);
        appSettings.ringtime = preferences.getUChar("ringtime", appSettings.ringtime);
        appSettings.ntpServer = preferences.getString("ntpServer", appSettings.ntpServer);
        appSettings.sensorPin = preferences.getString("sensorPin", appSettings.sensorPin);
        appSettings.sensorPairingCode = preferences.getString("pairingCode", appSettings.sensorPairingCode);
        appSettings.sensorPairingValid = preferences.getBool("pairingValid", appSettings.sensorPairingValid);
        appSettings.touchRingActiveColor = preferences.getChar("ringActCol", appSettings.touchRingActiveColor);
        appSettings.touchRingActiveSequence = preferences.getChar("ringActSeq", appSettings.touchRingActiveSequence);
        appSettings.scanColor = preferences.getChar("scanColor", appSettings.scanColor);
        appSettings.matchColor = preferences.getChar("matchColor", appSettings.matchColor);
        appSettings.enrollTemplates = preferences.getShort("enrollTemplates", appSettings.enrollTemplates);
        appSettings.sip_ip = preferences.getString("sip_ip", appSettings.sip_ip);
        appSettings.sip_user = preferences.getString("sip_user", appSettings.sip_user);
        appSettings.sip_pass = preferences.getString("sip_pass", appSettings.sip_pass);
        appSettings.sipTimeoutDial = preferences.getUChar("sipTimeoutDial", appSettings.sipTimeoutDial);
        appSettings.sipTimeoutConnect = preferences.getUShort("sipTimeoutConnect", appSettings.sipTimeoutConnect);
        appSettings.amp_gain = preferences.getUChar("amp_gain", appSettings.amp_gain);
        appSettings.mic_gain = preferences.getUChar("mic_gain", appSettings.mic_gain);
        appSettings.echocompensation = preferences.getBool("echocompensation", appSettings.echocompensation);
        appSettings.echothreshold = preferences.getLong("echothreshold", appSettings.echothreshold);
        appSettings.echodamping = preferences.getUChar("echodamping", appSettings.echodamping);
        appSettings.calldevicename = preferences.getString("calldevicename", appSettings.calldevicename);
        appSettings.phonenumber = preferences.getString("phonenumber", appSettings.phonenumber);
        appSettings.phonenumber2 = preferences.getString("phonenumber2", appSettings.phonenumber2);
        preferences.end();
        return true;
    } else {
        return false;
    }
}
   
void SettingsManager::saveWifiSettings() {
    Preferences preferences;
    preferences.begin("wifiSettings", false); 
    preferences.putString("ssid", wifiSettings.ssid);
    preferences.putString("password", wifiSettings.password);
    preferences.putString("hostname", wifiSettings.hostname);
    preferences.end();
}

void SettingsManager::saveAppSettings() {
    Preferences preferences;
    preferences.begin("appSettings", false); 
    preferences.putString("mqttServer", appSettings.mqttServer);
    preferences.putString("mqttUsername", appSettings.mqttUsername);
    preferences.putString("mqttPassword", appSettings.mqttPassword);
    preferences.putString("mqttRootTopic", appSettings.mqttRootTopic);
    preferences.putUChar("ringtime", appSettings.ringtime);
    preferences.putString("ntpServer", appSettings.ntpServer);
    preferences.putString("sensorPin", appSettings.sensorPin);
    preferences.putString("pairingCode", appSettings.sensorPairingCode);
    preferences.putBool("pairingValid", appSettings.sensorPairingValid);
    preferences.putChar("ringActCol", appSettings.touchRingActiveColor);
    preferences.putChar("ringActSeq", appSettings.touchRingActiveSequence);
    preferences.putChar("scanColor", appSettings.scanColor);
    preferences.putChar("matchColor", appSettings.matchColor);
    preferences.putShort("enrollTemplates", appSettings.enrollTemplates);
    preferences.putString("sip_ip", appSettings.sip_ip);
    preferences.putString("sip_user", appSettings.sip_user);
    preferences.putString("sip_pass", appSettings.sip_pass);
    preferences.putUChar("sipTimeoutDial", appSettings.sipTimeoutDial);
    Serial.println(appSettings.sipTimeoutConnect);
    preferences.putUShort("sipTimeoutConnect", appSettings.sipTimeoutConnect);
    preferences.putUChar("amp_gain", appSettings.amp_gain);
    preferences.putUChar("mic_gain", appSettings.mic_gain);
    preferences.putBool("echocompensation", appSettings.echocompensation);
    preferences.putLong("echothreshold", appSettings.echothreshold);
    preferences.putUChar("echodamping", appSettings.echodamping);
    preferences.putString("calldevicename", appSettings.calldevicename);
    preferences.putString("phonenumber", appSettings.phonenumber);
    preferences.putString("phonenumber2", appSettings.phonenumber2);
    preferences.end();
}

WifiSettings SettingsManager::getWifiSettings() {
    return wifiSettings;
}

void SettingsManager::saveWifiSettings(WifiSettings newSettings) {
    wifiSettings = newSettings;
    saveWifiSettings();
}

AppSettings SettingsManager::getAppSettings() {
    return appSettings;
}

void SettingsManager::saveAppSettings(AppSettings newSettings) {
    appSettings = newSettings;
    saveAppSettings();
}

bool SettingsManager::isWifiConfigured() {
    if (wifiSettings.ssid.isEmpty() || wifiSettings.password.isEmpty())
        return false;
    else
        return true;
}

bool SettingsManager::deleteAppSettings() {
    bool rc;
    Preferences preferences;
    rc = preferences.begin("appSettings", false); 
    if (rc)
        rc = preferences.clear();
    preferences.end();
    return rc;
}

bool SettingsManager::deleteWifiSettings() {
    bool rc;
    Preferences preferences;
    rc = preferences.begin("wifiSettings", false); 
    if (rc)
        rc = preferences.clear();
    preferences.end();
    return rc;
}

String SettingsManager::generateNewPairingCode() {

    /* Create a SHA256 hash */
    SHA256 hasher;

    /* Put some unique values as input in our new hash */
    hasher.doUpdate( String(esp_random()).c_str() ); // random number
    hasher.doUpdate( String(millis()).c_str() ); // time since boot
    hasher.doUpdate(getTimestampString().c_str()); // current time (if NTP is available)
    hasher.doUpdate(appSettings.mqttUsername.c_str());
    hasher.doUpdate(appSettings.mqttPassword.c_str());
    hasher.doUpdate(wifiSettings.ssid.c_str());
    hasher.doUpdate(wifiSettings.password.c_str());

    /* Compute the final hash */
    byte hash[SHA256_SIZE];
    hasher.doFinal(hash);
    
    // Convert our 32 byte hash to 32 chars long hex string. When converting the entire hash to hex we would need a length of 64 chars.
    // But because we only want a length of 32 we only use the first 16 bytes of the hash. I know this will increase possible collisions,
    // but for detecting a sensor replacement (which is the use-case here) it will still be enough.
    char hexString[33];
    hexString[32] = 0; // null terminatation byte for converting to string later
    for (byte i=0; i < 16; i++) // use only the first 16 bytes of hash
    {
        sprintf(&hexString[i*2], "%02x", hash[i]);
    }

    return String((char*)hexString);
}

