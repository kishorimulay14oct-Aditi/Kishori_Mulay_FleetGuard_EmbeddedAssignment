#ifndef FLEETGUARD_CONFIG_EXAMPLE_H
#define FLEETGUARD_CONFIG_EXAMPLE_H

/*
 * FleetGuard Example Configuration
 *
 * This file contains PLACEHOLDER values only.
 *
 * IMPORTANT:
 * Never commit real Wi-Fi passwords, MQTT usernames,
 * MQTT passwords, certificates, API keys, or other secrets
 * to the public GitHub repository.
 */

// ---------------------------------------------------------
// Wi-Fi Configuration
// ---------------------------------------------------------

#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"


// ---------------------------------------------------------
// MQTT Configuration
// ---------------------------------------------------------

#define MQTT_BROKER     "YOUR_MQTT_BROKER"
#define MQTT_PORT       1883

#define MQTT_USERNAME   "YOUR_MQTT_USERNAME"
#define MQTT_PASSWORD   "YOUR_MQTT_PASSWORD"

#define MQTT_TOPIC      "fleet/device001/telemetry"


#endif
