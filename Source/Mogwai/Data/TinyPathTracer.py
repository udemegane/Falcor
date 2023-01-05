from falcor import *

def render_graph_CustomPathTracer():
    g = RenderGraph("CustomPathTracer")
    loadRenderPassLibrary("AccumulatePass.dll")
    loadRenderPassLibrary("GBuffer.dll")
    loadRenderPassLibrary("CustomPathTracer.dll")
    loadRenderPassLibrary("ToneMapper.dll")
    AccumulatePass = createPass("AccumulatePass", {'enabled': True, 'precisionMode': AccumulatePrecision.Single})
    g.addPass(AccumulatePass, "AccumulatePass")
    ToneMapper = createPass("ToneMapper", {'autoExposure': False, 'exposureCompensation': 0.0})
    g.addPass(ToneMapper, "ToneMapper")
    CustomPathTracer = createPass("CustomPathTracer", {'maxBounces': 3})
    g.addPass(CustomPathTracer, "CustomPathTracer")
    VBufferRT = createPass("VBufferRT", {'samplePattern': SamplePattern.Stratified, 'sampleCount': 16})
    g.addPass(VBufferRT, "VBufferRT")
    g.addEdge("AccumulatePass.output", "ToneMapper.src")
    g.addEdge("VBufferRT.vbuffer", "CustomPathTracer.vbuffer")
    g.addEdge("VBufferRT.viewW", "CustomPathTracer.viewW")
    g.addEdge("CustomPathTracer.color", "AccumulatePass.input")
    g.markOutput("ToneMapper.dst")
    return g

CustomPathTracer = render_graph_CustomPathTracer()
try: m.addGraph(CustomPathTracer)
except NameError: None
