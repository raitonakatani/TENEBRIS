struct PS_Input
{
	float4 Position : SV_POSITION;
	float4 VColor : COLOR;
	float2 UV1 : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldP : TEXCOORD2;
	float3 WorldN : TEXCOORD3;
	float3 WorldT : TEXCOORD4;
	float3 WorldB : TEXCOORD5;
	float2 ScreenUV : TEXCOORD6;
};

float4	fLightDirection		: register( c0 );
float4	fLightColor		: register( c1 );
float4	fLightAmbient		: register( c2 );

Texture2D	g_colorTexture		: register( t0 );
sampler2D	g_colorSampler		: register( s0 );

Texture2D	g_normalTexture		: register( t1 );
sampler2D	g_normalSampler		: register( s1 );

float4 PS( const PS_Input Input ) : COLOR
{
	half3 texNormal = (tex2D(g_normalSampler, Input.UV1).xyz - 0.5) * 2.0;
	half3 localNormal = (half3)normalize(mul(texNormal, half3x3((half3)Input.WorldT, (half3)Input.WorldB, (half3)Input.WorldN)));

	float diffuse = max(dot(fLightDirection.xyz, localNormal.xyz), 0.0);

	float4 Output = tex2D(g_colorSampler, Input.UV1) * Input.VColor;
	Output.xyz = Output.xyz * (fLightColor.xyz * diffuse + fLightAmbient);

	if (Output.a <= 0.0f)
		discard;

	return Output;
}
