//------------Include the Application Header File----------------------------
#include "LJMULevelDemo.h"

//------------DX TK AND STD/STL Includes-------------------------------------
#include <sstream>
#include <iomanip>

//------------Include Hieroglyph Engine Files--------------------------------
#include "Log.h"
#include "EventManager.h"
#include "EvtFrameStart.h"
#include "EvtChar.h"
#include "EvtKeyUp.h"
#include "EvtKeyDown.h"
#include "PipelineManagerDX11.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"

//-------------Include Custom Framework Includes-----------------------------
#include "LJMUFPSCamera.h"
#include "LJMUMeshOBJ.h"

using namespace LJMUDX;

//---------CONSTANTS----------------------------------------------------------
const int	   LJMULevelDemo::SCREEN_WIDTH = 1600;
const int	   LJMULevelDemo::SCREEN_HEIGHT = 900;
const Vector4f LJMULevelDemo::SCREEN_BG_COLOUR = Vector4f(0.4f, 0.3f, 0.8f, 1.0f);
int            LJMULevelDemo::object_1_direction = 0;
int            LJMULevelDemo::object_2_direction = 0;
int            LJMULevelDemo::object_3_direction = 0;
float          LJMULevelDemo::acceleration = 0.0001;
float          LJMULevelDemo::speed = 0.0001;
unsigned int   LJMULevelDemo::_curr_obj = 0;

//---------INTIALISATION METHODS------------------------------------------------------------

/////////////////////////////////////
// Assemble our Input Layouts for this
// Stage of the Pipeline.
/////////////////////////////////////
void LJMULevelDemo::loadSharedResources()
{
	//-----SETUP OUR GEOMETRY FOR THIS SCENE-----------------------------------------
	this->_default_res.DefaultTexture = RendererDX11::Get()->LoadTexture(std::wstring(L"blank.png"));
	this->_default_res.DefaultFont = SpriteFontLoaderDX11::LoadFont(std::wstring(L"Arial"), 32, 0, false);

	//-----SETUP OUR RESOURCES USING THE RESOURCE MANAGER----------------------------
	this->_local_content.loadFont(0, std::wstring(L"Calibri"), 32);
	this->_local_content.loadImage(1, std::wstring(L"floor.png"));
	this->_local_content.loadImage(2, std::wstring(L"ljmu.jpg"));
	this->_local_content.loadImage(100, std::wstring(L"reptile_diff.png"));
	this->_local_content.loadImage(101, std::wstring(L"bull_diff.png"));
	this->_local_content.loadImage(102, std::wstring(L"human_diff.png"));
	this->_local_content.loadImage(103, std::wstring(L"trex_diff.png"));
	this->_local_content.loadImage(200, std::wstring(L"mountains.dds"));
}

/////////////////////////////////////////////
// Setup our Sprites and Fonts.
/////////////////////////////////////////////
void LJMULevelDemo::loadContent2D()
{
	Matrix4f tmatrix;
	tmatrix.MakeIdentity();
}

