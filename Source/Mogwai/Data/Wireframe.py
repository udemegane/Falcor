from falcor import *

def render_graph_Wireframe():
    g = RenderGraph('WIreframe')
    loadRenderPassLibrary('Wireframe.dll')
    Wireframe = createPass('Wireframe')
    g.addPass(Wireframe, 'Wireframe')
    g.markOutput('Wireframe.output')
    return g

Wireframe = render_graph_Wireframe()
try: m.addGraph(Wireframe)
except NameError: None
