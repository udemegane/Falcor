add_renderpass(Wireframe)

target_sources(Wireframe PRIVATE
        Wireframe.cpp
        Wireframe.h
        Wireframe.3d.slang
        )
target_copy_shaders(Wireframe RenderPasses/Wireframe)

target_source_group(Wireframe "RenderPasses")
