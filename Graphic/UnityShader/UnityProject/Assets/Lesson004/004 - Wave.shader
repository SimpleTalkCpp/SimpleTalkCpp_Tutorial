Shader "MyShader/004 - Wave"
{
	Properties {
		waveLength("waveLegngth", range(0.001, 1)) = 0.1
		amplitude("amplitude", range(0, 1)) = 0.3
		testTex("testTex", 2D) = "Black"
	}

	SubShader
	{
		Tags { "RenderType"="Opaque" }

		Pass
		{
			Cull Off
			
		//---------------
			CGPROGRAM
			#pragma vertex vs_main
			#pragma fragment ps_main
			
			#include "UnityCG.cginc"

			struct appdata {
				float4 pos		: POSITION;
				float4 color	: COLOR;
				float2 uv		: TEXCOORD0;
				float3 normal	: NORMAL;
			};

			struct v2f {
				float4 pos		: SV_POSITION;
				float4 color	: COLOR;
				float2 uv		: TEXCOORD0;
				float3 normal	: NORMAL;
			};

			float waveLength;
			float amplitude;
		
			v2f vs_main (appdata v) {
				v2f o;
//				o.pos = UnityObjectToClipPos(v.pos);

				float4 wpos = mul(unity_ObjectToWorld, v.pos);
				wpos.y += sin(_Time.y + v.uv.y / waveLength) * amplitude;

				o.pos = mul(UNITY_MATRIX_VP, wpos);

				o.color = v.color;
				o.uv = v.uv;
				o.normal = v.normal;
				return o;
			}

			sampler2D	testTex;

			float4 ps_main (v2f i) : SV_Target {
				return tex2D(testTex, i.uv);
			}
			ENDCG
		}
	}
}
