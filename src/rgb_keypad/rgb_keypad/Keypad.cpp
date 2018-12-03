// 
// 
// 

#include "Keypad.h"
#include <new.h>

Keypad::Keypad(array_ref<uint8_t> rowPins, array_ref<uint8_t> colPins)
    : m_rowPins(rowPins),
      m_colPins(colPins),
      m_keys(rowPins.size() * colPins.size())
{
    for (uint8_t row = 0; row < m_rowPins.size(); ++row)
    {
        for (uint8_t col = 0; col < m_colPins.size(); ++col)
        {
            uint8_t i = row*m_colPins.size() + col;
            Key& key = m_keys[i];
            key.Row = row;
            key.Col = col;
            key.Index = i;
        }
    }
}

bool Keypad::IsKeyDown(uint8_t row, uint8_t col)
{
    Key& key = m_keys[row*m_colPins.size() + col];
    return key.State == KeyState::Pressed || key.State == KeyState::Held;
}

bool Keypad::IsKeyDown(uint8_t keyIndex)
{
    Key& key = m_keys[keyIndex];
    return key.State == KeyState::Pressed || key.State == KeyState::Held;
}

bool Keypad::IsKeyHeld(uint8_t row, uint8_t col)
{
    Key& key = m_keys[row*m_colPins.size() + col];
    return key.State == KeyState::Held;
}

bool Keypad::IsKeyHeld(uint8_t keyIndex)
{
    Key& key = m_keys[keyIndex];

    return key.State == KeyState::Held;
}

void Keypad::NotifyKeyChange(const Key &key, const KeyState &prevState, const KeyState &newState)
{
    if (KeyChangedEvent != nullptr)
    {
		KeyChangedEvent.operator()(key, prevState, newState);
    }
}

void Keypad::Update()
{
    // Update at reduced interval to avoid wasting cycles
    if ((millis() - m_lastUpdateTime) >= UpdateInterval)
    {
        ReadPinState();
        UpdateKeyState();
        m_lastUpdateTime = millis();
    }
}

void Keypad::ReadPinState()
{
    //
    // We set Row pins to INPUT_PULLUP, which connects
    // them to VCC via a resistor (typically between 20k and 150k)
    //
    // We set Col pins to INPUT, which leaves them floating
    //
    // We then walk through each column and change it to OUTPUT LOW. This brings
    // it to ground. We read the state of each Row pin. If a button is pressed,
    // it will connect VCC to GND and we will read LOW. If a button is not pressed,
    // it will report as HIGH because its only connected to VCC.
    //
    // After testing a column, we set it back to INPUT so we can test the next column.
    // (If we didn't, our readings would be affected by button presses in other columns).
    //
	// Example:
	//	
	//	VCC---$---RowPin1--------BTN--------BTN--------PRESS
	//							  |			 |			|
	//							  |			 |			|
	//	VCC---$---RowPin2--------BTN--------PRESS------BTN
	//							  |			 |			|
	//							  |			 |			|
	//							ColPin1	   ColPin2	  ColPin3
	//							  |			 |			|
	//							  \			GND			\
	//
	// In this example, we're testing whether any buttons in column 2 are pressed (ColPin2 is set to LOW)
	// RowPin1 will read as HIGH
	// RowPin2 will read as LOW
	//

    // Connect rows to VCC via built-in resistor
    for (uint8_t row = 0; row < m_rowPins.size(); ++row)
    {
        pinMode(m_rowPins[row], INPUT_PULLUP);
    }
    // Disconnect columns
    for (uint8_t col = 0; col < m_colPins.size(); ++col)
    {
        pinMode(m_colPins[col], INPUT);
    }
    
    for (uint8_t col = 0; col < m_colPins.size(); ++col)
    {
        uint8_t colPin = m_colPins[col];

        // Bring column to ground
        pinMode(colPin, OUTPUT);
        digitalWrite(colPin, LOW);

        // Read each row. If the value is LOW, then
        // the button is pressed (connecting row to col GND).
        for (uint8_t row = 0; row < m_rowPins.size(); ++row)
        {
            bool pressed = digitalRead(m_rowPins[row]) == LOW;

            SetKeyState( row, col, pressed );            
        }

        // Disconnect column from ground
        pinMode(colPin, INPUT);
    }
}

void Keypad::SetKeyState( uint8_t row, uint8_t col, bool pressed )
{
    Key& key = m_keys[row*m_colPins.size() + col];

    if (key.IsPressed != pressed)
    {
        key.IsPressed = pressed;
        key.StartTime = millis();
    }
}

void Keypad::UpdateKeyState()
{
    unsigned long currentTime = millis();

    //
    // Update state machine for each key.
    // We only change states if a key has maintained its position beyond a threshold duration.
    //
    for (uint8_t i = 0; i < m_keys.size(); ++i)
    {
        Key& key = m_keys[i];
        KeyState prevState = key.State;

        if (prevState == KeyState::Released)
        {
            if (key.IsPressed && (currentTime - key.StartTime) >= DebounceThreshold)
            {
                key.State = KeyState::Pressed;
                NotifyKeyChange( key, prevState, key.State);
            }
        }
        else if (prevState == KeyState::Pressed)
        {
            if (key.IsPressed && (currentTime - key.StartTime) >= HoldThreshold)
            {
                key.State = KeyState::Held;
                NotifyKeyChange( key, prevState, key.State );
            }
            else if (!key.IsPressed && (currentTime - key.StartTime) >= DebounceThreshold)
            {
                key.State = KeyState::Released;
                NotifyKeyChange( key, prevState, key.State );
            }
        }
        else if (prevState == KeyState::Held)
        {
            if (!key.IsPressed && (currentTime - key.StartTime) >= DebounceThreshold)
            {
                key.State = KeyState::Released;
                NotifyKeyChange( key, prevState, key.State );
            }
        }
    }
}