#include <Bluepad32.h>

// Define button masks for face and shoulder buttons
const int MY_BUTTON_X = 0x0001;
const int MY_BUTTON_CIRCLE = 0x0002;
const int MY_BUTTON_SQUARE = 0x0004;
const int MY_BUTTON_TRIANGLE = 0x0008;
const int MY_BUTTON_R1 = 0x0020;
const int MY_BUTTON_L1 = 0x0010;
const int MY_BUTTON_R3 = 0x0200; // Right stick click
const int MY_BUTTON_L3 = 0x0100; // Left stick click

// Define D-Pad button masks
const int MY_BUTTON_DPAD_UP = 0x01;
const int MY_BUTTON_DPAD_DOWN = 0x02;
const int MY_BUTTON_DPAD_LEFT = 0x08;
const int MY_BUTTON_DPAD_RIGHT = 0x04;

// Define misc buttons (values based on your description)
const int MY_BUTTON_OPTIONS = 0x04; // Misc bit 0x01
const int MY_BUTTON_SHARE = 0x02;   // Misc bit 0x02
const int MY_BUTTON_PS = 0x01;      // Misc bit 0x04

// Previous states for comparison
int previousButtons = 0;
int previousDpad = 0;
int previousMiscButtons = 0; 
int previousBrake = 0; 
int previousThrottle = 0; 


ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
      }
    }

    if (!foundEmptySlot) {
      Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

    if (!foundController) {
      Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

// Function to set RGB color based on battery level
void setBatteryLEDColor(int batteryLevel, ControllerPtr ctl) {
    // Define the red, green, and blue components based on battery level (0 to 100)
    int r = 0, g = 0, b = 0;

    // Scale the battery level into 10 intervals (0-100 -> 10 levels)
    if (batteryLevel == 100) {
        r = 0;
        g = 255;
        b = 0; // Green
    } else if (batteryLevel >= 90) {
        r = 0;
        g = 230;
        b = 0; // Light Green
    } else if (batteryLevel >= 80) {
        r = 50;
        g = 200;
        b = 0; // Greenish Lime
    } else if (batteryLevel >= 70) {
        r = 100;
        g = 180;
        b = 0; // Lime Green
    } else if (batteryLevel >= 60) {
        r = 150;
        g = 160;
        b = 0; // Yellowish Lime
    } else if (batteryLevel >= 50) {
        r = 200;
        g = 150;
        b = 0; // Yellow
    } else if (batteryLevel >= 40) {
        r = 255;
        g = 125;
        b = 0; // Orange Yellow
    } else if (batteryLevel >= 30) {
        r = 255;
        g = 100;
        b = 0; // Strong Orange
    } else if (batteryLevel >= 20) {
        r = 255;
        g = 70;
        b = 0; // Red Orange
    } else if (batteryLevel >= 10) {
        r = 255;
        g = 40;
        b = 0; // Orange Red
    } else {
        r = 255;
        g = 0;
        b = 0; // Red (very low charge)
    }

    
    // Set the RGB LED color
    ctl->setColorLED(r, g, b);
}

// ========= SEE CONTROLLER VALUES IN SERIAL MONITOR ========= //

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
  "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
  "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
  ctl->index(),        // Controller Index
  ctl->dpad(),         // D-pad
  ctl->buttons(),      // bitmask of pressed buttons
  ctl->axisX(),        // (-511 - 512) left X Axis
  ctl->axisY(),        // (-511 - 512) left Y axis
  ctl->axisRX(),       // (-511 - 512) right X axis
  ctl->axisRY(),       // (-511 - 512) right Y axis
  ctl->brake(),        // (0 - 1023): brake button
  ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
  ctl->miscButtons(),  // bitmask of pressed "misc" buttons
  ctl->gyroX(),        // Gyro X
  ctl->gyroY(),        // Gyro Y
  ctl->gyroZ(),        // Gyro Z
  ctl->accelX(),       // Accelerometer X
  ctl->accelY(),       // Accelerometer Y
  ctl->accelZ()        // Accelerometer Z
  );
}


