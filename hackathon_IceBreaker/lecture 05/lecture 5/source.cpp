//--------------------------------------------------------------------------------------
// File: Tutorial022.cpp
//
// This application displays a triangle using Direct3D 11
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#include <time.h>
#include "resource.h"
#include "Font.h"
#include <string>


using namespace std;

struct VS_CONSTANT_BUFFER
	{
	float some_variable_a;
	float some_variable_b;
	float some_variable_c;
	float some_variable_d;
	};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pVertexBuffer2 = NULL;
ID3D11Buffer*			g_pConstantBuffer11 = NULL;
ID3D11BlendState*					g_BlendState;
ID3D11SamplerState*                 g_Sampler = NULL;

ID3D11ShaderResourceView*           g_Texture = NULL;
ID3D11ShaderResourceView*           g_Texture2 = NULL;
ID3D11ShaderResourceView*           g_Texture3 = NULL;
ID3D11ShaderResourceView*           g_Tex_BG= NULL;

float xpos = -0.75;
float ypos = -0.75;
float iy = 0.0;

float lxpos = 0.75;
float lypos = -0.75;

static bool intro, names, player1, player2, player3, player4;
static bool flag, flag2, flag3;

string favFood[4];
string favColor[4];
string myMajor[4];
string playName[4];
string answers[4];

string answersPlayer1[4];
string answersPlayer2[4];
string answersPlayer3[4];
string answersPlayer4[4];
string murderer[4];

bool space;

static int tester = 0;

static int numOfPlayers;

string murderName;
string murderAnswer1, murderAnswer2, murderAnswer3;

Font font, font2;
string name1, name2, name3, name4;
string ques, ques1, ques2, ques3, ques4, ques5, ques6, ques7, ques8, ques9, ques10;
string foodName1, foodName2, foodName3, foodName4;
string colorName1, colorName2, colorName3, colorName4;
string majorName1, majorName2, majorName3, majorName4;
bool firstName, secName, thrName, fourName;
static bool food1, food2, food3, food4;
static bool color1, color2, color3, color4;
static bool major1, major2, major3, major4;
bool q1, q2, q3, q4, q5, q6, q7, q8, q9, q10;
bool quesGiven1, quesGiven2, quesGiven3, quesGiven4;

string murdererName;
string currMurderName;
bool murder;
string question;
int var;
bool passedQuest1, passedQuest2, passedQuest3, passedQuest4;
string answer1, answer2, answer3, answer4;

static bool introduction, scene1, scene2, scene3, scene4, scene5, scene6, scene7, scene8, scene9,
scene10, scene11, scene12, scene13, scene14, scene15, scene16, scene17, scene18,
scene19, scene20, scene21, scene22, scene23, scene24, scene25;

VS_CONSTANT_BUFFER VsConstData;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 1140, 680 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 2: Rendering a Triangle",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{

	srand(time(0));
	var = rand() % 3 + 1;

	

	numOfPlayers = 3;
	name4 = "ERAGON";
	foodName4 = "BURRITO";
	colorName4 = "BLACK";
	majorName4 = "MARINE SCIENCE";
	introduction = true;
	murder = false;
	flag = true;
	flag2 = true;
	flag3 = true;
	//intro = true;
	space = false;
	//firstName = true;
	//player1 = true;
	//passedQuest1 = true;

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"shader.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"shader.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        XMFLOAT3( -0.5f, 0.5f, 0.5f ), XMFLOAT2(0.0f, 0.0f),
        XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT2(1.0f, 0.0f),
        XMFLOAT3( 0.5f, -0.5f, 0.5f ), XMFLOAT2(1.0f, 1.0f),
		XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f),
    };
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    
    

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


	// Supply the vertex shader constant data.
	
	VsConstData.some_variable_a = 0;
	VsConstData.some_variable_b = 0;
	VsConstData.some_variable_c = 1;
	VsConstData.some_variable_d = 1;
	
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Create the buffer.
	hr = g_pd3dDevice->CreateBuffer(&cbDesc, NULL,	&g_pConstantBuffer11);

	if (FAILED(hr))
		return hr;
	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"mario_lowres.png", NULL, NULL, &g_Texture, NULL);
	if (FAILED(hr))
		return hr;

	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"test.jpg", NULL, NULL, &g_Texture2, NULL);
	if (FAILED(hr))
		return hr;

	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"Luigi.png", NULL, NULL, &g_Texture3, NULL);
	if (FAILED(hr))
		return hr;

	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"mansion.png", NULL, NULL, &g_Tex_BG, NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_Sampler);
	if (FAILED(hr))
		return hr;
	
	//blendstate:
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = FALSE;
	blendStateDesc.IndependentBlendEnable = FALSE;
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
	g_pd3dDevice->CreateBlendState(&blendStateDesc, &g_BlendState);


	float blendFactor[] = { 0, 0, 0, 0 };
	UINT sampleMask = 0xffffffff;
	g_pImmediateContext->OMSetBlendState(g_BlendState, blendFactor, sampleMask);

	font.init(g_pd3dDevice, g_pImmediateContext, font.defaultFontMapDesc);
	//font.init(g_pd3dDevice, g_pImmediateContext, font.defaultFontMapDesc);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{

	
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
	if (g_pVertexBuffer2) g_pVertexBuffer2->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}



int dotheswitch(int variable)
{
	int number;
	switch (variable)
	{
	case 1:
		//font.setPosition(XMFLOAT3(-0.99, -0.6, 0));
		//font.setColor(XMFLOAT3(0, 0, 0));
		
		number = 0;

		break;

	case 2:

		number = 1;
		break;

	case 3:

		number = 2;
		break;

	case 4:

		number = 3;
		break;

	

	}

	return number;


}




/*
void story(string *favoriteFood, string *playersName, string *answers, int numberOfPlayers)
{

cout << "It was a dark and stormy night." << endl;

cout << "In the gloomy distance rose a once regal mansion now weathered and decayed by time. A car drives through the fog up the cobblestone drive. The car stops at the front stoop and four figures step out." << endl;

cout << "Their names were ";

for (int i = 0; i < numberOfPlayers; i++)

{

cout << playersName[i];

if (i == numberOfPlayers - 2)

{

cout << ", and ";

}

if (i < numberOfPlayers - 2)

{

cout << ", ";

}

}

cout << ". ";

cout << "They step towards the old mahogany door, the old porch cracking under them." << endl << endl;

cout << playersName[1] << " creaks open the door. There stands a butler with an evil grin by the name of William Stewart Baxterston." << endl << endl;

cout << "\"Come this way… the host is expecting you.\" cackled William Stewart Baxterston." << endl << endl;

cout << "The group follows the obviously murderously british butler. He leads them into the dining room. Sitting at the end of the long dining table is a man more put together than a chair from ikea." << endl << endl;

cout << "\"WELCOME ";

for (int i = 0; i < numberOfPlayers; i++)

{

cout << playersName[i];

if (i == numberOfPlayers - 2)

{

cout << ", and ";

}

if (i < numberOfPlayers - 2)

{

cout << ", ";

}

}

cout << " TO MY SPOOPY MANSION. I AM YOUR DISTINGUISHED HOST, STEVE\" he screamed." << endl << endl;

cout << "The guests wonder why he’s screaming at them. He’s not that far away." << endl << endl;

cout << playersName[2] << " says, \"May I ask why you called us here?\"" << endl << endl;

cout << "\"NO YOU MAY NOT ASK\", responded Steve still fucking screaming." << endl << endl;

cout << "The players stare at each other in confusion." << endl << endl;

cout << "Steve yells again, \"PLEASE SIT DOWN AND MY OBVIOUSLY MURDEROUSLY BRITISH BUTLER WILL FETCH US SOME FOOD FROM THE KITCHEN!\"" << endl << endl;

cout << "\"It’s to DIE for!\" William Stewart Baxterston laughs to himself as he dashes to the kitchen." << endl;

cout << "He brings back all of the guests’ favorite food. " << endl << endl << endl;

cout << "Dinner begins: " << endl << endl;



for (int i = 0; i < numberOfPlayers; i++)

{

if (i == 0 || i == 4)

{

cout << playersName[i] << " is served " << favoriteFood[i] << " and begins to eat." << endl;

}

else if (i == 1 || i == 5 || i == 8)

{

cout << playersName[i] << " digs into their " << favoriteFood[i] << ". " << endl;

}

else if (i == 2 || i == 6 || i == 10)

{

cout << playersName[i] << " is making out with their " << favoriteFood[i] << ". " << endl;

}

else if (i == 3 || i == 7 || i == 9)

{

cout << playersName[i] << " is sniffing their plate of " << favoriteFood[i] << ". " << endl;

}



}

cout << endl;

cout << "Steve stands and yells, \"PLEASE DO NOT BEGIN TO EAT UNTIL I GIVE MY TOSTE.\"" << endl;

}

*/

