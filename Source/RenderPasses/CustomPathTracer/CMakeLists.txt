add_renderpass(CustomPathTracer)

target_sources(CustomPathTracer PRIVATE
        CustomPathTracer.cpp
        CustomPathTracer.h
        CustomPathTracer.rt.slang
        )

target_copy_shaders(CustomPathTracer RenderPasses/CustomPathTracer)

target_source_group(CustomPathTracer "RenderPasses")
