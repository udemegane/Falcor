add_renderpass(SimpleCopyPass)

target_sources(SimpleCopyPass PRIVATE
    SimpleCopyPass.cpp
    SimpleCopyPass.h
)

target_source_group(SimpleCopyPass "RenderPasses")
