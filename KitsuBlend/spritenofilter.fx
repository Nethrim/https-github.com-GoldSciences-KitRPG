//=============================================================================
// sprite.fx by Pablo Zorrilla (C) 2010 All Rights Reserved.
//
// Vertex and texture transformations for sprite rendering.
//=============================================================================
uniform extern float4x4	gWVP;
uniform extern float4		gColor;

uniform extern texture	gTex0;
uniform extern float2		gTexOffset0;
uniform extern float2  	gTexSize0;
uniform extern float2  	gTexelSize0;

sampler SpriteS0 = sampler_state
{
	Texture = <gTex0>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	MaxAnisotropy = 1;
	AddressU  = WRAP;
	AddressV  = WRAP;
}; 
 
struct OutputVS
{
	float4 	posH : POSITION0;
	float2 	tex0 : TEXCOORD0;
};

OutputVS Vertex( float3 posL : POSITION0, float2 tex0 : TEXCOORD0 )
{
	OutputVS ovs 	= (OutputVS)0;
	// 
	ovs.tex0.x 	= tex0.x*(gTexelSize0.x*gTexSize0.x);
	ovs.tex0.y 	= tex0.y*(gTexelSize0.y*gTexSize0.y);

	ovs.tex0.x 	+= (gTexelSize0.x)*(gTexOffset0.x);
	ovs.tex0.y 	+= (gTexelSize0.y)*(gTexOffset0.y);

	ovs.posH 			= mul( float4( posL, 1.0f ), gWVP );
	return ovs;
}

float4 Pixel( float2 tex0 : TEXCOORD0 ) : COLOR
{
	// Get texel from texture map that gets mapped to this pixel.
	float4 texColor = tex2D(SpriteS0, tex0).rgba;
	
	// Scale with the sprite color parameter and skip alpha.
	return float4( texColor.r*gColor.r, texColor.g*gColor.g, texColor.b*gColor.b, 1.0f );
}

float4 PixelAlpha( float2 tex0 : TEXCOORD0 ) : COLOR
{
	// Get texel from texture map that gets mapped to this pixel.
	float4 texColor = tex2D(SpriteS0, tex0).rgba;
	
	// Scale with the sprite color parameter
	return float4( texColor.r*gColor.r, texColor.g*gColor.g, texColor.b*gColor.b, texColor.a*gColor.a );
}

technique Render
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 Vertex();
        pixelShader  = compile ps_2_0 Pixel();
    }
}

technique RenderAlpha
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vertex();
        pixelShader  = compile ps_2_0 PixelAlpha();
    }
}