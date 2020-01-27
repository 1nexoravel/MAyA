/* ESP8266 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
 * No messing with openssl or spiffs just regular pubsub and certificates in string constants
 * 
 * Note: For this to work you do need to do this:
 *       https://github.com/esp8266/Arduino/issues/3944
 *
 * Author: Anthony Elder 
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

const char* ssid = "<BAKA>";
const char* password = "<size1080p>";

const char* awsEndpoint = "a3m8k275ip9a0y-ats.iot.us-east-1.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// 9a0b6ce6db-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDWjCCAkKgAwIBAgIVAN1Jus0oONymIXj0Yg2avfR4+qw1MA0GCSqGSIb3DQEB" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA0MTUxNzM3" \
"MTVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDJlJPXiZ4n7LdBSCgP" \
"cil6RHtp2+c+IC8kFuQiS6bcdQmV/LzW0r5863l7VQLKhBIvOTDUdhtyKvJFus1X" \
"mMpON5eaIaZOFnH1Lc+VEKmgULbzLfC8ziUsYEVpYGpnZ+IoMsshbMkzDfr/QrFQ" \
"ap7nbSR9ObjP8ASV/F31s4BcWZjgQhJhBAoNSLjYQScOxrfZBBvNDZmR83RcVVqv" \
"UrD4xOWJPgaJOqIEctXrD8h/557rlRIFYUvHg/Ix1KjYHQeB2Shzj9/HUh1BTVKz" \
"kdaME+KdAH13jJ43A0DK5VQrVs9U1lPYysH3obJaXiOTABR4nTiMvAmV98J4zyJf" \ 
"k2kBAgMBAAGjYDBeMB8GA1UdIwQYMBaAFE+QE6A92jYaUUAPBKnxvm2z2rR3MB0G" \
"A1UdDgQWBBTZFGsOV0Zsm9IXiI5m8fdV7fDC5TAMBgNVHRMBAf8EAjAAMA4GA1Ud" \ 
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAvxCXh4w7LXGx57GlgzkrMu7/" \
"nK4vqVxKgup5enbUYErNNE9XnuG7r7XB9VfinqJr0mQy5gEpQT6BlQ1pzvOD/hHa" \
"Qjg0ZxBkXxGosPJmHpm6vi1XD7rRrZGNMWuROj/eAcF+G+2ULUsjevBNhAUMeiMY" \
"hvb9q8i86/2oXuSqV/rUzTi7lR2xg6jtyVubU7gLh2dQBbndZMM7f2GJqScCzkuJ" \
"g57hwc/F3iuqFF0Hp7n5bUj8VXebpsNXVYbzTLVvOYs8/7lTLXlmeqZb3xDQE1ud" \
"2oerWgKhJrPN5qXNU6p1rShckizUSeEt70cpO5n27ds+CELfta9ObZPRVXEIcA==";

//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
"MIIEowIBAAKCAQEAyZST14meJ+y3QUgoD3IpekR7advnPiAvJBbkIkum3HUJlfy8" \
"1tK+fOt5e1UCyoQSLzkw1HYbciryRbrNV5jKTjeXmiGmThZx9S3PlRCpoFC28y3w" \
"vM4lLGBFaWBqZ2fiKDLLIWzJMw36/0KxUGqe520kfTm4z/AElfxd9bOAXFmY4EIS" \
"YQQKDUi42EEnDsa32QQbzQ2ZkfN0XFVar1Kw+MTliT4GiTqiBHLV6w/If+ee65US" \
"BWFLx4PyMdSo2B0Hgdkoc4/fx1IdQU1Ss5HWjBPinQB9d4yeNwNAyuVUK1bPVNZT" \
"2MrB96GyWl4jkwAUeJ04jLwJlffCeM8iX5NpAQIDAQABAoIBAHHzOmuyEKRzDNir" \
"aHHq0AWc5sLdm2ekG+HjysyURVwEEQonV7x8X1AQcJjGYpCGfh0R0EeaCEPOgSvn" \
"wCJPbbUvedSYRwGHcJ3vapPSWoEUf4cSJi8kvSDLvxElfqC7ub2NkQRxEX953LQl" \
"SES0MOWT7feYBKAPBxieE6NL+Di7L0OH9rI5c206Kapa2v3u7SswJ68bgOQLL2e0" \
"fXju7qdDu351sNegxOcFu1LxUDAgvgG3J7sAuwX/pJm1M4dAUZIyQnpjs8gw3LG4" \
"Kb9MmEJWayQTSNSh8r6CTZprNQk4MCPqmFljt5d9giaEs2ZdydBNjLdw4FgYNvqk" \
"Zx90p9ECgYEA+lzMIUHNYxjXyQkTlR6b6yeumm9wut3S/A0SGH9LiLTJVGkxqCB/" \
"6IIbwGJc/mL4pziZTct1Fmgb8ffFl+ysaL9OfJzQGscnDFAxnmPUz/xOhBQpUKpl" \
"alfqymLTcNvRMnva7zz7iFhxTM8oBkJ7PxDbfHnOynxwk82/q+JmsFUCgYEAzh6T" \
"9wIQxCqOq/1VivefsNnYjpqLi9VbcCD3K3UM8aV9MGHDMfmFMChfZsrmag1Wd3w4" \
"L4onNQPlxm9S1tDre9Na8DCCtdjy36sjGhSyHhkXbklHMioKcKSbmHKjBLtmyLgn" \
"XELjqlh0xmVp+AGXBtqIHiZOO1c+3f5Ik6hbkf0CgYAFigwSKEyNBr2vev3t7LFy" \
"cHxSemi6GbJTVU6zMA3Rxn412eX5XE64LDjE9ugr0LDDsmLh78B1cHQ91EeQ5vGh" \
"l6n649VO4l0gzp1qbbcFqwVpsrB/UmXQBoXxWGJpOtPUFB1x8Z13FM3c7zBpgwsL" \
"CCpzBJbor/wlqQW2ndm7yQKBgQC2A/ij6pYFtJMSu/6bXjoNgQIkQ8Mw83LwnJvI" \
"K3AHwBzB6cLmUbLUqQI+ErTQOAEAOJihGn2u6kcxKUNc4VbhnO20JxJ2JSOsckLo" \
"GBiGTaAMSvdtDAT2HdYtFCZy2k5Ut5JCfervlXdhb1LZqAXHDpmfDjKNYmjovlE2" \
"2zgpvQKBgBZEDorlDvLNDy7S/RpJoe+57GfdhT8FwhBQ1pIb5MtZz426LugxJyF9" \
"drFsuKNNLoR9EapQPtcH3OKjSVv2F9fpBPqebdtauizpaSwz9BoKN0nCJbinjVml" \
"5iHdpRGK7XXECHxggqsq1M2lFppGcSPPXq21g15S0+YzbaNNHDbu" ;
//-----END RSA PRIVATE KEY-----

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  if (millis() - lastPublish > 10000) {
    String msg = String("Hello from ESP8266: ") + ++msgCount;
    pubSubClient.publish("outTopic", msg.c_str());
    Serial.print("Published: "); Serial.println(msg);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}
