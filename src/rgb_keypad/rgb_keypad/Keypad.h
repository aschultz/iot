// Keypad.h

#ifndef _KEYPAD_h
#define _KEYPAD_h

#include <Arduino.h>
#include "vector.h"

typedef enum
{
    Released,
    Pressed,
    Held,
} KeyState;

class Key
{
public:
    uint8_t         Row;
    uint8_t         Col;
    uint8_t         Index;
    bool            IsPressed   = false;
    unsigned long   StartTime   = 0;
    KeyState        State       = KeyState::Released;
};

/// <summary>
/// Represents a button pad organized as an NxM matrix with row and column pins connected to 
/// </summary>
class Keypad
{
public:
    typedef void(*KeyChangeListener)(Key&, KeyState, KeyState);

    Keypad( array<const uint8_t> &&rowPins, array<const uint8_t> &&colPins );

    bool IsKeyDown( uint8_t row, uint8_t col );
    bool IsKeyDown( uint8_t keyIndex );
    bool IsKeyHeld( uint8_t row, uint8_t col );
    bool IsKeyHeld( uint8_t keyIndex );

    // Registers a callback to receive notifications on key changes. The callback should minimize work so as to avoid
    // interrupting the state polling routine.
    void SetKeyChangeListener( KeyChangeListener listener );

    unsigned int UpdateInterval     = 10;   // Time in ms that must pass before Update will actually do any work. This determines the max time resolution.
    unsigned int DebounceThreshold  = 10;   // Time in ms a button must be in the same state before it will register as a key change
    unsigned int HoldThreshold      = 500;  // Time in ms a button must be held down before is changes to the Held state

    void Update();

    array<Key>& GetKeys() { return m_keys; }

private:
    void ReadPinState();
    void SetKeyState( uint8_t row, uint8_t col, bool pressed );
    void UpdateKeyState();
    void NotifyKeyChange( Key&, KeyState, KeyState );

    array<const uint8_t> m_rowPins;
    array<const uint8_t> m_colPins;
    array<Key>           m_keys;
    unsigned long        m_lastUpdateTime;
    KeyChangeListener    m_keyChangeListener;
};

#endif