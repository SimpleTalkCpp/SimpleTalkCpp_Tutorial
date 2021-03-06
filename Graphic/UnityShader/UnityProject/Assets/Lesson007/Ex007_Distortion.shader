Shader "MyShader/Ex007_Distortion"
{
	Properties
	{
		_MainTex ("Base (RGB)", 2D) = "white" {}
		intensity("intensity", Range(0,1)) = 1
		FlowMap  ("FlowMap", 2D) = "black"
	}

	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100

		Cull Off
		ZTest Always
		ZWrite Off

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 pos : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float4 pos : SV_POSITION;
				float2 uv : TEXCOORD0;
			};

			sampler2D _MainTex;
			sampler2D FlowMap;
			float4 FlowMap_ST;
			float intensity;

			v2f vert (appdata v)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(v.pos);
				o.uv = v.uv;
				return o;
			}

			float4 frag (v2f i) : SV_Target
			{
				float4 flow = tex2D(FlowMap, i.uv * FlowMap_ST.xy + FlowMap_ST.zw) * 2 - 1;

				float4 c = tex2D(_MainTex, i.uv + flow * intensity);
				return c;
			}
			ENDCG
		}
	}
}
