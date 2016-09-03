//=============================================================================
// phonglighting.fx by Pablo Zorrilla (C) 2010 All Rights Reserved.
//
// Does basic ambient, diffuse, and specular lighting.
//=============================================================================

// Extern matrix declarations
uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInverseTranspose;
uniform extern float4x4 gWVP;
uniform extern float4x4 gVP;

// Ambient material and lighting
uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;

// Diffuse material and lighting
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;

// Specular material and lighting
uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float  gSpecularPower;

// Light vectors
uniform extern float3 gLightVecW;
uniform extern float3 gLightPosW;
uniform extern float3 gLightDirW;

uniform extern float3 gAttenuation012;

uniform extern float3 gEyePosW;
uniform extern float gSpotPower;


struct HomNWPW
{
      float4 posH    : POSITION0;
      float3 normalW : TEXCOORD0;
      float3 posW    : TEXCOORD1;
};

float4 CalcDiffuse( float3 normalW : TEXCOORD0, float3 lightVecW : TEXCOORD1 )
{
	// Compute the color and return: from Equation 10.1.
	float s = max( dot(lightVecW, normalW), 0.0f );
	return float4( s*(gDiffuseMtrl*gDiffuseLight).rgb, gDiffuseMtrl.a );	
}

float4 CalcAmbient()
{
	// Compute the color and return result: from Equation 10.2
	return float4( ( gAmbientMtrl*gAmbientLight ).rgb, gAmbientMtrl.a );
}

float4 CalcSpecular( float3 normalW : TEXCOORD0, float3 posW : TEXCOORD1, float3 lightVecW : TEXCOORD2 )
{
	//=======================================================
	// Compute the color: from Equation 10.3.
	// Compute the vector from the vertex to the eye position.
	float3 toEye = normalize(gEyePosW - posW);
	
	// Compute the reflection vector.
	float3 r = reflect(-lightVecW, normalW);
	
	// Determine how much (if any) specular light makes it into the eye and return the last result.
	float t  = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	return float4( t*( gSpecularMtrl*gSpecularLight ).rgb, gSpecularMtrl.a );
}

float CalcAttenuation( float3 posW : TEXCOORD0 )
{
	// Attentuation.
	float d = distance(gLightPosW, posW);
	return gAttenuation012.x + gAttenuation012.y*d + gAttenuation012.z*d*d; // attenuation
}


// The directional lighting vertex shader
HomNWPW DirectionalVS(float3 posL : POSITION0, float3 normalL : NORMAL0)
{
      // Zero out our output.
      HomNWPW outVS = (HomNWPW)0;

      // Transform normal to world space.
      outVS.normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
      outVS.normalW = normalize(outVS.normalW);

      // Transform vertex position to world space.
      outVS.posW  = mul(float4(posL, 1.0f), gWorld).xyz;

      // Transform to homogeneous clip space.
      outVS.posH = mul(float4(posL, 1.0f), gWVP);

      // Done--return the output.
      return outVS;
}

// The directional lighting pixel shader
float4 DirectionalPS(float3 normalW : TEXCOORD0, float3 posW : TEXCOORD1) : COLOR
{
	normalW = normalize(normalW);
	
	// Compute the ambient, diffuse, and specular terms separately.
	float3 spec = CalcSpecular( normalW, posW, gLightVecW );
	float3 diffuse = CalcDiffuse( normalW, gLightVecW );
	float3 ambient = CalcAmbient();
	
	// Sum all the terms together and copy over the diffuse alpha.
	//return float4(ambient + diffuse + spec, gDiffuseMtrl.a);
	return gDiffuseMtrl;
}

technique RenderDirectional
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 DirectionalVS();
        pixelShader  = compile ps_2_0 DirectionalPS();
    }
}

HomNWPW PointLightVS(float3 posL : POSITION0, float3 normalL : NORMAL0 )
{
      // Zero out our output.
      HomNWPW outVS = (HomNWPW)0;

      // Transform normal to world space.
      outVS.normalW = mul(float4(normalL, 0.0f),
                          gWorldInverseTranspose).xyz;
      outVS.normalW = normalize(outVS.normalW);

      // Transform vertex position to world space.
      outVS.posW  = mul(float4(posL, 1.0f), gWorld).xyz;

      // Transform to homogeneous clip space.
      outVS.posH = mul(float4(posL, 1.0f), gWVP);

      // Done--return the output.
      return outVS;
}

float4 PointLightPS( float3 normalW : TEXCOORD0, float3 posW : TEXCOORD1 ) : COLOR
{
	// Unit vector from vertex to light source.
	normalW = normalize( normalW );

	float3 lightVecW = normalize(gLightPosW - posW);

	// Compute the ambient, diffuse, and specular terms separately.
	float3 specular = CalcSpecular( normalW, posW, lightVecW );
	float3 diffuse = CalcDiffuse( normalW, lightVecW );
	float3 ambient = CalcAmbient();
	
	// add colors and attenuate
	return float4( ( ambient + ( (diffuse+specular)/CalcAttenuation(posW) ).rgb ), gDiffuseMtrl.a );
//	return float4( ( (ambient+diffuse+specular)/CalcAttenuation(posW) ).rgb, gDiffuseMtrl.a );
}

technique RenderPointLight
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 PointLightVS();
        pixelShader  = compile ps_2_0 PointLightPS();
    }
}


HomNWPW SpotLightVS(float3 posL : POSITION0, float3 normalL : NORMAL0 )
{
    // Zero out our output.
	HomNWPW outVS = (HomNWPW)0;
	
	// Transform normal to world space.
	outVS.normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;

	outVS.normalW = normalize(outVS.normalW);

	// Transform vertex position to world space.
	outVS.posW  = mul(float4(posL, 1.0f), gWorld).xyz;

	// Transform vertex position to homogeneous space.
	outVS.posH  = mul(float4(posL, 1.0f), gWVP);

	// Done - return the output.
	return outVS;
}

float4 SpotLightPS(float3 normalW : TEXCOORD0, float3 posW : TEXCOORD1 ) : COLOR
{
	// Unit vector from vertex to light source.
	normalW = normalize( normalW );

	float3 lightVecW = normalize(gLightPosW - posW);

	// Compute the ambient, diffuse, and specular terms separately.
	float3 specular = CalcSpecular( normalW, posW, lightVecW );
	float3 diffuse = CalcDiffuse( normalW, lightVecW );
	float3 ambient = CalcAmbient();
	
	// add colors and attenuate
	// Spotlight factor.
	float spot = pow(max(dot(-lightVecW, gLightDirW), 0.0f), gSpotPower);
	
	// Calculate color and return the output.
	return float4( spot*(ambient + ( (diffuse+specular)/CalcAttenuation(posW) )), gDiffuseMtrl.a );
}

technique RenderSpotLight
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 SpotLightVS();
        pixelShader  = compile ps_2_0 SpotLightPS();
    }
}