#include "LJMUArcballCamera.h"

#include "PCH.h"
#include "EventManager.h"
#include "EvtKeyDown.h"
#include "EvtKeyUp.h"
#include "EvtFrameStart.h"
#include "EvtMouseMove.h"
#include "EvtMouseLeave.h"

#include "LJMUMath.h"

using namespace LJMUDX;

//--------------CONSTANT DEFINITION----------------------------------------------------------
namespace LJMUDX
{
	static const int MOUSE_INVALID = -99999;
}

//------------CONSTRUCTORS/DESTRUCTORS-------------------------------------------------------

///////////////////////////////////////////////
// Constructor for our Arc Ball Camera
///////////////////////////////////////////////
LJMUArcBallCamera::LJMUArcBallCamera() :    
_mouse_prev_x(MOUSE_INVALID),
_mouse_prev_y(MOUSE_INVALID),
_mouse_delta_x(0),
_mouse_delta_y(0)
{
	for (UINT i = 0; i < ARC_KEY_COUNT; ++i)
	{
        this->_key_states[i] = false;
	}
	
	this->RequestEvent(Glyph3::SYSTEM_KEYBOARD_KEYUP);
	this->RequestEvent(Glyph3::SYSTEM_KEYBOARD_KEYDOWN);
	this->RequestEvent(Glyph3::SYSTEM_MOUSE_MOVE);
	this->RequestEvent(Glyph3::SYSTEM_MOUSE_LEAVE);
	this->RequestEvent(Glyph3::RENDER_FRAME_START);
}

///////////////////////////////////////////////
// Destructor for our ArcBall Camera
///////////////////////////////////////////////
LJMUArcBallCamera::~LJMUArcBallCamera()
{

}

//----------------INPUT MAPPING---------------------------------------------------------------

///////////////////////////////////////////////
// Key Mapping for our Camera Controls
// (Correspond to Enum Indices).
///////////////////////////////////////////////
const UINT LJMUArcBallCamera::KeyBindings[ARC_KEY_COUNT] =
{
	'O', 'P', 'Q', 'A', 'Z', 'X'
};

//-----------------EVENT HANDLING INTERFACE---------------------------------------------------

///////////////////////////////////////////////
// Handle our Mouse, Keyboard and Frame Events.
///////////////////////////////////////////////
bool LJMUArcBallCamera::HandleEvent(Glyph3::EventPtr pevt)
{
	using namespace Glyph3; 
    eEVENT e = pevt->GetEventType();

    if (e == SYSTEM_KEYBOARD_KEYDOWN)
    {
        EvtKeyDownPtr tkeydown = std::static_pointer_cast<EvtKeyDown>(pevt);
        UINT tkey = tkeydown->GetCharacterCode();
		for( UINT i = 0; i < ARC_KEY_COUNT; ++i )
		{
            if(tkey == KeyBindings[i])
			{
                this->_key_states[i] = true;
		        return true;
			}
		}
    }
    else if (e == SYSTEM_KEYBOARD_KEYUP)
    {
		EvtKeyUpPtr tkeyup = std::static_pointer_cast<EvtKeyUp>(pevt);
		UINT tkey = tkeyup->GetCharacterCode();

		for (UINT i = 0; i < ARC_KEY_COUNT; ++i)
		{
            if(tkey == KeyBindings[i])
            {
				this->_key_states[i] = false;
			    return true;
			}
		}
    }
    else if (e == SYSTEM_MOUSE_MOVE)
    {
        EvtMouseMovePtr tmm = std::static_pointer_cast<EvtMouseMove>(pevt);  
		this->_mouse_delta_x = 0;
		this->_mouse_delta_y = 0;
        const int tcurrx = tmm->GetX();
        const int tcurry = tmm->GetY();

        if(this->_mouse_prev_x != MOUSE_INVALID && this->_mouse_prev_y != MOUSE_INVALID)
        {
            this->_mouse_delta_x = tcurrx - this->_mouse_prev_x;
            this->_mouse_delta_y = tcurry - this->_mouse_prev_y;
        }
		this->_mouse_prev_x = tcurrx;
		this->_mouse_prev_y = tcurry;
		return true;
    }
    else if (e == SYSTEM_MOUSE_LEAVE)
    {
		this->_mouse_prev_x = MOUSE_INVALID;
		this->_mouse_prev_y = MOUSE_INVALID;
    }
    else if (e == RENDER_FRAME_START)
    {
        this->update();
    }
    return false;
}

//-----------------CAMERA HELPERS------------------------------------------------------------

///////////////////////////////////////////////
// Update the Dynamics of the Camera. 
///////////////////////////////////////////////
void LJMUArcBallCamera::update()
{
    this->_timer.Update();
    float ttpf = this->_timer.Elapsed();    

    float tmovespeed		= 10.0f * ttpf;
    const float trotspeed	= 0.24f * ttpf;    
	
	if (this->_key_states[ARC_LEFT])
	{

	}
	if (this->_key_states[ARC_RIGHT])
	{

	}    
	if (this->_key_states[ARC_UP])
	{

	}
	if (this->_key_states[ARC_DOWN])
	{

	}
	if (this->_key_states[ARC_ZOOM_IN])
	{

	}       
	if (this->_key_states[ARC_ZOOM_OUT])
	{

	}
}

///////////////////////////////////////////////
// Convert Angle to range -PI to PI
///////////////////////////////////////////////
float LJMUArcBallCamera::wrapAngle(float pangle)
{
	float twrap;
	pangle = pangle + PI;
	twrap = fabsf(pangle);
	twrap = twrap - (TWO_PI * (float)((int)(twrap / TWO_PI)));
	twrap = twrap - PI;
	if (pangle < 0.0f)
		twrap = -twrap;
	return twrap;
}

//-------------COMPONENT NAME IMPLEMENTATION--------------------------------------------------

///////////////////////////////////////////////
// Get the Name of this Camera Component.
///////////////////////////////////////////////
std::wstring LJMUArcBallCamera::GetName()
{
	return L"LJMUArcBallCamera";
}
