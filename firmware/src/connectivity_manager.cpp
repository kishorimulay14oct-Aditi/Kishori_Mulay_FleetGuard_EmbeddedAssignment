#include "connectivity_manager.h"

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

/*
 * FleetGuard Connectivity Manager
 *
 * Wi-Fi + MQTT readiness
 *
 * Important:
 * Local monitoring must continue even when
 * Wi-Fi or MQTT is unavailable.
 */

// ---------------------------------------------------------
// Network Configuration
// ---------------------------------------------------------

// These are placeholders for the prototype.
// Do NOT commit real passwords or credentials to GitHub.

static const char* WIFI_SSID = "YOUR_WIFI_SSID";
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

static const char* MQTT_SERVER = "YOUR_MQTT_BROKER";
static const int MQTT_PORT = 1883;

static const char* MQTT_TOPIC =
    "fleet/device001/telemetry";


// ---------------------------------------------------------
// Network Objects
// ---------------------------------------------------------

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static bool connectivityInitialized = false;


// ---------------------------------------------------------
// Initialization
// ---------------------------------------------------------

bool connectivityManagerBegin()
{
    /*
     * Configure MQTT server.
     */
    mqttClient.setServer(
        MQTT_SERVER,
        MQTT_PORT
    );

    connectivityInitialized = true;

    Serial.println(
        "Connectivity Manager initialized."
    );

    return true;
}


// ---------------------------------------------------------
// Wi-Fi Connection
// ---------------------------------------------------------

bool connectToWiFi()
{
    if (!connectivityInitialized)
    {
        return false;
    }

    /*
     * Do not attempt connection if credentials
     * have not been configured.
     */
    if (strcmp(WIFI_SSID, "YOUR_WIFI_SSID") == 0)
    {
        Serial.println(
            "Wi-Fi credentials not configured."
        );

        return false;
    }

    Serial.print("Connecting to Wi-Fi");

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    unsigned long startTime = millis();

    /*
     * Use a timeout so that local monitoring
     * is never blocked indefinitely.
     */
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < 10000)
    {
        delay(250);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Wi-Fi connected.");

        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        return true;
    }

    Serial.println(
        "Wi-Fi connection unavailable."
    );

    return false;
}


// ---------------------------------------------------------
// MQTT Connection
// ---------------------------------------------------------

static bool connectToMQTT()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }

    if (strcmp(MQTT_SERVER, "YOUR_MQTT_BROKER") == 0)
    {
        return false;
    }

    if (mqttClient.connected())
    {
        return true;
    }

    Serial.println("Connecting to MQTT broker...");

    String clientId =
        "FleetGuard-" +
        String((uint32_t)ESP.getEfuseMac(), HEX);

    if (mqttClient.connect(clientId.c_str()))
    {
        Serial.println(
            "MQTT broker connected."
        );

        return true;
    }

    Serial.print(
        "MQTT connection failed. State: "
    );

    Serial.println(
        mqttClient.state()
    );

    return false;
}


// ---------------------------------------------------------
// Maintain Connectivity
// ---------------------------------------------------------

void maintainConnectivity()
{
    if (!connectivityInitialized)
    {
        return;
    }

    /*
     * If Wi-Fi is unavailable, do not block the
     * main monitoring system.
     */
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }

    /*
     * Maintain MQTT connection when possible.
     */
    if (mqttClient.connected())
    {
        mqttClient.loop();
    }
    else
    {
        connectToMQTT();
    }
}


// ---------------------------------------------------------
// Publish Telemetry
// ---------------------------------------------------------

bool publishTelemetry(
    float temperature,
    float humidity,
    const char* state)
{
    if (!connectivityInitialized)
    {
        return false;
    }

    if (!connectToMQTT())
    {
        return false;
    }

    /*
     * Simple JSON telemetry payload.
     */
    char payload[160];

    snprintf(
        payload,
        sizeof(payload),
        "{\"temperature\":%.2f,"
        "\"humidity\":%.2f,"
        "\"state\":\"%s\"}",
        temperature,
        humidity,
        state
    );

    bool published =
        mqttClient.publish(
            MQTT_TOPIC,
            payload
        );

    if (published)
    {
        Serial.println(
            "Telemetry published through MQTT."
        );
    }

    return published;
}
