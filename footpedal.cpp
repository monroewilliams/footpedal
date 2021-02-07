#include <Joystick.h>

// Pin assignments for polling.

// Each pair of pins in triplePins array is grouped together and read together to create one of 3 states, which are reported as 3 separate buttons:
// - first grounded
// - second grounded
// - both grounded
// (This matches the functionality of the Digitech FS3X pedal box, as well as several other three button effects foot pedals out there)
const int triplePins[] = { 2, 3 };

// Pins in this array are polled individually and reported as single buttons.
const int singlePins[] = { 4, 5 };

const uint8_t triplesCount = (sizeof(triplePins) / sizeof(triplePins[0]));
const uint8_t singlesCount = (sizeof(singlePins) / sizeof(singlePins[0]));
const int buttonCount = (triplesCount * 3) / 2 + singlesCount;

// USB descriptor for a gamepad with nothing but buttons.
Joystick_ Joystick
(
		JOYSTICK_DEFAULT_REPORT_ID, // hidReportId
		JOYSTICK_TYPE_GAMEPAD, // joystickType
    buttonCount, // buttonCount
		0, // hatSwitchCount
		false, // includeXAxis
		false, // includeYAxis
		false, // includeZAxis
		false, // includeRxAxis
		false, // includeRyAxis
		false, // includeRzAxis
		false, // includeRudder
		false, // includeThrottle
		false, // includeAccelerator
		false, // includeBrake
		false // includeSteering
);

int lastButtonState[buttonCount];

#define SERIAL_DEBUG 0

#if SERIAL_DEBUG
	#define DebugLog(...) Serial.print(__VA_ARGS__)
	#define DebugLogln(...) Serial.println(__VA_ARGS__)
#else
	#define DebugLog(...)
	#define DebugLogln(...)
#endif

void setup() {
#if SERIAL_DEBUG
  Serial.begin(115200);
#endif

  // Initialize Button Pins
  for (uint8_t i = 0; i < triplesCount; i++)
  {
    pinMode(triplePins[i], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < singlesCount; i++)
  {
    pinMode(singlePins[i], INPUT_PULLUP);
  }
  
  // Set up initial button state
  for (uint8_t i = 0; i < buttonCount; i++)
  {
    lastButtonState[i] = 0;
  }

  // Initialize Joystick Library
  Joystick.begin();
}

void loop() {
  int button = 0;

  // Read triplePins pin values by pairs
  for (uint8_t i = 0; i < triplesCount; i += 2)
  {
    int current[3] = {0, 0, 0};
    current[0] = !digitalRead(triplePins[i]);
    current[1] = !digitalRead(triplePins[i + 1]);

    DebugLog(F("("));
    DebugLog(triplePins[i]);
    DebugLog(F(", "));
    DebugLog(triplePins[i + 1]);
    DebugLog(F(") -> ("));
    DebugLog(current[0]);
    DebugLog(F(", "));
    DebugLog(current[1]);
    DebugLog(")  ");

    if (current[0] && current[1])
    {
        current[0] = 0;
        current[1] = 0;
        current[2] = 1;
    }

    for (int j = 0; j < 3; j++)
    {
        if (lastButtonState[button] != current[j])
        {
          Joystick.setButton(button, current[j]);
          lastButtonState[button] = current[j];
        }
        button++;
    }

  }

  // Read singlePins pins individually
  for (uint8_t i = 0; i < singlesCount; i++)
  {
    int current = !digitalRead(singlePins[i]);
    
    DebugLog(singlePins[i]);
    DebugLog(F(" -> "));
    DebugLog(current);
    DebugLog(F("  "));

    if (lastButtonState[button] != current)
    {
      Joystick.setButton(button, current);
      lastButtonState[button] = current;
    }
    button++;
  }

  DebugLogln("");

  delay(10);
}

