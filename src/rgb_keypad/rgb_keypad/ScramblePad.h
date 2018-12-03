#pragma once

#include "Keypad.h"
#include "Common.h"
#include "vector.h"

using namespace std;

class ScramblePad
{
public:
    ScramblePad( Keypad &keypad );
    ~ScramblePad();

    void SetKeyCode( array_ref<uint8_t> code ) { m_keyCode = code; }
    void SetConfirmKey( uint8_t confirm ) { m_confirmKey = confirm; }
    void SetCancelKey( uint8_t cancel ) { m_cancelKey = cancel; }

    //void SetStateChangeListener(void(*listener)());

private:
    void OnKeyChange(const Key &key, const KeyState &prevState, const KeyState &newState );
    void OnCancel();
    void OnFail();
    void OnSuccess();

    Keypad            &m_keyPad;
	array_ref<uint8_t> m_keyCode;
    uint8_t            m_confirmKey     = 255;
    uint8_t            m_cancelKey      = 255;
    size_t             m_matchIndex     = 0;
    uint32_t           m_failedAttempts = 0;
    bool               m_isInvalid      = false;
};