//////////////////////////////////////////////
// Setup our 3D Scene and Lights
//////////////////////////////////////////////
void LJMULevelDemo::loadContent3D()
{
	//-----------POPULATE LEVEL FLOOR--------------------------------------------------------------
	GeometryActor* tboard = new GeometryActor();
	tboard->UseTexturedMaterial(this->_local_content.getImage(1));
	tboard->SetColor(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	tboard->DrawRect(Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector3f(0, 0, -1), Vector2f(50, 50), Vector2f(4.0f, 4.0f));
	this->_list_floors.push_back(tboard);


	//------------POPULATE 3D OBJECTS--------------------------------------------------------------
	BasicMeshPtr tmesh = this->generateOBJMesh(L"reptile.obj", Vector4f(1, 1, 1, 1));
	this->_list_meshes.push_back(tmesh);

	BasicMeshPtr tmesh2 = this->generateOBJMesh(L"test.obj", Vector4f(1, 1, 1, 1));
	this->_list_meshes.push_back(tmesh2);

	BasicMeshPtr tmesh3 = this->generateOBJMesh(L"human.obj", Vector4f(1, 1, 1, 1));
	this->_list_meshes.push_back(tmesh3);

	BasicMeshPtr tmesh4 = this->generateOBJMesh(L"trex.obj", Vector4f(1, 1, 1, 1));
	this->_list_meshes.push_back(tmesh4);


	for (int i = 0; i < 4; i++)
	{
	int ttexindex = 100;
	Actor* tmeshactor = new Actor();
	MaterialPtr tmtl = LJMUShaderMaker::MakeActorPhongTex(*this->_obj_renderer11);
	tmtl->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(ttexindex));
	tmeshactor->GetBody()->SetGeometry(this->_list_meshes[0]);
	tmeshactor->GetBody()->SetMaterial(tmtl);
	this->_list_models.push_back(tmeshactor);
	}

	for (int i = 0; i < 3; i++)
	{
		int ttexindex2 = 101;
		Actor* tmeshactor2 = new Actor();
		MaterialPtr tmtl2 = LJMUShaderMaker::MakeActorPhongTex(*this->_obj_renderer11);
		tmtl2->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(ttexindex2));
		tmeshactor2->GetBody()->SetGeometry(this->_list_meshes[1]);
		tmeshactor2->GetBody()->SetMaterial(tmtl2);
		this->_list_models.push_back(tmeshactor2);
	}

	for (int i = 0; i < 4; i++)
	{
		int ttexindex3 = 102;
		Actor* tmeshactor3 = new Actor();
		MaterialPtr tmtl3 = LJMUShaderMaker::MakeActorPhongTex(*this->_obj_renderer11);
		tmtl3->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(ttexindex3));
		tmeshactor3->GetBody()->SetGeometry(this->_list_meshes[2]);
		tmeshactor3->GetBody()->SetMaterial(tmtl3);
		this->_list_models.push_back(tmeshactor3);
	}
	
	for (int i = 0; i < 4; i++)
	{
		int ttexindex4 = 103;
		Actor* tmeshactor4 = new Actor();
		MaterialPtr tmtl4 = LJMUShaderMaker::MakeActorPhongTex(*this->_obj_renderer11);
		tmtl4->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(ttexindex4));
		tmeshactor4->GetBody()->SetGeometry(this->_list_meshes[3]);
		tmeshactor4->GetBody()->SetMaterial(tmtl4);
		this->_list_models.push_back(tmeshactor4);
	}

	//-------------SKYBOX OBJECT-------------------------------------------------------------------
	this->_skybox = new SkyboxActor(this->_local_content.getImage(200), 10000.0f);

	//-------------POPULATE LIGHTS-----------------------------------------------------------------
	PointLight* tlight = new PointLight();
	tlight->SetAmbient(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	tlight->SetDiffuse(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	tlight->SetSpecular(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	tlight->GetNode()->Position() = Vector3f(0.0f, 100.0f, 0.0f);

	this->_list_lights.push_back(tlight);
}

///////////////////////////////////////////////
// Add our Sequence Container Contents to the
// Scene Object.  
///////////////////////////////////////////////
void LJMULevelDemo::loadToScene()
{
	for (auto it = this->_list_floors.begin(); it != this->_list_floors.end(); ++it)
	{
		this->m_pScene->AddActor((*it));
	}

	for (auto it = this->_list_panel_3D.begin(); it != this->_list_panel_3D.end(); ++it)
	{
		this->m_pScene->AddActor((*it));
	}

	for (auto it = this->_list_text_3D.begin(); it != this->_list_text_3D.end(); ++it)
	{
		this->m_pScene->AddActor((*it));
	}

	for (auto it = this->_list_lights.begin(); it != this->_list_lights.end(); ++it)
	{
		this->m_pScene->AddLight((*it));
	}

	for (auto it = this->_list_models.begin(); it != this->_list_models.end(); ++it)
	{
		this->m_pScene->AddActor((*it));
	}

	this->m_pScene->AddActor(this->_skybox);
}

///////////////////////////////////////////////
// Setup our 2D and 3D Camera Lists
///////////////////////////////////////////////
void LJMULevelDemo::loadCameras()
{
	float twidth = this->_obj_window->GetWidth();
	float theight = this->_obj_window->GetHeight();

	LJMUFPSCamera* tcam = new LJMUFPSCamera(twidth / 2, theight / 2);
	tcam->SetEventManager(&this->EvtManager);
	tcam->SetProjectionParams(0.1f, 1000.0f, twidth / theight, static_cast<float>(GLYPH_PI) / 3.0f);
	this->_list_cameras.push_back(tcam);

	Camera* tcam2d = new Camera();
	this->_list_ortho.push_back(tcam2d);
}

////////////////////////////////////////
// Setup our Render Target Views
////////////////////////////////////////
void LJMULevelDemo::loadViews()
{
	this->_render_view = new ViewPerspective(*this->_obj_renderer11, this->_tgt_render, this->_tgt_depth);
	this->_render_view->SetBackColor(LJMULevelDemo::SCREEN_BG_COLOUR);
	this->_render_view->SetDebugViewEnabled(true);

	for (auto it = this->_list_cameras.begin(); it != this->_list_cameras.end(); ++it)
	{
		(*it)->SetCameraView(this->_render_view);
		this->m_pScene->AddCamera((*it));
	}

	this->_obj_twod_rdr = new LJMUTwoDOverlay(*this->_obj_renderer11, this->_tgt_render, this->_default_res.DefaultTexture, this->_default_res.DefaultFont);

	for (auto it = this->_list_ortho.begin(); it != this->_list_ortho.end(); ++it)
	{
		(*it)->SetOverlayView(this->_obj_twod_rdr);
		this->m_pScene->AddCamera((*it));
	}
}

////////////////////////////////////
// Setup the Transformations on the Cameras
// and the 2D and 3D Scene Objects
////////////////////////////////////
void LJMULevelDemo::setupInitialState()
{
	float ttpf = this->m_pTimer->Elapsed();

	Vector3f tscale(0.1f, 0.1f, 0.1f);
	Vector3f tscale2(0.005f, 0.005f, 0.005f);

	//Setup the Camera
	Vector3f tcamerapos(0.0f, 10.0f, -50.0f);
	this->_list_cameras[0]->Spatial().SetTranslation(tcamerapos);

	//Setup the Lizard Models
	this->_list_models[0]->GetBody()->Scale() = tscale;
	this->_list_models[0]->GetBody()->Position() = Vector3f(-45, 0, -45);

	this->_list_models[1]->GetBody()->Scale() = tscale;
	this->_list_models[1]->GetBody()->Position() = Vector3f(45, 0, -45);

	this->_list_models[2]->GetBody()->Scale() = tscale;
	this->_list_models[2]->GetBody()->Position() = Vector3f(-45, 0, 45);

	this->_list_models[3]->GetBody()->Scale() = tscale;
	this->_list_models[3]->GetBody()->Position() = Vector3f(45, 0, 45);

	//Setup the Pteranodon Models
	this->_list_models[4]->GetBody()->Scale() = tscale2;
	this->_list_models[4]->GetBody()->Position() = Vector3f(-5, 20, 10);

	this->_list_models[5]->GetBody()->Scale() = tscale2;
	this->_list_models[5]->GetBody()->Position() = Vector3f(0, 30, -10);

	this->_list_models[6]->GetBody()->Scale() = tscale2;
	this->_list_models[6]->GetBody()->Position() = Vector3f(5, 40, 10);

	//Setup the Human Models
	this->_list_models[7]->GetBody()->Scale() = tscale;
	this->_list_models[7]->GetBody()->Position() = Vector3f(0, 0, 10);

	this->_list_models[8]->GetBody()->Scale() = tscale;
	this->_list_models[8]->GetBody()->Position() = Vector3f(0, 0, -10);

	this->_list_models[9]->GetBody()->Scale() = tscale;
	this->_list_models[9]->GetBody()->Position() = Vector3f(10, 0, 0);

	this->_list_models[10]->GetBody()->Scale() = tscale;
	this->_list_models[10]->GetBody()->Position() = Vector3f(-10, 0, 0);

	//Setup the T-Rex Models
	this->_list_models[11]->GetBody()->Scale() = tscale;
	this->_list_models[11]->GetBody()->Position() = Vector3f(0, 0, 30);

	this->_list_models[12]->GetBody()->Scale() = tscale;
	this->_list_models[12]->GetBody()->Position() = Vector3f(0, 0, -30);

	this->_list_models[13]->GetBody()->Scale() = tscale;
	this->_list_models[13]->GetBody()->Position() = Vector3f(30, 0, 0);

	this->_list_models[14]->GetBody()->Scale() = tscale;
	this->_list_models[14]->GetBody()->Position() = Vector3f(-30, 0, 0);
}

//------------------------UPDATE METHODS-----------------------------------------------------------------

//////////////////////////////////
//Handle Input Events in the Application
//////////////////////////////////
bool LJMULevelDemo::HandleEvent(EventPtr pevent)
{
	eEVENT e = pevent->GetEventType();

	if (e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EvtKeyDownPtr tkey_down = std::static_pointer_cast<EvtKeyDown>(pevent);
		unsigned int  tkeycode = tkey_down->GetCharacterCode();
	}
	else if (e == SYSTEM_KEYBOARD_KEYUP)
	{
		EvtKeyUpPtr tkey_up = std::static_pointer_cast<EvtKeyUp>(pevent);
		unsigned int tkeycode = tkey_up->GetCharacterCode();
	}

	return(Application::HandleEvent(pevent));
}



/////////////////////////////////////////
// Update the 2D Content of our Game Application.
/////////////////////////////////////////
void LJMULevelDemo::update2D()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	Matrix4f ttextpos = Matrix4f::Identity();
	float tx = 30.0f;	float ty = 10.0f;
	static Vector4f tblackclr(0.0f, 0.0f, 0.1f, 1.0f);
	static Vector4f tredclr(1.0f, 0.0f, 0.0f, 1.0f);

	ttextpos.SetTranslation(Vector3f(tx, ty, 0.0f));
	this->_obj_twod_rdr->drawText(this->outputFPSInfo(), ttextpos, tblackclr);

	Matrix4f ttextpos2 = Matrix4f::Identity();
	float tx2 = this->_obj_window->GetWidth() - 380.0f;
	float ty2 = 10.0f;

	ttextpos2.SetTranslation(Vector3f(tx2, ty2, 0.0f));
	this->_obj_twod_rdr->drawText(this->outputCameraInfo(), ttextpos2, tblackclr);
}

///////////////////////////////////////
// Render 2D Scene Content
///////////////////////////////////////
void LJMULevelDemo::render2D()
{
	for (auto it = this->_list_bg.begin(); it != this->_list_bg.end(); ++it)
	{
		this->_obj_twod_rdr->drawSprite(*it);
	}

	for (auto it = this->_list_sprites.begin(); it != this->_list_sprites.end(); ++it)
	{
		this->_obj_twod_rdr->drawSprite(*it);
	}

	for (auto it = this->_list_text_2D.begin(); it != this->_list_text_2D.end(); ++it)
	{
		this->_obj_twod_rdr->drawText(*it);
	}
}

//////////////////////////////////////////
// Update the 3D Content of our Game Application
//////////////////////////////////////////
void LJMULevelDemo::update3D()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	UpdateHuman();
	UpdatePteranodon();
	UpdateReptile();
	UpdateTrex();

}

