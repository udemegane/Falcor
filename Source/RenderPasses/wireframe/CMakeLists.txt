add_renderpass(wireframe)

target_sources(wireframe PRIVATE
    wireframe.cpp
    wireframe.h
)

target_source_group(wireframe "RenderPasses")
