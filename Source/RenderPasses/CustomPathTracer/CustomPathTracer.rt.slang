#include "Scene/SceneDefines.slangh"
#include "Utils/Math/MathConstants.slangh"

// __exported importで補完が効かない
import Scene.HitInfo;
import Scene.HitInfoType;
import Scene.Raytracing;
import Scene.Intersection;
import Utils.Math.MathHelpers;
import Utils.Geometry.GeometryHelpers;
import Utils.Sampling.SampleGenerator;
import Rendering.Lights.LightHelpers;

cbuffer CB
{
    uint gFrameCount;
    uint gPRNGDimension;
}

// Inputs
Texture2D<PackedHitInfo> gVBuffer;
Texture2D<float4> gViewW;

RWTexture2D<float4> gOutputColor;

#define is_valied(name) (is_valied_##name != 0)
static const uint kMaxBounces = MAX_BOUNCES;
static const bool kComputeDirect = COMPUTE_DIRECT;
static const bool kDirectOnly = DIRECT_ONLY;
static const bool kUseAnalyticLight = USE_ANALYTIC_LIGHT;
static const bool kUseEmissiveLight = USE_EMISSIVE_LIGHT;
static const bool kUseEnvLight = USE_ENV_LIGHT;
static const bool kUseEnvBackground = USE_ENV_BACKGROUND;
static const float3 kDefaultBackgroundColor = float3(0, 0, 0);
static const float kRayMax = FLT_MAX;

// shadow ray strcuture
struct ShadowRayData
{
    bool visible;
}

// scatter ray structure
struct ScatterRayData
{
    float3 radiance;
    float3 throughput;
    float3 origin;
    float3 direction;
    uint pathLength;
    bool terminated;

    SampleGenerator sg;
    __init(Samplegenerator sg)
    {
        this.sg = sg;

        this.terminated = false;
        this.pathLength = 0;
        this.radiance = float3(0, 0, 0);
        this.throughput = float3(1, 1, 1);
        this.origin = float3(0, 0, 0);
        this.direction = float3(0, 0, 0);
    }
}

float3 getPrimaryRayDir(uint2 pixel, uint2 screen, const Camera camera)
{
    if (is_valied(gViewW))
    {
        return -gViewW[pixel].xyz;
    }
    else
    {
        return camera.computeRayPinhole(pixel, screen).dir;
    }
}

ShadingData loadShadingData(const HitInfo hit, const float3 rayOrigin, const float3 rayDir, const ITextureSampler lod)
{
    VertexData vertexData = {};
    uint materialID = {};

// プリプロセッサかますと補完が壊れる
#if SCENE_HAS_GEOMETRY_TYPE(GEOMETRY_TYPE_TRIANGLE_MESH)
    if (hit.getType() == HitType::Triangle)
    {
        const TriangleHit th = hit.getTriangleHit();
        vertexData = gScene.getVertexData(th);
        materialID = gScene.getMaterialID(th.instanceID);
    }
#endif

#if SCENE_HAS_GEOMETRY_TYPE(GEOMETRY_TYPE_DISPLACED_TRIANGLE_MESH)
    if (hit.getType() == HitType::DisplacedTriangle)
    {
        const DisplacedTriangleHit dth = hit.getDisplacedTriangleHit();
        vertexData = gScene.getVertexData(dth, -rayDir);
        materialID = gScene.getMaterialID(dth.instanceID);
    }
#endif

#if SCENE_HAS_GEOMETRY_TYPE(GEOMETRY_TYPE_CURVE)
    if (hit.getType() == HitType::Curve)
    {
        const CurveHit ch = hit.getCurveHit();
        vertexData = gScene.getVertexDataFromCurve(ch);
        materialID = gScene.getMaterialID(ch.instanceID);
    }
#endif

#if SCENE_HAS_GEOMETRY_TYPE(GEOMETRY_TYPE_SDF_GRID)
    if (hit.getType() == HitType::SDFGrid)
    {
        const SDFGridHit sh = hit.getSDFGridHit();
        vertexData = gScene.getVertexDataFromSDFGrid(sh, rayOrigin, rayDir);
        materialID = gScene.getMaterialID(sh.instanceID);
    }
#endif

#if SCENE_HAS_GEOMETRY_TYPE(GEOMETRY_TYPE_CUSTOM)
    {
    }
#endif
    ShadingData sd = gScene.materials.prepareShadingData(vertexData, materialID, -rayDir, lod);
    return sd;
}

// raytracing main function
float3 tracePath(uint2 pixel, uint2 screen)
{
    float3 outputColor = float3(0, 0, 0);

    const float3 primaryRayDir = getPrimaryRayDir(pixel, screen, gScene.camera);
    const float3 primaryRayOrigin = gScene.camera.getPosition();

    const HitInfo hit = HitInfo(gVBuffer[pixel]);
    if (hit.isValid())
    {
        const bool isCurveHit = hit.getType() == HitType::Curve;
        let lod = ExplicitLodTextureSampler(0.f);

        ShadingData sd = loadShadingData(hit, primaryRayOrigin, primaryRayDir, lod);

        let bsdf = gScene.materials.getBSDF(sd, lod);
        SampleGenerator sg = SampleGenerator(pixel, gFrameCount);
        for (uint i = 0; i < gPRNGDimension; i++)
            sampleNext1D(sg);

        float3 rayOrigin;
        // ↓これじゃダメなのか？
        rayOrigin = sd.computeNewRayOrigin();

        // if (isCurveHit)
        // {
        // } else {

        // }

        if (kComputeDirect)
        {
        }
    }
}

// hardware ray tracing shaders

[shader("raygeneration")]
void rayGen()
{
    uint2 pixel = DispatchRaysIndex().xy;
    uint2 screen = DispatchRaysDimensions().xy;
    float3 color = tracePath(pixel, screen);
    gOutputColor[pixel] = float4(color, 1);
}