///////////////////////////////////
// Render 3D Scene Content
///////////////////////////////////
void LJMULevelDemo::render3D()
{
	this->_render_view->SetProjMatrix(this->_list_cameras[this->_active_cam]->ProjMatrix());
	this->m_pScene->Update(m_pTimer->Elapsed());
	this->m_pScene->Render(this->_obj_renderer11);
}

//-------------HELPER METHODS--------------------------------------------------

BasicMeshPtr LJMULevelDemo::generateOBJMesh(std::wstring pmeshname, Vector4f pmeshcolour)
{
	LJMUDX::LJMUMeshOBJ* tmesh = new LJMUDX::LJMUMeshOBJ(this->_fs.GetModelsFolder() + pmeshname);
	int tvertcount = tmesh->positions.size();

	auto tia = std::make_shared<DrawExecutorDX11<BasicVertexDX11::Vertex>>();
	tia->SetLayoutElements(BasicVertexDX11::GetElementCount(), BasicVertexDX11::Elements);
	tia->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tia->SetMaxVertexCount(tvertcount);

	BasicVertexDX11::Vertex tv;
	tv.color = pmeshcolour;

	for (auto& tobject : tmesh->objects)
	{
		for (auto& tface : tobject.faces)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				tv.position = tmesh->positions[tface.PositionIndices[i]];
				tv.normal = tmesh->normals[tface.NormalIndices[i]];
				tv.texcoords = tmesh->coords[tface.CoordIndices[i]];
				tia->AddVertex(tv);
			}
		}
	}
	return tia;
}

