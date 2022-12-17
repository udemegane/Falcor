def render_graph_ExampleBlitPass():
    g=RenderGraph("ExampleBlitPass example")
    BlitPass=createPass("SimpleCopyPass")
    g.addPass(BlitPass, "SimpleCopyPass")
    ImageLoader=createPass("ImageLoader", {'filename':'Cubemaps\\Sorsele3\\posz.jpg','mips':False, 'srgb':True, 'arrayIndex': 0,'mipLevel': 0})
    g.addPass(ImageLoader, "ImageLoader")
    g.addEdge("ImageLoader.dst", "SimpleCopyPass.src")
    g.markOutput("SimpleCopyPass.dst")
    return g
try: m.addGraph(render_graph_ExampleBlitPass())
except NameError: None
