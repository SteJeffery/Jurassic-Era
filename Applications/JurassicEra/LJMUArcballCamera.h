#ifndef _LJMU_ARCBALL_H_
#define _LJMU_ARCBALL_H_

#include "Camera.h"
#include "Timer.h"

namespace LJMUDX
{   
	////////////////////////////////
	// Camera Action Mapping
	////////////////////////////////
	enum LJMUArcControls
	{
		ARC_LEFT = 0,
		ARC_RIGHT,
		ARC_UP,
		ARC_DOWN,
		ARC_ZOOM_IN,
		ARC_ZOOM_OUT,
		ARC_KEY_COUNT
	};

	////////////////////////////////
	// Class to Represent our Arc Ball
	// Camera. 
	////////////////////////////////
    class LJMUArcBallCamera : public Glyph3::Camera
    {
		//--------CLASS CONSTANTS------------------------------------
		static const UINT KeyBindings[ARC_KEY_COUNT];

		public:
			//--------CONSTRUCTOR/DESTRUCTOR-----------------------------
			LJMUArcBallCamera();
			virtual ~LJMUArcBallCamera();    

			virtual bool HandleEvent(Glyph3::EventPtr pevt);
			virtual std::wstring GetName();

		protected:
			void update();  
			float wrapAngle(float pangle); 

		protected:
			bool _key_states[ARC_KEY_COUNT];        
			int	 _mouse_prev_x;
			int	 _mouse_prev_y;
			int  _mouse_delta_x;
			int  _mouse_delta_y;
			Glyph3::Timer _timer;
    };
};
#endif