//-------------APPLICATION INTERFACE METHODS-----------------------------------

///////////////////////////////////
// Configure the DirectX 11 Programmable
// Pipeline Stages and Create the Window
// Calls 
///////////////////////////////////
bool LJMULevelDemo::ConfigureEngineComponents()
{
	// Set the render window parameters and initialize the window
	this->_obj_window = new Win32RenderWindow();
	this->_obj_window->SetPosition(0, 0);
	this->_obj_window->SetSize(LJMULevelDemo::SCREEN_WIDTH, LJMULevelDemo::SCREEN_HEIGHT);
	this->_obj_window->SetCaption(this->GetName());
	this->_obj_window->Initialize(this);

	// Create the renderer and initialize it for the desired device type and feature level.
	this->_obj_renderer11 = new RendererDX11();

	if (!this->_obj_renderer11->Initialize(D3D_DRIVER_TYPE_HARDWARE, D3D_FEATURE_LEVEL_11_0))
	{
		ShowWindow(this->_obj_window->GetHandle(), SW_HIDE);
		MessageBox(this->_obj_window->GetHandle(), L"Could not create a hardware Direct3D 11 device!", L"5108COMP -- Camera Control", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		this->RequestTermination();
		return(false);
	}

	// Create a swap chain for the window that we started out with.
	SwapChainConfigDX11 tconfig;
	tconfig.SetWidth(this->_obj_window->GetWidth());
	tconfig.SetHeight(this->_obj_window->GetHeight());
	tconfig.SetOutputWindow(this->_obj_window->GetHandle());
	this->_swap_index = this->_obj_renderer11->CreateSwapChain(&tconfig);
	this->_obj_window->SetSwapChain(this->_swap_index);

	//Create Colour and Depth Buffers
	this->_tgt_render = this->_obj_renderer11->GetSwapChainResource(this->_swap_index);
	Texture2dConfigDX11 tdepthconfig;
	tdepthconfig.SetDepthBuffer(LJMULevelDemo::SCREEN_WIDTH, LJMULevelDemo::SCREEN_HEIGHT);
	this->_tgt_depth = this->_obj_renderer11->CreateTexture2D(&tdepthconfig, 0);

	// Bind the swap chain render target and the depth buffer for use in rendering.  
	this->_obj_renderer11->pImmPipeline->ClearRenderTargets();
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0, this->_tgt_render->m_iResourceRTV);
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.DepthTargetViews.SetState(this->_tgt_depth->m_iResourceDSV);
	this->_obj_renderer11->pImmPipeline->ApplyRenderTargets();

	D3D11_VIEWPORT tviewport;
	tviewport.Width = static_cast< float >(LJMULevelDemo::SCREEN_WIDTH);
	tviewport.Height = static_cast< float >(LJMULevelDemo::SCREEN_HEIGHT);
	tviewport.MinDepth = 0.0f;
	tviewport.MaxDepth = 1.0f;
	tviewport.TopLeftX = 0;
	tviewport.TopLeftY = 0;

	int tvpindex = this->_obj_renderer11->CreateViewPort(tviewport);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.ViewportCount.SetState(1);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.Viewports.SetState(0, tvpindex);
	return(true);
}

