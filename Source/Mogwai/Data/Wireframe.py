from falcor import *

def render_graph_Wireframe():
    g = RenderGraph('WireframePass')
    loadRenderPassLibrary('WireframePass.dll')
    Wireframe = createPass('WireframePass')
    g.addPass(Wireframe, 'WireframePass')
    g.markOutput('WireframePass.output')
    return g

WireframePass = render_graph_Wireframe()
try: m.addGraph(WireframePass)
except NameError: None