///////////////////////////////////
//		This Function is called every time the Left Mouse Button is down
///////////////////////////////////
void OnLBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is down
///////////////////////////////////
void OnRBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}
///////////////////////////////////
//		This Function is called every time a character key is pressed
///////////////////////////////////
void OnChar(HWND hwnd, UINT ch, int cRepeat)
{

}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is up
///////////////////////////////////
void OnLBU(HWND hwnd, int x, int y, UINT keyFlags)
{


}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is up
///////////////////////////////////
void OnRBU(HWND hwnd, int x, int y, UINT keyFlags)
{


}
///////////////////////////////////
//		This Function is called every time the Mouse Moves
///////////////////////////////////
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{

	if ((keyFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}

	if ((keyFlags & MK_RBUTTON) == MK_RBUTTON)
	{
	}
}


BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{

	return TRUE;
}
void OnTimer(HWND hwnd, UINT id)
{
	
}
//*************************************************************************
int r_down = 0;
int l_down = 0;
int a_down = 0;
int d_down = 0;
int sp_down = 0;
void OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
	{
		case 65: //a
			a_down = 0;
		break;
		case 68: //d
			d_down = 0;
		break;
		case 32: //space
			
			sp_down = 0;
		break;
		case 39: //right
			r_down = 0;
			break;
		case 37: //left
			l_down = 0;
			break;

	default:break;

	}

}
void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

	switch (vk)
	{
		default:break;
		case 65: //a
			if (firstName) {
				name1 += "A";
			}
			if (scene23) {
				murderName += "A";
			}
			if (food1) {
				foodName1 += "A";
			}
			if (color1) {
				colorName1 += "A";
			}
			if (major1) {
				majorName1 += "A";
			}
			if (quesGiven1) {
				answer1 += "A";
			}
			if (secName) {
				name2 += "A";
			}
			if (food2) {
				foodName2 += "A";
			}
			if (quesGiven2) {
				answer2 += "A";
			}
			if (color2) {
				colorName2 += "A";
			}
			if (major2) {
				majorName2 += "A";
			}
			if (major3) {
				majorName3 += "A";
			}
			if (thrName) {
				name3 += "A";
			}
			if (food3) {
				foodName3 += "A";
			}
			if (color3) {
				colorName3 += "A";
			}
			if (quesGiven3) {
				answer3 += "A";
			}
			if (fourName) {
				name4 += "A";
			}
			if (food4) {
				foodName4 += "A";
			}
			if (color4) {
				colorName4 += "A";
			}
			if (major4) {
				majorName4 += "A";
			}
			if (quesGiven4) {
				answer4 += "A";
			}
			
			//a_down = 1;
		break;
		case 66: //b
			if (firstName) {
				name1 += "B";
			}
			if (scene23) {
				murderName += "B";
			}
			if (food1) {
				foodName1 += "B";
			}
			if (color1) {
				colorName1 += "B";
			}
			if (major1) {
				majorName1 += "B";
			}
			if (quesGiven1) {
				answer1 += "B";
			}
			if (color2) {
				colorName2 += "B";
			}
			if (major2) {
				majorName2 += "B";
			}
			if (major3) {
				majorName3 += "B";
			}
			if (secName) {
				name2 += "B";
			}
			if (food2) {
				foodName2 += "B";
			}
			if (quesGiven2) {
				answer2 += "B";
			}
			if (thrName) {
				name3 += "B";
			}
			if (food3) {
				foodName3 += "B";
			}
			if (color3) {
				colorName3 += "B";
			}
			if (quesGiven3) {
				answer3 += "B";
			}
			if (fourName) {
				name4 += "B";
			}
			if (food4) {
				foodName4 += "B";
			}
			if (color4) {
				colorName4 += "B";
			}
			if (major4) {
				majorName4 += "B";
			}
			if (quesGiven4) {
				answer4 += "B";
			}
			break;
		case 67: //c
			if (firstName) {
				name1 += "C";
			}
			if (food1) {
				foodName1 += "C";
			}
			if (scene23) {
				murderName += "C";
			}
			if (color1) {
				colorName1 += "C";
			}
			if (major1) {
				majorName1 += "C";
			}
			if (color2) {
				colorName2 += "C";
			}
			if (major2) {
				majorName2 += "C";
			}
			if (major3) {
				majorName3 += "C";
			}
			if (secName) {
				name2 += "C";
			}
			if (food2) {
				foodName2 += "C";
			}
			if (thrName) {
				name3 += "C";
			}
			if (food3) {
				foodName3 += "C";
			}
			if (color3) {
				colorName3 += "C";
			}
			if (fourName) {
				name4 += "C";
			}
			if (food4) {
				foodName4 += "C";
			}
			if (color4) {
				colorName4 += "C";
			}
			if (major4) {
				majorName4 += "C";
			}
			if (quesGiven1) {
				answer1 += "C";
			}
			if (quesGiven2) {
				answer2 += "C";
			}
			if (quesGiven3) {
				answer3 += "C";
			}
			if (quesGiven4) {
				answer4 += "C";
			}
			break;
		case 68: //d
			if (firstName) {
				name1 += "D";
			}
			if (food1) {
				foodName1 += "D";
			}
			if (color1) {
				colorName1 += "D";
			}
			if (major1) {
				majorName1 += "D";
			}
			if (color2) {
				colorName2 += "D";
			}
			if (major2) {
				majorName2 += "D";
			}
			if (major3) {
				majorName3 += "D";
			}
			if (secName) {
				name2 += "D";
			}
			if (food2) {
				foodName2 += "D";
			}
			if (thrName) {
				name3 += "D";
			}
			if (food3) {
				foodName3 += "D";
			}
			if (color3) {
				colorName3 += "D";
			}
			if (fourName) {
				name4 += "D";
			}
			if (food4) {
				foodName4 += "D";
			}
			if (color4) {
				colorName4 += "D";
			}
			if (major4) {
				majorName4 += "D";
			}
			if (quesGiven1) {
				answer1 += "D";
			}
			if (quesGiven2) {
				answer2 += "D";
			}
			if (quesGiven3) {
				answer3 += "D";
			}
			if (scene23) {
				murderName += "D";
			}
			if (quesGiven4) {
				answer4 += "D";
			}
		break;
		case 69: //e
			if (firstName) {
				name1 += "E";
			}
			if (food1) {
				foodName1 += "E";
			}
			if (color1) {
				colorName1 += "E";
			}
			if (scene23) {
				murderName += "E";
			}
			if (major1) {
				majorName1 += "E";
			}
			if (color2) {
				colorName2 += "E";
			}
			if (major2) {
				majorName2 += "E";
			}
			if (major3) {
				majorName3 += "E";
			}
			if (secName) {
				name2 += "E";
			}
			if (food2) {
				foodName2 += "E";
			}
			if (thrName) {
				name3 += "E";
			}
			if (food3) {
				foodName3 += "E";
			}
			if (color3) {
				colorName3 += "E";
			}
			if (fourName) {
				name4 += "E";
			}
			if (food4) {
				foodName4 += "E";
			}
			if (color4) {
				colorName4 += "E";
			}
			if (major4) {
				majorName4 += "E";
			}
			if (quesGiven1) {
				answer1 += "E";
			}
			if (quesGiven2) {
				answer2 += "E";
			}
			if (quesGiven3) {
				answer3 += "E";
			}
			if (quesGiven4) {
				answer4 += "E";
			}
			break;
		case 70: //f
			if (firstName) {
				name1 += "F";
			}
			if (food1) {
				foodName1 += "F";
			}
			if (color1) {
				colorName1 += "F";
			}
			if (major1) {
				majorName1 += "F";
			}
			if (color2) {
				colorName2 += "F";
			}
			if (major2) {
				majorName2 += "F";
			}
			if (scene23) {
				murderName += "F";
			}
			if (major3) {
				majorName3 += "F";
			}
			if (secName) {
				name2 += "F";
			}
			if (food2) {
				foodName2 += "F";
			}
			if (thrName) {
				name3 += "F";
			}
			if (food3) {
				foodName3 += "F";
			}
			if (color3) {
				colorName3 += "F";
			}
			if (fourName) {
				name4 += "F";
			}
			if (food4) {
				foodName4 += "F";
			}
			if (color4) {
				colorName4 += "F";
			}
			if (major4) {
				majorName4 += "F";
			}
			if (quesGiven1) {
				answer1 += "F";
			}
			if (quesGiven2) {
				answer2 += "F";
			}
			if (quesGiven3) {
				answer3 += "F";
			}
			if (quesGiven4) {
				answer4 += "F";
			}
			break;
		case 71: //g
			if (firstName) {
				name1 += "G";
			}
			if (food1) {
				foodName1 += "G";
			}
			if (color1) {
				colorName1 += "G";
			}
			if (scene23) {
				murderName += "G";
			}
			if (major1) {
				majorName1 += "G";
			}
			if (color2) {
				colorName2 += "G";
			}
			if (major2) {
				majorName2 += "G";
			}
			if (major3) {
				majorName3 += "G";
			}
			if (secName) {
				name2 += "G";
			}
			if (food2) {
				foodName2 += "G";
			}
			if (thrName) {
				name3 += "G";
			}
			if (food3) {
				foodName3 += "G";
			}
			if (color3) {
				colorName3 += "G";
			}
			if (fourName) {
				name4 += "G";
			}
			if (food4) {
				foodName4 += "G";
			}
			if (color4) {
				colorName4 += "G";
			}
			if (major4) {
				majorName4 += "G";
			}
			if (quesGiven1) {
				answer1 += "G";
			}
			if (quesGiven2) {
				answer2 += "G";
			}
			if (quesGiven3) {
				answer3 += "G";
			}
			if (quesGiven4) {
				answer4 += "G";
			}
			break;
		case 72: //h
			if (firstName) {
				name1 += "H";
			}
			if (food1) {
				foodName1 += "H";
			}
			if (color1) {
				colorName1 += "H";
			}
			if (major1) {
				majorName1 += "H";
			}
			if (color2) {
				colorName2 += "H";
			}
			if (scene23) {
				murderName += "H";
			}
			if (major2) {
				majorName2 += "H";
			}
			if (major3) {
				majorName3 += "H";
			}
			if (secName) {
				name2 += "H";
			}
			if (food2) {
				foodName2 += "H";
			}
			if (thrName) {
				name3 += "H";
			}
			if (food3) {
				foodName3 += "H";
			}
			if (color3) {
				colorName3 += "H";
			}
			if (fourName) {
				name4 += "H";
			}
			if (food4) {
				foodName4 += "H";
			}
			if (color4) {
				colorName4 += "H";
			}
			if (major4) {
				majorName4 += "H";
			}
			if (quesGiven1) {
				answer1 += "H";
			}
			if (quesGiven2) {
				answer2 += "H";
			}
			if (quesGiven3) {
				answer3 += "H";
			}
			if (quesGiven4) {
				answer4 += "H";
			}
			break;
		case 73: //i
			if (firstName) {
				name1 += "I";
			}
			if (food1) {
				foodName1 += "I";
			}
			if (color1) {
				colorName1 += "I";
			}
			if (major1) {
				majorName1 += "I";
			}
			if (color2) {
				colorName2 += "I";
			}
			if (major2) {
				majorName2 += "I";
			}
			if (major3) {
				majorName3 += "I";
			}
			if (secName) {
				name2 += "I";
			}
			if (scene23) {
				murderName += "I";
			}
			if (food2) {
				foodName2 += "I";
			}
			if (thrName) {
				name3 += "I";
			}
			if (food3) {
				foodName3 += "I";
			}
			if (color3) {
				colorName3 += "I";
			}
			if (fourName) {
				name4 += "I";
			}
			if (food4) {
				foodName4 += "I";
			}
			if (color4) {
				colorName4 += "I";
			}
			if (major4) {
				majorName4 += "I";
			}
			if (quesGiven1) {
				answer1 += "I";
			}
			if (quesGiven2) {
				answer2 += "I";
			}
			if (quesGiven3) {
				answer3 += "I";
			}
			if (quesGiven4) {
				answer4 += "I";
			}
			break;
		case 74: //j
			if (firstName) {
				name1 += "J";
			}
			if (food1) {
				foodName1 += "J";
			}
			if (scene23) {
				murderName += "J";
			}
			if (color1) {
				colorName1 += "J";
			}
			if (major1) {
				majorName1 += "J";
			}
			if (color2) {
				colorName2 += "J";
			}
			if (major2) {
				majorName2 += "J";
			}
			if (major3) {
				majorName3 += "J";
			}
			if (secName) {
				name2 += "J";
			}
			if (food2) {
				foodName2 += "J";
			}
			if (thrName) {
				name3 += "J";
			}
			if (food3) {
				foodName3 += "J";
			}
			if (color3) {
				colorName3 += "J";
			}
			if (fourName) {
				name4 += "J";
			}
			if (food4) {
				foodName4 += "J";
			}
			if (color4) {
				colorName4 += "J";
			}
			if (major4) {
				majorName4 += "J";
			}
			if (quesGiven1) {
				answer1 += "J";
			}
			if (quesGiven2) {
				answer2 += "J";
			}
			if (quesGiven3) {
				answer3 += "J";
			}
			if (quesGiven4) {
				answer4 += "J";
			}
			break;
		case 75: //k
			if (firstName) {
				name1 += "K";
			}
			if (food1) {
				foodName1 += "K";
			}
			if (color1) {
				colorName1 += "K";
			}
			if (major1) {
				majorName1 += "K";
			}
			if (color2) {
				colorName2 += "K";
			}
			if (major2) {
				majorName2 += "K";
			}
			if (major3) {
				majorName3 += "K";
			}
			if (secName) {
				name2 += "K";
			}
			if (food2) {
				foodName2 += "K";
			}
			if (thrName) {
				name3 += "K";
			}
			if (food3) {
				foodName3 += "K";
			}
			if (color3) {
				colorName3 += "K";
			}
			if (fourName) {
				name4 += "K";
			}
			if (food4) {
				foodName4 += "K";
			}
			if (color4) {
				colorName4 += "K";
			}
			if (scene23) {
				murderName += "K";
			}
			if (major4) {
				majorName4 += "K";
			}
			if (quesGiven1) {
				answer1 += "K";
			}
			if (quesGiven2) {
				answer2 += "K";
			}
			if (quesGiven3) {
				answer3 += "K";
			}
			if (quesGiven4) {
				answer4 += "K";
			}
			break;
		case 76: //l
			if (firstName) {
				name1 += "L";
			}
			if (food1) {
				foodName1 += "L";
			}
			if (color1) {
				colorName1 += "L";
			}
			if (major1) {
				majorName1 += "L";
			}
			if (color2) {
				colorName2 += "L";
			}
			if (major2) {
				majorName2 += "L";
			}
			if (major3) {
				majorName3 += "L";
			}
			if (secName) {
				name2 += "L";
			}
			if (food2) {
				foodName2 += "L";
			}
			if (thrName) {
				name3 += "L";
			}
			if (food3) {
				foodName3 += "L";
			}
			if (color3) {
				colorName3 += "L";
			}
			if (fourName) {
				name4 += "L";
			}
			if (food4) {
				foodName4 += "L";
			}
			if (color4) {
				colorName4 += "L";
			}
			if (scene23) {
				murderName += "L";
			}
			if (major4) {
				majorName4 += "L";
			}
			if (quesGiven1) {
				answer1 += "L";
			}
			if (quesGiven2) {
				answer2 += "L";
			}
			if (quesGiven3) {
				answer3 += "L";
			}
			if (quesGiven4) {
				answer4 += "L";
			}
			break;
		case 77: //m
			if (firstName) {
				name1 += "M";
			}
			if (food1) {
				foodName1 += "M";
			}
			if (color1) {
				colorName1 += "M";
			}
			if (major1) {
				majorName1 += "M";
			}
			if (color2) {
				colorName2 += "M";
			}
			if (major2) {
				majorName2 += "M";
			}
			if (major3) {
				majorName3 += "M";
			}
			if (secName) {
				name2 += "M";
			}
			if (food2) {
				foodName2 += "M";
			}
			if (thrName) {
				name3 += "M";
			}
			if (food3) {
				foodName3 += "M";
			}
			if (color3) {
				colorName3 += "M";
			}
			if (fourName) {
				name4 += "M";
			}
			if (food4) {
				foodName4 += "M";
			}
			if (color4) {
				colorName4 += "M";
			}
			if (major4) {
				majorName4 += "M";
			}
			if (quesGiven1) {
				answer1 += "M";
			}
			if (quesGiven2) {
				answer2 += "M";
			}
			if (quesGiven3) {
				answer3 += "M";
			}
			if (scene23) {
				murderName += "M";
			}
			if (quesGiven4) {
				answer4 += "M";
			}
			break;
		case 78: //n
			if (firstName) {
				name1 += "N";
			}
			if (scene23) {
				murderName += "N";
			}
			if (food1) {
				foodName1 += "N";
			}
			if (color1) {
				colorName1 += "N";
			}
			if (major1) {
				majorName1 += "N";
			}
			if (color2) {
				colorName2 += "N";
			}
			if (major2) {
				majorName2 += "N";
			}
			if (major3) {
				majorName3 += "N";
			}
			if (secName) {
				name2 += "N";
			}
			if (food2) {
				foodName2 += "N";
			}
			if (thrName) {
				name3 += "N";
			}
			if (food3) {
				foodName3 += "N";
			}
			if (color3) {
				colorName3 += "N";
			}
			if (fourName) {
				name4 += "N";
			}
			if (food4) {
				foodName4 += "N";
			}
			if (color4) {
				colorName4 += "N";
			}
			if (major4) {
				majorName4 += "N";
			}
			if (quesGiven1) {
				answer1 += "N";
			}
			if (quesGiven2) {
				answer2 += "N";
			}
			if (quesGiven3) {
				answer3 += "N";
			}
			if (quesGiven4) {
				answer4 += "N";
			}
			break;
		case 79: //o
			if (firstName) {
				name1 += "O";
			}
			if (food1) {
				foodName1 += "O";
			}
			if (color1) {
				colorName1 += "O";
			}
			if (scene23) {
				murderName += "O";
			}
			if (major1) {
				majorName1 += "O";
			}
			if (color2) {
				colorName2 += "O";
			}
			if (major2) {
				majorName2 += "O";
			}
			if (major3) {
				majorName3 += "O";
			}
			if (secName) {
				name2 += "O";
			}
			if (food2) {
				foodName2 += "O";
			}
			if (thrName) {
				name3 += "O";
			}
			if (food3) {
				foodName3 += "O";
			}
			if (color3) {
				colorName3 += "O";
			}
			if (fourName) {
				name4 += "O";
			}
			if (food4) {
				foodName4 += "O";
			}
			if (color4) {
				colorName4 += "O";
			}
			if (major4) {
				majorName4 += "O";
			}
			if (quesGiven1) {
				answer1 += "O";
			}
			if (quesGiven2) {
				answer2 += "O";
			}
			if (quesGiven3) {
				answer3 += "O";
			}
			if (quesGiven4) {
				answer4 += "O";
			}
			break;
		case 80: //p
			if (firstName) {
				name1 += "P";
			}
			if (food1) {
				foodName1 += "P";
			}
			if (color1) {
				colorName1 += "P";
			}
			if (major1) {
				majorName1 += "P";
			}
			if (scene23) {
				murderName += "P";
			}
			if (color2) {
				colorName2 += "P";
			}
			if (major2) {
				majorName2 += "P";
			}
			if (major3) {
				majorName3 += "P";
			}
			if (secName) {
				name2 += "P";
			}
			if (food2) {
				foodName2 += "P";
			}
			if (thrName) {
				name3 += "P";
			}
			if (food3) {
				foodName3 += "P";
			}
			if (color3) {
				colorName3 += "P";
			}
			if (fourName) {
				name4 += "P";
			}
			if (food4) {
				foodName4 += "P";
			}
			if (color4) {
				colorName4 += "P";
			}
			if (major4) {
				majorName4 += "P";
			}
			if (quesGiven1) {
				answer1 += "P";
			}
			if (quesGiven2) {
				answer2 += "P";
			}
			if (quesGiven3) {
				answer3 += "P";
			}
			if (quesGiven4) {
				answer4 += "P";
			}
			break;
		case 81: //q
			if (firstName) {
				name1 += "Q";
			}
			if (food1) {
				foodName1 += "Q";
			}
			if (color1) {
				colorName1 += "Q";
			}
			if (major1) {
				majorName1 += "Q";
			}
			if (color2) {
				colorName2 += "Q";
			}
			if (major2) {
				majorName2 += "Q";
			}
			if (major3) {
				majorName3 += "Q";
			}
			if (secName) {
				name2 += "Q";
			}
			if (food2) {
				foodName2 += "Q";
			}
			if (thrName) {
				name3 += "Q";
			}
			if (scene23) {
				murderName += "Q";
			}
			if (food3) {
				foodName3 += "Q";
			}
			if (color3) {
				colorName3 += "Q";
			}
			if (fourName) {
				name4 += "Q";
			}
			if (food4) {
				foodName4 += "Q";
			}
			if (color4) {
				colorName4 += "Q";
			}
			if (major4) {
				majorName4 += "Q";
			}
			if (quesGiven1) {
				answer1 += "Q";
			}
			if (quesGiven2) {
				answer2 += "Q";
			}
			if (quesGiven3) {
				answer3 += "Q";
			}
			if (quesGiven4) {
				answer4 += "Q";
			}
			break;
		case 82: //r
			if (firstName) {
				name1 += "R";
			}
			if (food1) {
				foodName1 += "R";
			}
			if (color1) {
				colorName1 += "R";
			}
			if (major1) {
				majorName1 += "R";
			}
			if (color2) {
				colorName2 += "R";
			}
			if (major2) {
				majorName2 += "R";
			}
			if (major3) {
				majorName3 += "R";
			}
			if (secName) {
				name2 += "R";
			}
			if (food2) {
				foodName2 += "R";
			}
			if (thrName) {
				name3 += "R";
			}
			if (food3) {
				foodName3 += "R";
			}
			if (scene23) {
				murderName += "R";
			}
			if (color3) {
				colorName3 += "R";
			}
			if (fourName) {
				name4 += "R";
			}
			if (food4) {
				foodName4 += "R";
			}
			if (color4) {
				colorName4 += "R";
			}
			if (major4) {
				majorName4 += "R";
			}
			if (quesGiven1) {
				answer1 += "R";
			}
			if (quesGiven2) {
				answer2 += "R";
			}
			if (quesGiven3) {
				answer3 += "R";
			}
			if (quesGiven4) {
				answer4 += "R";
			}
			break;
		case 83: //s
			if (firstName) {
				name1 += "S";
			}
			if (food1) {
				foodName1 += "S";
			}
			if (color1) {
				colorName1 += "S";
			}
			if (major1) {
				majorName1 += "S";
			}
			if (color2) {
				colorName2 += "S";
			}
			if (major2) {
				majorName2 += "S";
			}
			if (major3) {
				majorName3 += "S";
			}
			if (secName) {
				name2 += "S";
			}
			if (food2) {
				foodName2 += "S";
			}
			if (thrName) {
				name3 += "S";
			}
			if (food3) {
				foodName3 += "S";
			}
			if (color3) {
				colorName3 += "S";
			}
			if (fourName) {
				name4 += "S";
			}
			if (food4) {
				foodName4 += "S";
			}
			if (color4) {
				colorName4 += "S";
			}
			if (major4) {
				majorName4 += "S";
			}
			if (quesGiven1) {
				answer1 += "S";
			}
			if (quesGiven2) {
				answer2 += "S";
			}
			if (scene23) {
				murderName += "S";
			}
			if (quesGiven3) {
				answer3 += "S";
			}
			if (quesGiven4) {
				answer4 += "S";
			}
			break;
		case 84: //t
			if (firstName) {
				name1 += "T";
			}
			if (scene23) {
				murderName += "T";
			}
			if (food1) {
				foodName1 += "T";
			}
			if (color1) {
				colorName1 += "T";
			}
			if (major1) {
				majorName1 += "T";
			}
			if (color2) {
				colorName2 += "T";
			}
			if (major2) {
				majorName2 += "T";
			}
			if (major3) {
				majorName3 += "T";
			}
			if (secName) {
				name2 += "T";
			}
			if (food2) {
				foodName2 += "T";
			}
			if (thrName) {
				name3 += "T";
			}
			if (food3) {
				foodName3 += "T";
			}
			if (color3) {
				colorName3 += "T";
			}
			if (fourName) {
				name4 += "T";
			}
			if (food4) {
				foodName4 += "T";
			}
			if (color4) {
				colorName4 += "T";
			}
			if (major4) {
				majorName4 += "T";
			}
			if (quesGiven1) {
				answer1 += "T";
			}
			if (quesGiven2) {
				answer2 += "T";
			}
			if (quesGiven3) {
				answer3 += "T";
			}
			if (quesGiven4) {
				answer4 += "T";
			}
			break;
		case 85: //u
			if (firstName) {
				name1 += "U";
			}
			if (food1) {
				foodName1 += "U";
			}
			if (color1) {
				colorName1 += "U";
			}
			if (major1) {
				majorName1 += "U";
			}
			if (color2) {
				colorName2 += "U";
			}
			if (major2) {
				majorName2 += "U";
			}
			if (major3) {
				majorName3 += "U";
			}
			if (secName) {
				name2 += "U";
			}
			if (food2) {
				foodName2 += "U";
			}
			if (thrName) {
				name3 += "U";
			}
			if (food3) {
				foodName3 += "U";
			}
			if (scene23) {
				murderName += "U";
			}
			if (color3) {
				colorName3 += "U";
			}
			if (fourName) {
				name4 += "U";
			}
			if (food4) {
				foodName4 += "U";
			}
			if (color4) {
				colorName4 += "U";
			}
			if (major4) {
				majorName4 += "U";
			}
			if (quesGiven1) {
				answer1 += "U";
			}
			if (quesGiven2) {
				answer2 += "U";
			}
			if (quesGiven3) {
				answer3 += "U";
			}
			if (quesGiven4) {
				answer4 += "U";
			}
			break;
		case 86: //v
			if (firstName) {
				name1 += "V";
			}
			if (food1) {
				foodName1 += "V";
			}
			if (color1) {
				colorName1 += "V";
			}
			if (major1) {
				majorName1 += "V";
			}
			if (color2) {
				colorName2 += "V";
			}
			if (major2) {
				majorName2 += "V";
			}
			if (scene23) {
				murderName += "V";
			}
			if (major3) {
				majorName3 += "V";
			}
			if (secName) {
				name2 += "V";
			}
			if (food2) {
				foodName2 += "V";
			}
			if (thrName) {
				name3 += "V";
			}
			if (food3) {
				foodName3 += "V";
			}
			if (color3) {
				colorName3 += "V";
			}
			if (fourName) {
				name4 += "V";
			}
			if (food4) {
				foodName4 += "V";
			}
			if (color4) {
				colorName4 += "V";
			}
			if (major4) {
				majorName4 += "V";
			}
			if (quesGiven1) {
				answer1 += "V";
			}
			if (quesGiven2) {
				answer2 += "V";
			}
			if (quesGiven3) {
				answer3 += "V";
			}
			if (quesGiven4) {
				answer4 += "V";
			}
			break;
		case 87: //w
			if (firstName) {
				name1 += "W";
			}
			if (scene23) {
				murderName += "W";
			}
			if (food1) {
				foodName1 += "W";
			}
			if (color1) {
				colorName1 += "W";
			}
			if (major1) {
				majorName1 += "W";
			}
			if (color2) {
				colorName2 += "W";
			}
			if (major2) {
				majorName2 += "W";
			}
			if (major3) {
				majorName3 += "W";
			}
			if (secName) {
				name2 += "W";
			}
			if (food2) {
				foodName2 += "W";
			}
			if (thrName) {
				name3 += "W";
			}
			if (food3) {
				foodName3 += "W";
			}
			if (color3) {
				colorName3 += "W";
			}
			if (fourName) {
				name4 += "W";
			}
			if (food4) {
				foodName4 += "W";
			}
			if (color4) {
				colorName4 += "W";
			}
			if (major4) {
				majorName4 += "W";
			}
			if (quesGiven1) {
				answer1 += "W";
			}
			if (quesGiven2) {
				answer2 += "W";
			}
			if (quesGiven3) {
				answer3 += "W";
			}
			if (quesGiven4) {
				answer4 += "W";
			}
			break;
		case 88: //x
			if (firstName) {
				name1 += "X";
			}
			if (food1) {
				foodName1 += "X";
			}
			if (color1) {
				colorName1 += "X";
			}
			if (major1) {
				majorName1 += "X";
			}
			if (scene23) {
				murderName += "X";
			}
			if (color2) {
				colorName2 += "X";
			}
			if (major2) {
				majorName2 += "X";
			}
			if (major3) {
				majorName3 += "X";
			}
			if (secName) {
				name3 += "X";
			}
			if (food2) {
				foodName2 += "X";
			}
			if (thrName) {
				name3 += "X";
			}
			if (food3) {
				foodName3 += "X";
			}
			if (color3) {
				colorName3 += "X";
			}
			if (fourName) {
				name4 += "X";
			}
			if (food4) {
				foodName4 += "X";
			}
			if (color4) {
				colorName4 += "X";
			}
			if (major4) {
				majorName4 += "X";
			}
			if (quesGiven1) {
				answer1 += "X";
			}
			if (quesGiven2) {
				answer2 += "X";
			}
			if (quesGiven3) {
				answer3 += "X";
			}
			if (quesGiven4) {
				answer4 += "X";
			}
			break;
		case 89: //y
			if (firstName) {
				name1 += "Y";
			}
			if (food1) {
				foodName1 += "Y";
			}
			if (color1) {
				colorName1 += "Y";
			}
			if (major1) {
				majorName1 += "Y";
			}
			if (scene23) {
				murderName += "Y";
			}
			if (color2) {
				colorName2 += "Y";
			}
			if (major2) {
				majorName2 += "Y";
			}
			if (major3) {
				majorName3 += "Y";
			}
			if (secName) {
				name2 += "Y";
			}
			if (food2) {
				foodName2 += "Y";
			}
			if (thrName) {
				name3 += "Y";
			}
			if (food3) {
				foodName3 += "Y";
			}
			if (color3) {
				colorName3 += "Y";
			}
			if (fourName) {
				name4 += "Y";
			}
			if (food4) {
				foodName4 += "Y";
			}
			if (color4) {
				colorName4 += "Y";
			}
			if (major4) {
				majorName4 += "Y";
			}
			if (quesGiven1) {
				answer1 += "Y";
			}
			if (quesGiven2) {
				answer2 += "Y";
			}
			if (quesGiven3) {
				answer3 += "Y";
			}
			if (quesGiven4) {
				answer4 += "Y";
			}
			break;
		case 90: //z
			if (firstName) {
				name1 += "Z";
			}
			if (food1) {
				foodName1 += "Z";
			}
			if (color1) {
				colorName1 += "Z";
			}
			if (major1) {
				majorName1 += "Z";
			}
			if (scene23) {
				murderName += "Z";
			}
			if (color2) {
				colorName2 += "Z";
			}
			if (major2) {
				majorName2 += "Z";
			}
			if (major3) {
				majorName3 += "Z";
			}
			if (secName) {
				name2 += "Z";
			}
			if (food2) {
				foodName2 += "Z";
			}
			if (thrName) {
				name3 += "Z";
			}
			if (food3) {
				foodName3 += "Z";
			}
			if (color3) {
				colorName3 += "Z";
			}
			if (fourName) {
				name4 += "Z";
			}
			if (food4) {
				foodName4 += "Z";
			}
			if (color4) {
				colorName4 += "Z";
			}
			if (major4) {
				majorName4 += "Z";
			}
			if (quesGiven1) {
				answer1 += "Z";
			}
			if (quesGiven2) {
				answer2 += "Z";
			}
			if (quesGiven3) {
				answer3 += "Z";
			}
			if (quesGiven4) {
				answer4 += "Z";
			}
			break;
		case 13: //enter
			if (introduction == true) {
				introduction = false;
				
				names = true;
				player1 = true;
				firstName = true;
				//scene1 = true;
			} else if (firstName) {
				firstName = false;
				//secName = true;
				food1 = true;
			} else if (food1) {
				food1 = false;
				//quesGiven1 = true;
				color1 = true;
			}
			else if (color1) {
				major1 = true;
				
				color1 = false;
				
			}
			else if (major1) {
				major1 = false;
				player1 = false;
				
				player2 = true;
				secName = true;
			}
			else if (secName) {
				secName = false;
				food2 = true;
			}
			else if (food2) {
				food2 = false;
				color2 = true;
				
			}
			else if (color2) {
				color2 = false;
				major2 = true;
			}
			else if (major2) {
				thrName = true;
				major2 = false;
				player3 = true;
				player2 = false;
			}
			else if (thrName) {
				thrName = false;
				food3 = true;
			}
			else if (food3) {
				food3 = false;
				
				color3 = true;
			}
			else if (color3) {
				color3 = false;
				major3 = true;
			}
			else if (major3) {
				fourName = true;
				major3 = false;
				player4 = true;
				player3 = false;
				if (numOfPlayers == 3) {
					names = false;
					scene1 = true;
				}
			}
			else if (numOfPlayers == 4) {
				 if (fourName) {
					fourName = false;
					food4 = true;
				}
				else if (food4) {
					food4 = false;
					color4 = true;
					//player4 = true;
					//fourName = true;
				}
				else if (color4) {
					color4 = false;
					major4 = true;
				}
				else if (major4) {
					major4 = false;
					player4 = false;
					names = false;
					scene1 = true;
				}
			}
			else if (scene1 == true) {
				scene1 = false;
				scene2 = true;
			}

			else if (scene2 == true) {
				scene2 = false;
				scene3 = true;
			}
			else if (scene3 == true) {
				scene3 = false;
				scene4 = true;
			}
			else if (scene4 == true) {
				scene4 = false;
				scene5 = true;
			}
			else if (scene5 == true) {
				scene5 = false;
				scene6 = true;
			}
			else if (scene6 == true) {
				scene6 = false;
				scene7 = true;
			}
			else if (scene7 == true) {
				scene7 = false;
				scene8 = true;
			}
			else if (scene8 == true) {
				scene8 = false;
				scene9 = true;
			}
			else if (scene9 == true) {
				scene9 = false;
				scene10 = true;
			}
			else if (scene10 == true) {
				scene10 = false;
				scene11 = true;
			}
			else if (scene11 == true) {
				scene11 = false;
				scene12 = true;
			}
			else if (scene12 == true) {
				scene12 = false;
				scene13 = true;
			}
			else if (scene13 == true) {
				scene13 = false;
				scene14 = true;
			}
			else if (scene14 == true) {
				scene14 = false;
				scene15 = true;
			}
			else if (scene15 == true) {
				scene15 = false;
				scene16 = true;
			}
			else if (scene16 == true) {
				scene16 = false;
				scene17 = true;
			}
			else if (scene17 == true) {
				scene17 = false;
				scene18 = true;
			}
			else if (scene18 == true) {
				scene18 = false;
				scene19 = true;
			}
			else if (scene19 == true) {
				scene19 = false;
				scene20 = true;
			}
			else if (scene20 == true) {
				scene20 = false;
				scene21 = true;
			}
			else if (scene21 == true) {
				scene21 = false;
				scene22 = true;
			}
			else if (scene22 == true) {
				scene22 = false;
				scene23 = true;
			}
			else if (scene23 == true) {
				scene23 = false;
				scene24 = true;
			}
			else if (scene24 == true) {
				scene24 = false;
				scene25 = true;
			}
			
			break;
		case 32: //space
			//sp_down = 1;
			if (intro) {
				tester = 1;
				space = true;
			}
			
			//iy = 0.0006;
		break;
		case 51: //3
			/*
			if (introduction) {
				numOfPlayers = 3;
				intro = false;
				names = true;
			}
			*/
			break;
		case 52: //4
			/*
			if (introduction) {
				numOfPlayers = 4;
				intro = false;
				names = true;
			}
			*/
			break;
		case 27: //escape
			PostQuitMessage(0);
			break;
		case 39: //right
			break;
		case 37: //left
			break;
	}
}
//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	SCROLLINFO si;



	switch (message)
	{
		/*
		#define HANDLE_MSG(hwnd, message, fn)    \
		case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))
		*/

		//HANDLE_MSG(hwnd, WM_CHAR, OnChar);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLBD);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLBU);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM);
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, OnKeyUp);

	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