////////////////////////////////////
// Initialise our DirectX 3D Scene
////////////////////////////////////
void LJMULevelDemo::Initialize()
{
	this->loadSharedResources();
	this->loadContent2D();
	this->loadContent3D();
	this->loadToScene();
	this->loadCameras();
	this->loadViews();
	this->setupInitialState();
}

///////////////////////////////////
// Update the State of our Game and 
// Output the Results to Screen (Render)
/////////////////////////////////// 
void LJMULevelDemo::Update()
{
	this->m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr(new EvtFrameStart(this->m_pTimer->Elapsed())));

	//----------START RENDERING---------------------------------------------------------
	this->UpdateLight();

	this->update2D();
	this->update3D();

	this->render2D();
	this->render3D();

	//--------END RENDERING-------------------------------------------------------------
	this->_obj_renderer11->Present(this->_obj_window->GetHandle(), this->_obj_window->GetSwapChain());
}

//////////////////////////////////
// Destroy Resources created by the engine
//////////////////////////////////
void LJMULevelDemo::ShutdownEngineComponents()
{
	if (this->_obj_renderer11)
	{
		this->_obj_renderer11->Shutdown();
		delete this->_obj_renderer11;
	}

	if (this->_obj_window)
	{
		this->_obj_window->Shutdown();
		delete this->_obj_window;
	}
}

//////////////////////////////////
// Shutdown the Application
//////////////////////////////////
void LJMULevelDemo::Shutdown()
{
	//NOTHING TO DO HERE
}