void processGamepad(ControllerPtr ctl) {
    // Store the current button state
    int currentButtons = ctl->buttons();
    int currentDpad = ctl->dpad();       // D-Pad state
    int currentMisc = ctl->miscButtons(); // Miscellaneous buttons state

    // Get battery level (0-100%)
    int batteryLevel = ctl->battery();
    // Call the function to set LED color based on battery level
    setBatteryLEDColor(batteryLevel, ctl);
   
    // Handle X button
    if ((currentButtons & MY_BUTTON_X) && !(previousButtons & MY_BUTTON_X)) {
        // X button pressed
        Serial.println("X pressed");
    }
    if (!(currentButtons & MY_BUTTON_X) && (previousButtons & MY_BUTTON_X)) {
        // X button released
        Serial.println("X released");
    }

    // Handle Circle button
    if ((currentButtons & MY_BUTTON_CIRCLE) && !(previousButtons & MY_BUTTON_CIRCLE)) {
        // Circle button pressed
        Serial.println("Circle pressed");
        digitalWrite(0, HIGH); // Example pin
    }
    if (!(currentButtons & MY_BUTTON_CIRCLE) && (previousButtons & MY_BUTTON_CIRCLE)) {
        // Circle button released
        Serial.println("Circle released");
        digitalWrite(0, LOW); // Example pin
    }

    // Handle Square button
    if ((currentButtons & MY_BUTTON_SQUARE) && !(previousButtons & MY_BUTTON_SQUARE)) {
        // Square button pressed
        Serial.println("Square pressed");
        digitalWrite(4, HIGH); // Example pin
    }
    if (!(currentButtons & MY_BUTTON_SQUARE) && (previousButtons & MY_BUTTON_SQUARE)) {
        // Square button released
        Serial.println("Square released");
        digitalWrite(4, LOW); // Example pin
    }

    // Handle Triangle button
    if ((currentButtons & MY_BUTTON_TRIANGLE) && !(previousButtons & MY_BUTTON_TRIANGLE)) {
        // Triangle button pressed
        Serial.println("Triangle pressed");
    }
    if (!(currentButtons & MY_BUTTON_TRIANGLE) && (previousButtons & MY_BUTTON_TRIANGLE)) {
        // Triangle button released
        Serial.println("Triangle released");
    }

    // Handle D-Pad buttons (up, down, left, right)
    if ((currentDpad & MY_BUTTON_DPAD_UP) && !(previousDpad & MY_BUTTON_DPAD_UP)) {
        Serial.println("Up pressed");
    }
    if (!(currentDpad & MY_BUTTON_DPAD_UP) && (previousDpad & MY_BUTTON_DPAD_UP)) {
        Serial.println("Up released");
    }

    if ((currentDpad & MY_BUTTON_DPAD_DOWN) && !(previousDpad & MY_BUTTON_DPAD_DOWN)) {
        Serial.println("Down pressed");
    }
    if (!(currentDpad & MY_BUTTON_DPAD_DOWN) && (previousDpad & MY_BUTTON_DPAD_DOWN)) {
        Serial.println("Down released");
    }

    if ((currentDpad & MY_BUTTON_DPAD_LEFT) && !(previousDpad & MY_BUTTON_DPAD_LEFT)) {
        Serial.println("Left pressed");
    }
    if (!(currentDpad & MY_BUTTON_DPAD_LEFT) && (previousDpad & MY_BUTTON_DPAD_LEFT)) {
        Serial.println("Left released");
    }

    if ((currentDpad & MY_BUTTON_DPAD_RIGHT) && !(previousDpad & MY_BUTTON_DPAD_RIGHT)) {
        Serial.println("Right pressed");
    }
    if (!(currentDpad & MY_BUTTON_DPAD_RIGHT) && (previousDpad & MY_BUTTON_DPAD_RIGHT)) {
        Serial.println("Right released");
    }

    // Handle Misc buttons (Options, Share, PS)
    if ((currentMisc & MY_BUTTON_OPTIONS) && !(previousMiscButtons & MY_BUTTON_OPTIONS)) {
        Serial.println("Options pressed");
        int batteryLevel = ctl->battery();  // Get the battery level (0 to 100%)
        // Print the battery level if the controller is connected
        Serial.print("Battery Level: ");
        Serial.print(batteryLevel);
        Serial.println("%");
    }
    if (!(currentMisc & MY_BUTTON_OPTIONS) && (previousMiscButtons & MY_BUTTON_OPTIONS)) {
        Serial.println("Options released");
    }

    if ((currentMisc & MY_BUTTON_SHARE) && !(previousMiscButtons & MY_BUTTON_SHARE)) {
        Serial.println("Share pressed");
    }
    if (!(currentMisc & MY_BUTTON_SHARE) && (previousMiscButtons & MY_BUTTON_SHARE)) {
        Serial.println("Share released");
    }

    if ((currentMisc & MY_BUTTON_PS) && !(previousMiscButtons & MY_BUTTON_PS)) {
        Serial.println("PS pressed");
    }
    if (!(currentMisc & MY_BUTTON_PS) && (previousMiscButtons & MY_BUTTON_PS)) {
        Serial.println("PS released");
    }

    // Handle L3 button press/release
    if ((currentButtons & MY_BUTTON_L3) && !(previousButtons & MY_BUTTON_L3)) {
        Serial.println("L3 pressed");
    }
    if (!(currentButtons & MY_BUTTON_L3) && (previousButtons & MY_BUTTON_L3)) {
        Serial.println("L3 released");
    }
    
    // Handle R3 button press/release
    if ((currentButtons & MY_BUTTON_R3) && !(previousButtons & MY_BUTTON_R3)) {
        Serial.println("R3 pressed");
    }
    if (!(currentButtons & MY_BUTTON_R3) && (previousButtons & MY_BUTTON_R3)) {
        Serial.println("R3 released");
    }
    
    // Handle R1 button press/release
    if ((currentButtons & MY_BUTTON_R1) && !(previousButtons & MY_BUTTON_R1)) {
        Serial.println("R1 pressed");
    }
    if (!(currentButtons & MY_BUTTON_R1) && (previousButtons & MY_BUTTON_R1)) {
        Serial.println("R1 released");
    }
    
    // Handle L1 button press/release
    if ((currentButtons & MY_BUTTON_L1) && !(previousButtons & MY_BUTTON_L1)) {
        Serial.println("L1 pressed");
    }
    if (!(currentButtons & MY_BUTTON_L1) && (previousButtons & MY_BUTTON_L1)) {
        Serial.println("L1 released");
    }

    // Handle L2 (brake) and R2 (throttle) values
    int currentBrake = ctl->brake();  // L2
    int currentThrottle = ctl->throttle();  // R2
    
    // Handle L2 (brake) button press/release
    if (currentBrake > 0) {
        Serial.print("L2 pressed, Value: ");
        Serial.println(currentBrake);
    }
    if (currentBrake == 0 && previousBrake > 0) {
        Serial.println("L2 released");
    }

    // Handle R2 (throttle) button press/release
    if (currentThrottle > 0) {
        Serial.print("R2 pressed, Value: ");
        Serial.println(currentThrottle);
    }
    if (currentThrottle == 0 && previousThrottle > 0) {
        Serial.println("R2 released");
    }

    // Handle Axis (Joystick) with calibration
    int currentLeftJoystickX = ctl->axisX()-4;
    int currentLeftJoystickY = ctl->axisY()-4;
    int currentRightJoystickX = ctl->axisRX()-4;
    int currentRightJoystickY = ctl->axisRY()-4;
    
    // Left joystick X and Y (Check if they are idle)
    if (currentLeftJoystickX != 0 || currentLeftJoystickY != 0) {
        Serial.print("Left Joystick X: ");
        Serial.println(currentLeftJoystickX);
        Serial.print("Left Joystick Y: ");
        Serial.println(currentLeftJoystickY);
    }

    // Right joystick X and Y (Check if they are idle)
    if (currentRightJoystickX != 0 || currentRightJoystickY != 0) {
        Serial.print("Right Joystick X: ");
        Serial.println(currentRightJoystickX);
        Serial.print("Right Joystick Y: ");
        Serial.println(currentRightJoystickY);
    }

    // Update previous states
    previousButtons = currentButtons;
    previousDpad = currentDpad;
    previousMiscButtons = currentMisc;
    previousBrake = currentBrake;
    previousThrottle = currentThrottle;

    // Debug info
    //dumpGamepad(ctl);
}



void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
         processGamepad(myController);
      }
      else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
//  const uint8_t addr[6] = {0xAB, 0xAB, 0x77, 0x77, 0x77, 0xA1};  // Assign the specified MAC address
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);

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


}

// Arduino loop function. Runs in CPU 1.
void loop() {
  // This call fetches all the controllers' data.
  // Call this function in your main loop.
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();
           
    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    // vTaskDelay(1);
  delay(30);
}


// ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */, 0x40 /* strongMagnitude */);
// ctl->setColorLED(255, 0, 0)
