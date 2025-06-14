#include "Box3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<BoxData3D> gInstanceData : register(t0);

// ───────────────────────────────────────────────
// ローカル定数テーブル
// ───────────────────────────────────────────────

// 8 corner 頂点インデックス → 6 面 × 4 corner への展開
static const uint kCornerLUT[6][4] =
{
    /* Front  */
    { 0, 1, 2, 3 }, // -Z
    /* Back   */
    { 5, 4, 7, 6 }, // +Z
    /* Left   */
    { 4, 0, 6, 2 }, // -X
    /* Right  */
    { 1, 5, 3, 7 }, // +X
    /* Top    */
    { 4, 5, 0, 1 }, // +Y
    /* Bottom */
    { 2, 3, 6, 7 } // -Y
};

// 面ごとの 2 枚ポリゴン（4 頂点）→ CCW インデックス
static const uint3 kTriLUT[12] =
{
    // Front
    uint3(0, 1, 2), uint3(2, 1, 3),
    // Back
    uint3(4, 5, 6), uint3(6, 5, 7),
    // Left
    uint3(8, 9, 10), uint3(10, 9, 11),
    // Right
    uint3(12, 13, 14), uint3(14, 13, 15),
    // Top
    uint3(16, 17, 18), uint3(18, 17, 19),
    // Bottom
    uint3(20, 21, 22), uint3(22, 21, 23)
};

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[12],
    out vertices ShadowMeshOutput verts[24]
)
{
    uint instanceID = payload.instanceID;

    BoxData3D box = gInstanceData[instanceID];

    //　出力頂点 三角形数
    SetMeshOutputCounts(24, 12);

    // 24頂点生成
    [unroll]
    for (uint face = 0; face < 6; ++face)
    {
        [unroll]
        for (uint corner = 0; corner < 4; ++corner)
        {
            uint outIdx = face * 4 + corner;
            uint inIdx = kCornerLUT[face][corner];

            // ローカル → ワールド → クリップ
            float4 localPos = box.offsets[inIdx];
            float4 worldPos = mul(localPos, box.worldMatrix);
            float4 clipPos = mul(worldPos, gCamera.viewProjection);

            verts[outIdx].position = clipPos;
        }
    }

    // インデックス
    [unroll]
    for (uint i = 0; i < 12; ++i)
    {
        tris[i] = kTriLUT[i];
    }
        
}