///////////////////////////////////
// Implement the Interface
///////////////////////////////////
void LJMULevelDemo::TakeScreenShot()
{
	//NOTHING TO DO HERE
}

/////////////////////////////////////
// Get Name of the Application
/////////////////////////////////////
std::wstring LJMULevelDemo::GetName()
{
	return(std::wstring(L"5108COMP Artefact Coursework"));
}

void LJMULevelDemo::UpdateLight()
{
	this->_list_lights[0]->SetAmbient(Vector4f(0.4f, 0.4f, 0.4f, 1.0f));
	this->_list_lights[0]->SetDiffuse(Vector4f(0.9f, 0.9f, 0.9f, 1.0f));
	this->_list_lights[0]->SetSpecular(Vector4f(0.7f, 0.7f, 0.7f, 1.0f));
}

std::wstring LJMULevelDemo::outputFPSInfo()
{
	std::wstringstream out;
	out << L"FPS: " << m_pTimer->Framerate();
	return out.str();
}

std::wstring LJMULevelDemo::outputCameraInfo()
{
	std::wstringstream out;
	out << L"Camera X Position: " << _list_cameras[0]->Spatial().GetTranslation().x;
	out << L"\nCamera Y Position: " << _list_cameras[0]->Spatial().GetTranslation().y;
	out << L"\nCamera Z Position: " << _list_cameras[0]->Spatial().GetTranslation().z;
	return out.str();
}

void LJMULevelDemo::UpdateHuman()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	// Linear Interpolation

	static float tposx = 0;
	static float tposy = 0;
	static float tposz = 0;

	//Given Point
	Vector3f point(5.0f, 5.0f, 5.0f);

	if (tposx <= point.x && tposx >= 0.0)
	{
		if (this->object_1_direction == 0)
		{
			this->speed += this->acceleration;
			tposx += this->speed;
			tposy += this->speed;
			tposz += this->speed;
		}

		else if (this->object_1_direction == 1)
		{
			this->speed -= this->acceleration;
			tposx -= this->speed;
			tposy -= this->speed;
			tposz -= this->speed;
		}

	}

	if (tposx > point.x)
	{
		tposx -= this->speed;
		tposy -= this->speed;
		tposz -= this->speed;
		this->object_1_direction = 1;
	}
	else if (tposx < 0.0)
	{
		tposx += this->speed;
		tposy += this->speed;
		tposz += this->speed;
		this->object_1_direction = 0;
	}

	this->_list_models[7]->GetNode()->Position() = Vector3f(this->_list_models[7]->GetNode()->Position().x, tposy, this->_list_models[7]->GetNode()->Position().z);
	this->_list_models[8]->GetNode()->Position() = Vector3f(this->_list_models[8]->GetNode()->Position().x, tposy, this->_list_models[8]->GetNode()->Position().z);
	this->_list_models[9]->GetNode()->Position() = Vector3f(this->_list_models[9]->GetNode()->Position().x, tposy, this->_list_models[9]->GetNode()->Position().z);
	this->_list_models[10]->GetNode()->Position() = Vector3f(this->_list_models[10]->GetNode()->Position().x, tposy, this->_list_models[10]->GetNode()->Position().z);

}

void LJMULevelDemo::UpdateReptile()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	// Linear Interpolation

	static float tposx = 0;
	static float tposy = 0;
	static float tposz = 0;

	//Given Point

	Vector3f point(90.0, 90.0, 90.0);

	if (tposx <= point.x && tposx >= 0.0)
	{
		if (this->object_2_direction == 0)
		{
			this->speed += this->acceleration;
			tposx += this->speed;
			tposy += this->speed;
			tposz += this->speed;
		}

		else if (this->object_2_direction == 1)
		{
			this->speed -= this->acceleration;
			tposx -= this->speed;
			tposy -= this->speed;
			tposz -= this->speed;
		}

	}

	if (tposx > point.x)
	{
		tposx -= this->speed;
		tposy -= this->speed;
		tposz -= this->speed;
		this->object_2_direction = 1;
	}
	else if (tposx < 0.0)
	{
		tposx += this->speed;
		tposy += this->speed;
		tposz += this->speed;
		this->object_2_direction = 0;
	}

	this->_list_models[0]->GetNode()->Position() = Vector3f(tposx, this->_list_models[0]->GetNode()->Position().y, this->_list_models[0]->GetNode()->Position().z);
	this->_list_models[1]->GetNode()->Position() = Vector3f(this->_list_models[1]->GetNode()->Position().x, this->_list_models[1]->GetNode()->Position().y, tposz);
	this->_list_models[2]->GetNode()->Position() = Vector3f(this->_list_models[2]->GetNode()->Position().x, this->_list_models[2]->GetNode()->Position().y, -tposz);
	this->_list_models[3]->GetNode()->Position() = Vector3f(-tposx, this->_list_models[3]->GetNode()->Position().y, this->_list_models[3]->GetNode()->Position().z);
}

