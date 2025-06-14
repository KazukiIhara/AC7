struct CylinderData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    uint cylinderDivide;
    float topRadius;
    float bottomRadius;
    float height;
    bool hasCap;
    float3 padding;
};

// タイルの最大数
static const uint kMaxTileCount = 256 * 256;