//*************************************************

class sprite
	{
	public:
		XMFLOAT2 position;
		XMFLOAT2 impulse;
		sprite()
			{
			position = XMFLOAT2(0, 0);
			impulse = XMFLOAT2(0.0001, 0);
			}
	};
//***************************************************
sprite player, npc, luigi;
void animation()
	{
	player.position.x = player.position.x + player.impulse.x;
	npc.position.x = npc.position.x + npc.impulse.x*2;
	luigi.position.x = luigi.position.x + luigi.impulse.x*3;
	
	if (player.position.x < -1) player.impulse.x = 0.0001;
	if (player.position.x > 1) player.impulse.x = -0.0001;

	if (luigi.position.x < -1) luigi.impulse.x = 0.0001;
	if (luigi.position.x > 1) luigi.impulse.x = -0.0001;

	if (npc.position.x < -1) npc.impulse.x = 0.0001;
	if (npc.position.x > 1) npc.impulse.x = -0.0001;


	}
//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------



void Render()
{
	
	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer2, &stride, &offset);

	// Clear the back buffer 
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	float scale_factor = 1.0;

	animation();

	//for all rectangles:
	g_pImmediateContext->PSSetSamplers(0, 1, &g_Sampler);
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	
	
	

	//draw background
	scale_factor = 2.0;
	VsConstData.some_variable_a = 0;
	VsConstData.some_variable_b = 0;
	VsConstData.some_variable_c = scale_factor;
	VsConstData.some_variable_d = 0;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer11, 0, 0, &VsConstData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	g_pImmediateContext->PSSetShaderResources(0, 1, &g_Tex_BG);
	g_pImmediateContext->Draw(6, 0);

	if (introduction == true) {
		//intro
		font.setPosition(XMFLOAT3(-.45, -.45, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Hello and welcome to Icebreaker: Murder Mansion."; //+ speed;
		font.setPosition(XMFLOAT3(-.85, -.6, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "You are about to adventure through a horror filled mansion with icebreaker challenges. These icebreaker";
		font.setPosition(XMFLOAT3(-.85, -.68, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "challenges will help you solve which of of you is the killer. Will you be able to solve the murder before the";
		font.setPosition(XMFLOAT3(-.85, -.76, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "police arrive or will the murderer escape?";
		font.setPosition(XMFLOAT3(-.20, -.84, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "3 Players needed";
		//font << "How many players? (3-4) " + to_string(numOfPlayers);
		//font << "Press enter to start.";
		
	}

	/*
	if (intro) {
		int number;
		font.setPosition(XMFLOAT3(-1, -0.65, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Ice Breaker: A Murder Mystery";

		font.setPosition(XMFLOAT3(-1, -0.8, 0));

		font.setColor(XMFLOAT3(tester, tester, tester));
		string firstTxt = "Press spacebar to start";

		font << firstTxt;
		// ADD THE CODE FOR THIS
		if (space) {
			font.setColor(XMFLOAT3(0, 0, 0));
			font << "How many players? (3-4) " + to_string(numOfPlayers);
			font.setPosition(XMFLOAT3(-1, -0.95, 0));
			//font << "Thank you!";
			//intro = false;
		}
	}
	
	
	*/

	//names = true;
	if (names) {
			if (player1) {
				if (firstName) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 1, enter your name: " + name1;
				}
				//after typing in name. press enter to continue. firstName becomes false

				if (firstName == false) {
					playName[0] = name1;
					
				}

				//food1 becomes activated
				if (food1) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 1, enter your name: " + name1;
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[0] + ", enter your favorite food: " + foodName1;
				}
				
				//food1 is false when enter is pressed.
				
				if (food1 == false) {
					favFood[0] = foodName1;
					answersPlayer1[0] = favFood[0];

				}

				//color1 is true when food1 is false

				if (color1) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[0] + ", enter your favorite color: " + colorName1;
				}

				//color1 becomes false after enter
				if (color1 == false) {
					favColor[0] = colorName1;
					answersPlayer1[1] = favColor[0];

				}

				//major1 is true when color1 is false
				if (major1) {
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[0] + ", enter your major: " + majorName1;
				}

				//major1 is false after enter

				if (major1 == false) {
					myMajor[0] = majorName1;
					answersPlayer1[2] = myMajor[0];

				}


				/*
				if (passedQuest1) {
					question = dotheswitch(var, answers, 0);
					passedQuest1 = false;
					passedQuest2 = true;
				}
				

				if (quesGiven1) {
					font.setPosition(XMFLOAT3(-1, -0.65, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << question + answer1;
				}

				if (quesGiven1 == false) {
					answers[0] = answer1;

				}
				*/

				//font << playName[0] + ", ";
			}
			if (player2) {
				if (secName) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 2, enter your name: " + name2;
				}

				if (secName == false) {
					playName[1] = name2;

				}

				if (food2) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 2, enter your name: " + name2;
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[1] + ", enter your favorite food: " + foodName2;
				}


				if (food2 == false) {
					favFood[1] = foodName2;
					answersPlayer2[0] = favFood[1];

				}
				
				if (color2) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[1] + ", enter your favorite color: " + colorName2;
				}


				if (color2 == false) {
					favColor[1] = colorName2;
					answersPlayer2[1] = favColor[1];

				}

				if (major2) {
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[1] + ", enter your major: " + majorName2;
				}


				if (major2 == false) {
					myMajor[1] = majorName2;
					answersPlayer2[2] = myMajor[1];

				}
				//font << playName[0] + ", ";
			}

			if (player3) {
				if (thrName) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 3, enter your name: " + name3;
				}

				if (thrName == false) {
					playName[2] = name3;

				}

				if (food3) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << "Player 3, enter your name: " + name3;
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[2] + ", enter your favorite food: " + foodName3;
				}


				if (food3 == false) {
					favFood[2] = foodName3;
					answersPlayer3[0] = favFood[2];
				}
				
				if (color3) {
					font.setPosition(XMFLOAT3(-.85, -.45, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[2] + ", enter your favorite color: " + colorName3;
				}


				if (color3 == false) {
					favColor[2] = colorName3;
					answersPlayer3[1] = favColor[2];

				}

				if (major3) {
					font.setPosition(XMFLOAT3(-.85, -.55, 0));
					font.setColor(XMFLOAT3(0, 0, 0));
					font << playName[2] + ", enter your major: " + majorName3;
				}


				if (major3 == false) {
					myMajor[2] = majorName3;
					answersPlayer3[2] = myMajor[2];
					if (numOfPlayers == 3) {
						if (flag2) {
							murder = true;
							flag2 = false;
						}
						
					}

				}
				//font << playName[0] + ", ";
			}

			if (numOfPlayers == 4) {
				if (player4) {
					if (fourName) {
						font.setPosition(XMFLOAT3(-.85, -.45, 0));
						font.setColor(XMFLOAT3(0, 0, 0));
						font << "Player 4, enter your name: " + name4;
					}

					if (fourName == false) {
						playName[3] = name4;

					}

					if (food4) {
						font.setPosition(XMFLOAT3(-.85, -.45, 0));
						font.setColor(XMFLOAT3(0, 0, 0));
						font << "Player 4, enter your name: " + name4;
						font.setPosition(XMFLOAT3(-.85, -.55, 0));
						font.setColor(XMFLOAT3(0, 0, 0));
						font << playName[3] + ", enter your favorite food: " + foodName4;
					}


					if (food4 == false) {
						favFood[3] = foodName4;
						answersPlayer4[0] = favFood[3];
					}

					if (color4) {
						font.setPosition(XMFLOAT3(-.85, -.45, 0));
						font.setColor(XMFLOAT3(0, 0, 0));
						font << playName[3] + ", enter your favorite color: " + colorName4;
					}


					if (color4 == false) {
						favColor[3] = colorName4;
						answersPlayer4[1] = favColor[3];

					}

					if (major4) {
						font.setPosition(XMFLOAT3(-.85, -.55, 0));
						font.setColor(XMFLOAT3(0, 0, 0));
						font << playName[3] + ", enter your major: " + majorName4;
					}
					if (major4 == false) {
						myMajor[3] = majorName4;
						answersPlayer4[2] = myMajor[3];
						if (flag3) {
							murder = true;
							flag3 = false;
						}
						

					}
					//font << playName[0] + ", ";
				}
			}

			/*
			
			srand(time(0));
			if (numOfPlayers == 4) {
				if (flag) {
					var = rand() % 4 + 1;
					flag = false;
				}
				
			}
			else {
				if (flag) {
					var = rand() % 3 + 1;
					flag = false;
				}
				
			}
			

			if (murder ) {
				int guilty = dotheswitch(var);
				murdererName = playName[guilty];
				if (guilty == 0) {
					murderer[0] = answersPlayer1[0];
					murderer[1] = answersPlayer1[1];
					murderer[2] = answersPlayer1[2];
				} else if (guilty == 1) {
					murderer[0] = answersPlayer2[0];
					murderer[1] = answersPlayer2[1];
					murderer[2] = answersPlayer2[2];
				} else if (guilty == 2) {
					murderer[0] = answersPlayer3[0];
					murderer[1] = answersPlayer3[1];
					murderer[2] = answersPlayer3[2];
				} else if (guilty == 3) {
					murderer[0] = answersPlayer4[0];
					murderer[1] = answersPlayer4[1];
					murderer[2] = answersPlayer4[2];
				}
				murder = false;
			}
			*/

			



			
		
	}

	if (var == 1) {
		murderAnswer1 = foodName1;
		murderAnswer2 = colorName1;
		murderAnswer3 = majorName1;
		currMurderName = name1;
	}
	else if (var == 2) {
		murderAnswer1 = foodName2;
		murderAnswer2 = colorName2;
		murderAnswer3 = majorName2;
		currMurderName = name2;
	}
	else if (var == 3) {
		murderAnswer1 = foodName3;
		murderAnswer2 = colorName3;
		murderAnswer3 = majorName3;
		currMurderName = name3;
	}

	


	if (scene1 == true) {
		//scene 1
		font.setPosition(XMFLOAT3(-.25, -.45, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "It was a dark and stormy night.";
		font.setPosition(XMFLOAT3(-.85, -.6, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "In the gloomy distance rose a once regal mansion now weathered and decayed by time. A car drives through";
		font.setPosition(XMFLOAT3(-.85, -.68, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "the cobblestone drive. The car stops at the front stoop and four figures step out. Their names were \n" + name1 + ", " + name2 + ", " + name3 + ", " + name4;
		//add in player names
	}

	if (scene2 == true) {
		//scene 2
		//add in player 1
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "They step towards the old mahogany door, the old porch craking under them. " + name1 + " creaks open the door.";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "There stands a butler with an evil grin by the name of William Steward Baxterston. \"Come this way......";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "the host is expecting you,\" cackled William Stewart Baxterston.";
	}

	if (scene3 == true) {
		//scene 3
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "The group follows the obviously murderous british bulter. He leads them upstairs to the second floor.";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "They continue down a red carpeted hallway and pass two armored knights going into a study with two";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "large wooden tables littered with books. Standing in front of the decrepit bookshelves is a man more put";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "together than a chair from ikea.";
	}

	if (scene4 == true) {
		//scene 4
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "William Stewart Baxterston coughs and says,\"Excuse me? Sir? I have your guests\". He turns around with";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "a dead blank stare and screams,\"WELCOME " + name1 + ", " + name2 + ", " + name3 + ", " + name4 + " TO MY";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "SPOOKY MANSION. I AM YOUR DISTINGUISHED HOST, STEVE!\" Everybody wonders why he's screaming at them.";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "He's not that faraway." + name3 + " says,\"May I ask why you called us here?\"";
	}

	if (scene5 == true) {
		//scene 5
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"NO YOU MAY NOT ASK, STEVE INSTANTLY RESPONDED, STILL SCREAMING.\" Everybody looks at";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "each other confused. Steve yells again,\"PLEASE SIT DOWN AND MY OBVIOUSLY MURDEROUS BRITISH";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "BUTLER WILL FETCH US SOME FOOD FROM THE KITCHEN!\" \"It's to DIE for!\" says William Stewart";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " Baxterston as he evilly laughs to himself and dashes off to the kitchen on the first floor.";
	}

	if (scene6 == true) {
		//scene 6
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody stands waiting for Steve to lead them to a dining area, but instead walks over and just ";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "sits down at one of the wooden tables. " + name2 + " asks, \"Are we going to eat in this room?\" ";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Steve responds, \"OF COURSE WE ARE! THE FIRST FLOOR IS BEING FUMIGATED!\" Once again";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "everybody looks at each other in confusion and they move to sit at the wooden tables.";
	}

	if (scene7 == true) {
		//scene 7
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "They waited for a good seven minutes in complete silence until they started to hear the evil laughing ";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "of the butler. He enters the room with four large plates of food and places each plate in front of";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "each guest. The only thing on the plates are small chicken wings. They sit there and start to pick";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "it up and take a bite. When Steve yells, \"PLEASE DO NOT BEGIN TO EAT UNTIL I'VE GIVEN MY TOSTE!!!\"";
	}

	if (scene8 == true) {
		//scene 8
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody instantly drops the chicken back onto their plates. He continues, \"YOU ALL MAY BE WONDERING";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "WHY I INVITED YOU ALL HERE! (Awkward slience fills the room). WELL I JUST WANTED TO HAVE A MEAL";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " WITH YOU SO EAT!,\" Steve shouts. They pick up their chicken wings and start to eat again. As they do Steve ";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "starts laughing to himself. Once they all finish their meal a lightning strike hits and all the lights go out.";
	}

	if (scene9 == true) {
		//scene 9
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody hears a loud thump, which if followed by a scream. The lights turn back on and there on the";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " ground is the butler with a fatal head injury. Everyone stands up in fear except Steve and he beings to";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " laugh loudly. He stands up and yells, \"WELL LOOKS LIKE IT HAPPENED SOONER THAN I EXPECTED!\"";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << name4 + " responds nervously, \"What do you mean?\"";
	}

	if (scene10 == true) {
		//scene 10
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"IN YOUR FOOD I HAD WILLIAM STEWART BAXTERSTON INSERT A PILL INTO ONE OF YOUR DINNERS";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " THAT WILL MAKE YOU A MURDERER!,\" Steve shouted. All the guests look at each other in suspicion. \"Well ";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "who is it?,\" says " + name3 + ". Steve yells, \"I DO NOT KNOW AND THE PERSON THAT TOOK THE PILL HAS NO";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "RECOLLECTION OF ANYTHING THEY DID BECAUSE IT GIVES YOU MEMORY LOSS!\"";
	}

	if (scene11 == true) {
		//scene 11
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"How do we know who the murderer is,\" says "+ name2 + ". Steve yells, \"YOU DON'T! THE ONLY PERSON";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "THAT WOULD KNOW IS WILLIAM STEWART BAXTERSTON AND EVEN IF HE WAS A GHOST IT WOULDN'T ";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "HELP YOU THAT MUCH!\" \"Why?,\" asks "+ name1 + ". \"BECAUSE HE WAS BAD WITH NAMES AND THE ";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "ONLY THING THAT HE CAN REMEMBER WAS WHAT PEOPLE LIKE!,\" RESPONDED STEVE.";
	}

	if (scene12 == true) {
		//scene 12
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "They all stand in confusion, not knowing what to do or say. Steve yells, \"WELL HAVE FUN";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "FIGURING OUT WHO THE MURDERER IS, I HAVE SOME BUSINESS TO ATTEND TO!.\" Steve begins";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "to walk out and as he is about to exit the room he says, \"I HAVE CONTACTED THE POLICE AND";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "THEY WILL ARRICE SHORTLY! HAVE FUN FIGURING OUT WHO THE MURDERER IS!\"";
	}

	if (scene13 == true) {
		//scene 13
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody looks at each other in confusion wondering what to do next. " + name1 + " says, \"Well this is ";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "interesting. How are we going to find out who the killer is if we don't even know anything about";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "him\". \"I guess we have to ask William Stewart Baxterston's ghost then,\" " + name2 + " says jokingly.";
	}

	if (scene14 == true) {
		//scene 14
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Suddenly the lights start to flicker and a cold chill runs down everyones backs and before their";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "very eyes, a blue fant figure appears before them. \"IT'S THE BUTLER!,\" yells " + name3 + ". They all";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "scream as they see him flout there in the room. " + name4 + " yells, \"WHAT DO YOU WANT!\" at the ghost";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "of William Stewart Baxterston.";
	}

	if (scene15 == true) {
		//scene 15
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "The ghost of the once alive butler says in a spooky grim voice, \"Yeah I was going to tell you who the killer";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "is, but my memory isn't too great. However, I do know what color they like.\" " + name4 + " asks, \"Well what is it?\"";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"Let me think. Oh yes! They liked the color " + murderAnswer2 + ",\" said the ghost of William Stewart Baxterston. ";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "He fades away and the study goes back to normal.";
	}

	if (scene16 == true) {
		//scene 16
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"The color " + murderAnswer2 + "?\" repeats " + name3 + " breaking the silence. They all think to themselves what";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "their favorite colors are and ask each other about their favorite colors. (Talk out loud about your favorite";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " colors). All of a sudden the walls start to bleed and books are flying off the shelf and the butlers ghost returns";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " and says, \"Well have you figured out who the killer is yet? I still don't know who it is and it's nerve racking.\"";
	}

	if (scene17 == true) {
		//scene 17
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"Can you give us one more hint before you disappear again,\" says " + name2 + ". William Stewart Baxterston";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " responds, \"Well if my memory isn't as bad as I think it is then I think I remember another thing that the killer";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " likes.\" He begins to concentrate really hard and beings to glow while doing so. Wind starts blowing and";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " howling through the hall.\"I am getting an image of something you can eat, he says in a stressful voice.\"";
	}

	if (scene18 == true) {
		//scene 18
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"Their favorite food is " + murderAnswer1 + ",\" says William Stewart Baxterston and then instantly vanishes leaving";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "nothing behind. They start to look at each oth suspicisouly thinking who likes "+ murderAnswer1 +" to themselves";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "and who the murderer could be. (Talk out loud about your favorite food). \"Okay so who could be the killer,\"";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " asks " + name1 + " and then the ground starts to shake. \"What now!,\" yells " + name2 + ".";
	}

	if (scene19 == true) {
		//scene 19
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "The butlers ghost returns and says in a frustrated voice, \"You people still haven't figured out the killer!\"";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"You only gave us thirty seconds to think about it,\" says "+name4+". \"Well sorry, but I'll give you guys";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "one last hint before I leave once and for all.\" \"Let me think about this...,\" says the ghost of William ";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Stewart Baxterston. He ponders for a little bit.";
	}

	if (scene20 == true) {
		//scene 20
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"I'm not 100% sure, but the murderer did want to major in... hmm I think it was " + murderAnswer3 + ".\"";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "(Talk out loud about your majors). \""+ murderAnswer3 + "? Are you sure?,\" asks " + name3 + ". \"Yes, yes I'm";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << " quite sure. I must be off, I'm very tired and wish to retire for the remainder of my day,\" says the";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "the butler. \"But how will we...\" " + name3 + " (gets interrupted by the butler) who says, \"I SAID GOOD DAY!\"";
	}

	if (scene21 == true) {
		//scene 21
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody goes silent and the ghost of William Stewart Baxterston disappeared into thin air. All the guests";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "debate on who the killer might be, they look at each other back and forth trying to figure out who it is.";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "The talking is suddenly interrupted with a loud knock echoing throughout the mansion. Then another knock is";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "heard. \"Someone is knocking at the door,\" says " + name4 + ". \"We should answer it,\" says " + name3 + ".";
	}

	if (scene22 == true) {
		//scene 22
		font.setPosition(XMFLOAT3(-.85, -.55, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "Everybody walks into the hallway downstairs to the door to open it. They open the door and standing right ";
		font.setPosition(XMFLOAT3(-.85, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "in front of them is a man in a drenched brown trench coat holding up a police badge. In a deep voice the";
		font.setPosition(XMFLOAT3(-.85, -.71, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "detective asks, \"So I heard there was a murder here. Is this the place?\" Everybody responds with yes.";
		font.setPosition(XMFLOAT3(-.85, -.79, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "\"So who is the killer then?,\" asks the detective.";
	}

	if (scene23 == true) {
		//scene 23
		font.setPosition(XMFLOAT3(-.30, -.63, 0));
		font.setColor(XMFLOAT3(0, 0, 0));
		font << "The killer is... (Type in killer's name):" + murderName;
	}

	if (scene23 == false) {
		if (murderName == currMurderName) {
			//right answer
			if (scene24 == true) {
				//scene 24
				font.setPosition(XMFLOAT3(-.75, -.55, 0));
				font.setColor(XMFLOAT3(0, 0, 0));
				font << "The group tells the detective that "+ murderName +" is the killer and the detective arrests them.";
				font.setPosition(XMFLOAT3(-.20, -.71, 0));
				font.setColor(XMFLOAT3(0, 0, 0));
				font << "YOU WIN, congratulations!.";
			}
		}
		else {
			//wrong answer
			if (scene25 == true) {
				//scene 25
				font.setPosition(XMFLOAT3(-.75, -.55, 0));
				font.setColor(XMFLOAT3(0, 0, 0));
				font << "The group tells the detective that " + murderName + " is the killer. The detective doesn't believe you";
				font.setPosition(XMFLOAT3(-.75, -.63, 0));
				font.setColor(XMFLOAT3(0, 0, 0));
				font << "and storms past you to talk to the owner of the house.";
				font.setPosition(XMFLOAT3(-.25, -.79, 0));
				font.setColor(XMFLOAT3(0, 0, 0));
				font << "YOU LOSE, the killer gets away.";
			}
		}
	}
	
	
//	string tes = myMajor[0];

	//string speed = "Hi";
	
	
	

	

	//int numberPlayers = playerNumber();

	//questions(favFood, playName, answers, numberPlayers);

	//story(favFood, playName, answers, numberPlayers);

	/*
	//draw npc
	scale_factor = 1.0;
	VsConstData.some_variable_a = npc.position.x;
	VsConstData.some_variable_b = npc.position.y;
	VsConstData.some_variable_c = scale_factor;
	VsConstData.some_variable_d = 0;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer11, 0, 0, &VsConstData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	g_pImmediateContext->PSSetShaderResources(0, 1, &g_Texture2);
	g_pImmediateContext->Draw(6, 0);

	//draw player (luigi)
	//scale_factor = 0.3;
	static float size = 0.0;
	size += 0.00004;
	scale_factor = sin(size) + 1;
	
	
	
	//float ff = 0;
	//(sin(ff) + 1.) / 2.
	VsConstData.some_variable_a = lxpos;
	VsConstData.some_variable_b = lypos;
	VsConstData.some_variable_c = scale_factor;
	VsConstData.some_variable_d = 0;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer11, 0, 0, &VsConstData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	g_pImmediateContext->PSSetShaderResources(0, 1, &g_Texture3);
	g_pImmediateContext->Draw(6, 0);

	//draw player (mario)
	scale_factor = 0.3;
	VsConstData.some_variable_a = xpos;
	VsConstData.some_variable_b = ypos;
	VsConstData.some_variable_c = scale_factor;
	VsConstData.some_variable_d = 1;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer11, 0, 0, &VsConstData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	g_pImmediateContext->PSSetShaderResources(0, 1, &g_Texture);
	g_pImmediateContext->Draw(6, 0);

	//draw rectangle
	
	scale_factor = 1.0;
	VsConstData.some_variable_a = 0;
	VsConstData.some_variable_b = 0;
	VsConstData.some_variable_c = scale_factor;
	VsConstData.some_variable_d = 0;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer11, 0, 0, &VsConstData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer11);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer11);

	g_pImmediateContext->PSSetShaderResources(0, 1, &g_Tex_BG);
	g_pImmediateContext->Draw(6, 0);
	*/
	



	// Present the information rendered to the back buffer to the front buffer (the screen)
	g_pSwapChain->Present(0, 0);
}

