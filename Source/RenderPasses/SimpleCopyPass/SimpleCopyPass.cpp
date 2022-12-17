/***************************************************************************
 # Copyright (c) 2015-22, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/
#include "SimpleCopyPass.h"
#include "RenderGraph/RenderPassLibrary.h"

const RenderPass::Info SimpleCopyPass::kInfo{"SimpleCopyPass", "Insert pass description here."};

// Don't remove this. it's required for hot-reload to function properly
extern "C" FALCOR_API_EXPORT const char *getProjDir()
{
    return PROJECT_DIR;
}

extern "C" FALCOR_API_EXPORT void getPasses(Falcor::RenderPassLibrary &lib)
{
    lib.registerPass(SimpleCopyPass::kInfo, SimpleCopyPass::create);
}

SimpleCopyPass::SharedPtr SimpleCopyPass::create(RenderContext *pRenderContext, const Dictionary &dict)
{
    SharedPtr pPass = SharedPtr(new SimpleCopyPass());
    return pPass;
}

Dictionary SimpleCopyPass::getScriptingDictionary()
{
    return Dictionary();
}

RenderPassReflection SimpleCopyPass::reflect(const CompileData &compileData)
{
    // Define the required resources here
    RenderPassReflection reflector;
    reflector.addInput("src", "the source texture.");
    reflector.addOutput("dst", "the destination texture");
    return reflector;
}

void SimpleCopyPass::execute(RenderContext *pRenderContext, const RenderData &renderData)
{
    const auto &pSrcTex = renderData.getTexture("src");
    const auto &pDstTex = renderData.getTexture("dst");
    if (pSrcTex && pDstTex)
    {
        pRenderContext->blit(pSrcTex->getSRV(), pDstTex->getRTV());
    }
    else
    {
        logWarning("SimpleCopyPass::execute() - missing an input or output resource.");
    }
    // renderData holds the requested resources
    // auto& pTexture = renderData.getTexture("src");
}

void SimpleCopyPass::renderUI(Gui::Widgets &widget)
{
}
