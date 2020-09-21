#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "./functions.h"
#include <string.h>


#define disable 0
#define enable  1
unsigned int channel = 1;

const String HOST = "http://macsniff.herokuapp.com/api/attendance/set";

void setup() {
  Serial.begin(57600);

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb_main);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
}

void loop() {
  channel = 1;
  wifi_set_channel(channel);
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 100) {
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()
  }
}

String decToHex(int n){
  char hex[2];  
  sprintf(hex, "%02x", n);

  return String(hex);
}

void promisc_cb_main(uint8_t *buf, uint16_t len)
{
  HTTPClient http;  
  String link;
  int i = 0;
  uint16_t seq_n_new = 0;
  if (len == 12) {
    struct RxControl *sniffer = (struct RxControl*) buf;
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    struct beaconinfo beacon = parse_beacon(sniffer->buf, 112, sniffer->rx_ctrl.rssi);
    if (register_beacon(beacon) == 0) {
//      print_beacon(beacon);
      nothing_new = 0;
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
    //Is data or QOS?
    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client(ci) == 0) {

          String dev = "DEVICE : ";
          for(int j=0;j<6;j++){
              dev += decToHex((int)ci.station[j]);
              if(j==5){              
                continue;
              }else{
                dev += "-";
              }
            }
        Serial.println(dev);
           String ap = "AP : ";
          for(int k=0;k<6;k++){
              ap += decToHex((int)ci.ap[k]);
              if(k==5){              
                continue;
              }else{
                ap += "-";
              }
            }
           
          Serial.println(ap);
          
        //HTTP request

        link = HOST + "?device=" + dev +"?ap=" + ap;
        http.begin(link);

        int httpCode = http.GET(); 
        String payload = http.getString();

        Serial.println(httpCode);
        Serial.println(payload);
        http.end();

          nothing_new = 0;
        }
      }
    }
  }
}
