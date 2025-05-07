cbuffer CBChangesEveryFrame : register(b0)
{
    matrix g_WorldViewProj;
}

struct VertexPos
{
    float3 posL : POSITION;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posL : POSITION;
};

VertexOut VS(VertexPos vIn)
{
    VertexOut vOut;
    
    // ����z = wʹ��z/w = 1(��պб�����NDC����Զƽ��)
    float4 posH = mul(float4(vIn.posL, 1.0f), g_WorldViewProj);
    vOut.posH = posH.xyww;
    vOut.posL = vIn.posL;
    return vOut;
}
