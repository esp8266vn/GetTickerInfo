#include <CoinMarketCapApi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "OLEDDisplayUi.h"
#include "bitcoin.h"
#include "eth.h"
#include "zcash.h"
#include "litecoin.h"
#include "iot.h"
/*******************************************************************
    An example of getting ticker info from coinmarketcap.com
 *                                                                 *
    Written by Brian Lough
 *******************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//------- Install From Library Manager -------
#include <ArduinoJson.h>


//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASS";  // your network key



WiFiClient client;
CoinMarketCapApi api(client);

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.


SSD1306  display(0x3c, 4, 5);
// SH1106 display(0x3c, D3, D5);

OLEDDisplayUi ui     ( &display );

String btc = "0.000", eth = "0.00", zcash = "0.00", litecoin = "0.00";
void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 0, String(millis()));
}

void drawFrame0(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  display->drawXbm(x + 2, y + 14, iot_width, iot_height, iot_bits);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(42 + x, 20 + y, "iotmaker.vn");
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  display->drawXbm(x + 2, y + 14, bitcoin_width, bitcoin_height, bitcoin_bits);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(40 + x, 16 + y, btc);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x + 2, y + 14, eth_width, eth_height, eth_bits);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(40 + x, 16 + y, eth);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x + 2, y + 14, zcash_width, zcash_height, zcash_bits);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(40 + x, 16 + y, zcash);
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x + 2, y + 14, litecoin_width, litecoin_height, litecoin_bits);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(40 + x, 16 + y, litecoin);
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = {drawFrame0, drawFrame1, drawFrame2, drawFrame3, drawFrame4 };


// how many frames are there?
int frameCount = 5;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void setup() {

  Serial.begin(115200);

  ui.setTargetFPS(60);

  // Customize the active and inactive symbol
  //  ui.setActiveSymbol(activeSymbol);
  //  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void printTickerData(String ticker) {
  Serial.println("---------------------------------");
  Serial.println("Getting ticker data for " + ticker);


  // Ticker unfortunately is not the symbol for some reason.
  // Go to CoinMarketCap.com and select the coin you would like to check
  // The ticker name makes up the last part of the URL
  // e.g: http://coinmarketcap.com/currencies/bitcoin/ , "bitcoin" is the ticker value

  // Currency is optional, so you can pass only ticker if you want.
  // Check out the currency drop down on CoinMarketCap.com to get available values
  CMCTickerResponse response = api.GetTickerInfo(ticker, "usd");
  if (response.error == "") {
    if (ticker == "bitcoin") {
      btc = String(response.price_usd);
    }

    if (ticker == "ethereum") {
      eth = String(response.price_usd);
    }

    if (ticker == "zcash") {
      zcash = String(response.price_usd);
    }

    if (ticker == "litecoin") {
      litecoin = String(response.price_usd);
    }
    
    Serial.print("ID: ");
    Serial.println(response.id);
    Serial.print("Name: ");
    Serial.println(response.name);
//    Serial.print("Symbol: ");
//    Serial.println(response.symbol);
//
//    Serial.print("Rank: ");
//    Serial.println(response.rank);
//
//    Serial.print("Price in USD: ");
//    Serial.println(response.price_usd);
//    Serial.print("Price in BTC: ");
//    Serial.println(response.price_btc);
//
//    Serial.print("24h Volume USD: ");
//    Serial.println(response.volume_usd_24h);
//    Serial.print("Market Cap USD: ");
//    Serial.println(response.market_cap_usd);
//
//    Serial.print("Available Supply: ");
//    Serial.println(response.available_supply);
//    Serial.print("Total Supply: ");
//    Serial.println(response.total_supply);
//
//    Serial.print("Percent Change 1h: ");
//    Serial.println(response.percent_change_1h);
//    Serial.print("Percent Change 24h: ");
//    Serial.println(response.percent_change_24h);
//    Serial.print("Percent Change 7d: ");
//    Serial.println(response.percent_change_7d);
//    Serial.print("Last Updated: ");
//    Serial.println(response.last_updated);
//
//    // These fields will not come back if you do not request a currency
//    Serial.print("Price in requested currecy: ");
//    Serial.println(response.price_currency);
//    Serial.print("24h Volume in requested currency: ");
//    Serial.println(response.volume_currency_24h);
//    Serial.print("Market Cap in requested currency: ");
//    Serial.println(response.market_cap_currency);


  } else {
    Serial.print("Error getting data: ");
    Serial.println(response.error);
  }
  Serial.println("---------------------------------");
}

unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;
String coins[] = {"bitcoin", "ethereum", "zcash", "litecoin"};
int coin_cnt = 0;
void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > api_due_time))  {
    printTickerData(coins[0]);
    printTickerData(coins[1]);
    printTickerData(coins[2]);
    printTickerData(coins[3]);
//    coin_cnt ++;
//    if (coin_cnt >= frameCount) 
//      coin_cnt = 0;
    api_due_time = timeNow + api_mtbs;
  }

  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
  }
}
