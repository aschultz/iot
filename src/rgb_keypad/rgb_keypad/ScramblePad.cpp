#include "ScramblePad.h"


ScramblePad::ScramblePad( Keypad &keypad )
    : m_keyPad( keypad )
{
	m_keyPad.KeyChangedEvent = delegate<void(const Key&,const KeyState&,const KeyState&)>::create<ScramblePad,&ScramblePad::OnKeyChange>(this);
}

ScramblePad::~ScramblePad()
{
}

void ScramblePad::OnCancel()
{
    m_matchIndex = 0;
    m_isInvalid = false;
}

void ScramblePad::OnFail()
{
    ++m_failedAttempts;
    m_matchIndex = 0;
    m_isInvalid = false;
}

void ScramblePad::OnSuccess()
{

}

void ScramblePad::OnKeyChange(const Key &key, const KeyState &prevState, const KeyState &newState)
{
    // User pressed, released, or held a button.
    // 
    // 1. If user pressed Cancel, reset
    // 2. If user pressed Confirm, check for success
    // 3. If user pressed anything else, check for match and increment position in match

    if (key.Index == m_cancelKey)
    {        
        OnCancel();
    }
    else if (key.Index == m_confirmKey)
    {
        if (m_isInvalid || m_matchIndex != m_keyCode.size())
        {
            // Bad keycode
            OnFail();
        }
        else
        {
            // Correct keycode
            OnSuccess();
        }
    }
    else
    {
        if (m_isInvalid ||
            m_matchIndex >= m_keyCode.size() ||
            key.Index != m_keyCode[m_matchIndex])
        {
            // Entered invalid or too many keys. Mark the attempt as invalid,
            // but don't notify failure yet so that we obscure the key length and value.
            m_isInvalid = true;
        }
        else
        {
            // Correct key
            ++m_matchIndex;
        }
    }
}