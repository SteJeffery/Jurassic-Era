#pragma once

#include "Application.h"
#include "FileSystem.h"
#include "Win32RenderWindow.h"
#include "RendererDX11.h"
#include "ViewPerspective.h"
#include "Camera.h"
#include "Scene.h"
#include "PointLight.h"

#include "GeometryActor.h"
#include "TextActor.h"
#include "TextureActor.h"
#include "SkyBoxActor.h"

//STL Includes
#include <vector>

//LJMU Framework Includes
#include "LJMUTwoDOverlay.h"
#include "LJMUH3Util.h"
#include "LJMUShaderMaker.h"
#include "LJMUMateriaParams.h"

using namespace Glyph3;

//TYPE DEFINITIONS OF OUR MESH COMPRISED OF BASIC VERTEX OBJECTS
typedef std::shared_ptr<Glyph3::DrawExecutorDX11<Glyph3::BasicVertexDX11::Vertex>> BasicMeshPtr;

namespace LJMUDX
{
	struct LJMUDefaultResources
	{
		ResourcePtr		DefaultTexture;
		SpriteFontPtr	DefaultFont;
	};

	//////////////////////////////////////
	//Class Application for a DirectX 11
	//Driven Application using the DirectX Toolkit
	//Hieroglyph 3 Rendering Engine and LUA.
	//
	//
	//			
	//////////////////////////////////////
	class LJMULevelDemo : public Application //Inherit from the Hieroglyph Base Class
	{
	public:
		//------------CONSTRUCTORS------------------------------------------------
		LJMULevelDemo() :_active_cam(0){};

	public:
		//------------INHERITED METHODS-------------------------------------------
		virtual void Initialize();	//Initialise the DirectX11 Scene
		virtual void Update();		//Update the DirectX Scene
		virtual void Shutdown();	//Shutdown the DirectX11 Scene

		virtual bool ConfigureEngineComponents();	//Initialise Hieroglyph and DirectX TK Modules
		virtual void ShutdownEngineComponents();	//Destroy Hieroglyph and DirectX TK Modules

		virtual bool HandleEvent(EventPtr pEvent);	//Handle an I/O Event
		virtual std::wstring GetName();				//Get the Name of this App Instance
		virtual void TakeScreenShot();

	protected:
		//------------CUSTOM METHODS-----------------------------------------------
		void loadSharedResources();					//Stage to setup our Content Manager
		void loadContent2D();						//Stage to setup our 2D Sprites and 2D Text
		void loadContent3D();						//Stage to setup our 3D Actors and 3D Text
		void loadCameras();							//Setup our Virtual Camera Objects
		void loadViews();							//Stage to setup our Virtual Cameras (Both 2D and 3D)
		void loadToScene();							//Stage to add our Object Vectors to the Scene
		void setupInitialState();					//setup the Initial State of our Virtual Scene (Apply Static Transforms)

		void update2D();							//Separate the Transformation Updates of our Sprites
		void update3D();							//Separate the Transformation Updates of our 3D Objects

		void render2D();							//Separate the Rendering Calls of the 2D Content
		void render3D();							//Separate the Rendering Calls of the 3D Content

		void UpdateLight();

		std::wstring outputFPSInfo();				//Convert the timer's Frames Per Second to a formatted string
		std::wstring outputCameraInfo();

		void UpdateHuman();
		void UpdateReptile();
		void UpdateTrex();
		void UpdatePteranodon();

		//-------------HELPER METHODS----------------------------------------------
		BasicMeshPtr generateOBJMesh(std::wstring pmesh, Vector4f pcolour = Vector4f(0, 0, 0, 1));

	public:
		//-------------CONSTANTS---------------------------------------------------
		static const int			SCREEN_WIDTH;
		static const int			SCREEN_HEIGHT;
		static const Vector4f		SCREEN_BG_COLOUR;

		//----------------Object Animation Control----------------------------------
		static int                                object_1_direction;
		static int                                object_2_direction;
		static int                                object_3_direction;
		static float                              acceleration;
		static float                              speed;
		static unsigned int				          _curr_obj;

	protected:
		//-------------CLASS MEMBERS-----------------------------------------------
		RendererDX11*			_obj_renderer11;	//Pointer to our DirectX 11 Device
		Win32RenderWindow*		_obj_window;		//Pointer to our Windows-Based Window

		int						_swap_index;		//Index of our Swap Chain 
		ResourcePtr				_tgt_render;		//Pointer to the GPU Render Target for Colour
		ResourcePtr				_tgt_depth;			//Pointer to the GPU Render Target for Depth
		Vector4f				_bg_clr;

		FileSystem              _fs;
		//--------------HIEROGLYPH OBJECTS-----------------------------------------
		ViewPerspective*					_render_view;
		LJMUTwoDOverlay*					_obj_twod_rdr;
		LJMUResourceHelper					_local_content;
		LJMUDefaultResources				_default_res;

		std::vector<Camera*>				_list_cameras;
		std::vector<Camera*>				_list_ortho;
		int									_active_cam;

		std::vector<BasicMeshPtr>			_list_meshes;
		std::vector<LJMUMaterialParams*>	_list_params;

		std::vector<GeometryActor*>			_list_floors;
		std::vector<Actor*>					_list_models;
		std::vector<TextActor*>				_list_text_3D;
		std::vector<TextureActor*>			_list_panel_3D;
		SkyboxActor*						_skybox;

		std::vector<PointLight*>			_list_lights;

		std::vector<LJMUSpriteEntry>		_list_sprites;
		std::vector<LJMUTextEntry>			_list_text_2D;
		std::vector<LJMUSpriteEntry>		_list_bg;
	};
}