/********
 *author=Jarod
 *
 *Jump to the bottom and read those lines and comments first, then work your 
 *  way up to the functions and their comments. It'll make more sense that way.
 */

#include "Arduino.h"
#include "heltec.h"

void printBuffer(void) {
  // Initialize the log buffer and allocate memory to store 5 lines of 
  //    text (don't change this number) and 30 chars per line (haven't 
  //    figured out why this matters).
  Heltec.display->setLogBuffer(5, 30);

  // List of things to be printed (in order)
  const char* test[] = {
      "Hello",
      "World" ,
      "----"
  };

  for (uint8_t i = 0; i < 6; i++) {
    //clears the display
    Heltec.display->clear();
    //prints a line to the screen
    String myString = "Hello World";
    Heltec.display->println(myString);
    //print next element of list test to the screen
    /*Heltec.display->println(test[i]);*/
    // Add all println lines to the internal screen buffer (log)
    Heltec.display->drawLogBuffer(0, 0);
    // Display the log on the screen
    Heltec.display->display();
    delay(500);
  }
  Heltec.display->clear();
  Heltec.display->println("All Done");
  Heltec.display->drawLogBuffer(0, 0);
  Heltec.display->display();
}

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->setContrast(255);
  Heltec.display->setFont(ArialMT_Plain_10);

  //Use this method if you want it to scroll through more lines than the screen can fit
  printBuffer();
  delay(1000);
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_16);
}

void loop() {
  //Use this method if you only want a few lines and want them to stay there
  Heltec.display->clear();
  Heltec.display->drawString(0,0,"Meh");
  Heltec.display->drawString(0,6,"Meh");
  Heltec.display->display();
}
