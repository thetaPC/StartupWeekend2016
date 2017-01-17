//--------------------------------------------------------------------------------------
// File: Tutorial022.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

cbuffer VS_CONSTANT_BUFFER : register(b0)
	{
	
	float cb_a;
	float cb_b;
	float cb_c;
	float cb_d;
	};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;	
	output.Pos = input.Pos;
	output.Pos.x *= cb_c;
	output.Pos.y *= cb_c;

	output.Pos.x += cb_a;
	output.Pos.y += cb_b;
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	
	float2 texture_coordinates = input.Tex;
	float4 color = txDiffuse.Sample(samLinear, texture_coordinates);

	/*
	black/white
	color = color.r + color.b + color.g;
	color = color / 3;

	higher contrast
	color = pow(color, 2);
	*/

	/*
	warm
	color.g = color.g * 0.3;
	color.b = color.b * 0.3;

	contrast
	color.r = color.r + 0.1;
	color.b = color.b + 0.9;
	color.g = color.g + 0.3;

	green
	
	*/
	color.g = color.g + cb_d;

	return color;
}
//float3(0.3, 0.59, 0.11)