void LJMULevelDemo::UpdateTrex()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	static float trotx = 0;
	static float troty = 0;
	static float trotz = 0;
	const float LOCAL_PI = 3.14159265f;
	const float DEG_TO_RAD = LOCAL_PI / 180.0f;
	trotx += 180 * ttpf * DEG_TO_RAD;
	troty += 90 * ttpf * DEG_TO_RAD;
	trotz += 360 * ttpf * DEG_TO_RAD;

	//Relative Change
	Matrix3f trelrot1, trelrot2, trelrot3,trelrot4;
	trelrot1.RotationX(45 * ttpf * DEG_TO_RAD);
	trelrot2.RotationX(-45 * ttpf * DEG_TO_RAD);
	trelrot3.RotationZ(45 * ttpf * DEG_TO_RAD);
	trelrot4.RotationZ(-45 * ttpf * DEG_TO_RAD);

	this->_list_models[11]->GetBody()->Rotation() *= trelrot2;
	this->_list_models[12]->GetBody()->Rotation() *= trelrot1;
	this->_list_models[13]->GetBody()->Rotation() *= trelrot3;
	this->_list_models[14]->GetBody()->Rotation() *= trelrot4;
}

void LJMULevelDemo::UpdatePteranodon()
{
	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();

	// Linear Interpolation

	static float tposx = 0;
	static float tposy = 0;
	static float tposz = 0;

	//Given Point

	Vector3f point(10.0, 10.0, 10.0);

	if (tposx <= point.x && tposx >= 0.0)
	{
		if (this->object_3_direction == 0)
		{
			this->speed += this->acceleration;
			tposx += this->speed;
			tposy += this->speed;
			tposz += this->speed;
		}

		else if (this->object_3_direction == 1)
		{
			this->speed -= this->acceleration;
			tposx -= this->speed;
			tposy -= this->speed;
			tposz -= this->speed;
		}

	}

	if (tposx > point.x)
	{
		tposx -= this->speed;
		tposy -= this->speed;
		tposz -= this->speed;
		this->object_3_direction = 1;
	}
	else if (tposx < 0.0)
	{
		tposx += this->speed;
		tposy += this->speed;
		tposz += this->speed;
		this->object_3_direction = 0;
	}

	this->_list_models[4]->GetNode()->Position() = Vector3f(this->_list_models[4]->GetNode()->Position().x, tposy, this->_list_models[4]->GetNode()->Position().z);
	this->_list_models[5]->GetNode()->Position() = Vector3f(this->_list_models[5]->GetNode()->Position().x, tposy, this->_list_models[5]->GetNode()->Position().z);
	this->_list_models[6]->GetNode()->Position() = Vector3f(this->_list_models[6]->GetNode()->Position().x, tposy, this->_list_models[6]->GetNode()->Position().z);

	static float trotx = 0;
	static float troty = 0;
	static float trotz = 0;
	const float LOCAL_PI = 3.14159265f;
	const float DEG_TO_RAD = LOCAL_PI / 180.0f;
	trotx += 180 * ttpf * DEG_TO_RAD;
	troty += 90 * ttpf * DEG_TO_RAD;
	trotz += 360 * ttpf * DEG_TO_RAD;

	//Relative Change
	Matrix3f trelrot1, trelrot2, trelrot3;
	trelrot1.RotationY(-360 * ttpf * DEG_TO_RAD);
	trelrot2.RotationY(180 * ttpf * DEG_TO_RAD);
	trelrot3.RotationY(-90 * ttpf * DEG_TO_RAD);

	this->_list_models[4]->GetNode()->Rotation() *= trelrot1;
	this->_list_models[5]->GetNode()->Rotation() *= trelrot2;
	this->_list_models[6]->GetNode()->Rotation() *= trelrot3;
}
//-------Application Starting Point - Simply Create an Instance of our Application
LJMULevelDemo AppInstance;