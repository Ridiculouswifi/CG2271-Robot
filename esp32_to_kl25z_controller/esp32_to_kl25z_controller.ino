#include <Bluepad32.h>

#define RXD2 16
#define TXD2 17

#define LED 2

/* All code commented with //## header is code related to serial for computer*/

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

ControllerPtr myController;

/*
// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {

      //## Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);

      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();

      //## Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
      //##          properties.product_id);

      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }

  digitalWrite(LED, HIGH);

  if (!foundEmptySlot) {
    //## Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}
*/

void onConnectedController(ControllerPtr ctl) {
  ControllerProperties properties = ctl->getProperties();
  myController = ctl;

  digitalWrite(LED, HIGH);
}

/*
void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {

      //## Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);

      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  digitalWrite(LED, LOW);

  if (!foundController) {
    //## Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}
*/

void onDisconnectedController(ControllerPtr ctl) {
  myController = nullptr;

  BP32.forgetBluetoothKeys();

  digitalWrite(LED, LOW);
}

void dumpGamepad(ControllerPtr ctl) {
  //## Serial.printf("idx=%d, dpad: 0x%02x, buttons: 0x%04x",
  //##          ctl->index(),   // Controller Index
  //##          ctl->dpad(),    // D-pad
  //##          ctl->buttons()  // bitmask of pressed buttons
  //## );
}

void processGamepad(ControllerPtr ctl) {
  // Left Stick (LS) thresholds
  int16_t x = ctl->axisX();
  int16_t y = ctl->axisY();

  // Right Stick (RS) thresholds
  int16_t rx = ctl->axisRX(); // X value of the right stick

  // Checking value of buttons
  int16_t bumper = ctl->buttons();

  // Wave (Emergency brake)
  if (bumper == 0x0010 || bumper == 0x0020 || bumper == 0x0030) {

    ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */, 
            0x40 /* strongMagnitude */);
    // Stop all movement (emergency brake)

    sendPacketToKL25Z(0xFF);

  } else if (bumper) {

    if (bumper == 0x0001) {
      sendPacketToKL25Z(0x10); // Button B Pressed: Turn on Moving Song
    } 

    else if (bumper == 0x0002) {
      sendPacketToKL25Z(0x20); // Button A Pressed: Turn on Ending Song
    }

    else if (bumper == 0x0008) {
      sendPacketToKL25Z(0x30); // Button X Pressed: High Speed 
    }

    else if (bumper == 0x0004) {
      sendPacketToKL25Z(0x40); // Button Y Pressed: Low Speed
    }


  } else {
    /* Check the value of Right Stick first */

    if (rx <= -300) {   // Right Stick is pointing LEFT

      if (x >= -50 && x <= 50 && y >= -50 && y <= 50) {
        // Rotate LEFT
        sendPacketToKL25Z(0x08);
      }

      else if (y <= -330) {
        // FORWARD LEFT
        sendPacketToKL25Z(0x09);
      }

      else if (y >= 330) {
        // BACKWARD LEFT
        sendPacketToKL25Z(0x0A);
      }

    } 
    
    else if (rx >= 300) {   // Right Stick is pointing RIGHT

      if (x >= -50 && x <= 50 && y >= -50 && y <= 50) {
        // Rotate RIGHT
        sendPacketToKL25Z(0x04);
      }

      else if (y <= -330) {
        // FORWARD RIGHT
        sendPacketToKL25Z(0x05);
      }

      else if (y >= 330) {
        // BACKWARD RIGHT
        sendPacketToKL25Z(0x06);
      }

    } 
    
    else {  // Right Stick is IDLE
      
      if (x >= -50 && x <= 50 && y >= -50 && y <= 50) {
        // IDLE
        sendPacketToKL25Z(0x00);
      } 

      else if (y <= -450) {
        // FORWARD
        sendPacketToKL25Z(0x01);
      }
      
      else if (y >= 450) {
        // BACKWARD
        sendPacketToKL25Z(0x02);
      }
      
      else {
        // If none of the conditions are met, you can decide to either do nothing or handle it
        sendPacketToKL25Z(0xAA);  // Optional: Handle unknown state
      }
    }
  }
}

/*
void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {

        //## Serial.println("Unsupported controller");

      }
    }
  }
}
*/

void processController() {
  if (myController && myController->isConnected() && myController->hasData()) {
    if (myController->isGamepad()) {
      processGamepad(myController);
    }
  }
}

void sendPacketToKL25Z(uint8_t command) {
  // Send the command packet via Serial to KL25Z
  Serial2.write(command);

  //## Serial.printf("Command Send: 0x%02x", command);
  //## Serial.println("");

  Serial2.flush();  // Ensure all data is sent
  delay(10);
}

// Arduino setup function. Runs in CPU 1
void setup() {
  //## Serial.begin(9600);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  //## Serial.printf("Firmware: %s\n", BP32.firmwareVersion());

  const uint8_t* addr = BP32.localBdAddress();

  //## Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);

  // Setup in-built LED
  pinMode(LED, OUTPUT);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
  // This call fetches all the controllers' data.
  // Call this function in your main loop.
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processController();
  else {
    sendPacketToKL25Z(0x0F);
  }

  // The main loop must have some kind of "yield to lower priority task" event.
  // Otherwise, the watchdog will get triggered.
  // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
  // Detailed info here:
  // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

  vTaskDelay(1);
  delay(150);
}
