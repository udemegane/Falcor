/***************************************************************************
 # Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
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
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
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
#pragma once
#include "Falcor.h"
#include "Utils/Algorithm/ComputeParallelReduction.h"

namespace Falcor
{

    /** Helper class for collecting runtime stats in the path tracer.

        We log per-pixel stats in buffers on the GPU, which are immediately ready for consumption
        after end() is called. These stats are summarized in a reduction pass, which are
        available in getStats() or printStats() after async readback to the CPU.
    */
    class dlldecl PixelStats
    {
    public:
        struct Stats
        {
            uint32_t totalRays = 0;
            float    avgRaysPerPixel = 0.f;
            float    avgPathLength = 0.f;
        };

        using SharedPtr = std::shared_ptr<PixelStats>;
        virtual ~PixelStats() = default;

        static SharedPtr create();

        void beginFrame(RenderContext* pRenderContext, const glm::uvec2& frameDim);
        void endFrame(RenderContext* pRenderContext);

        void prepareProgram(const Program::SharedPtr& pProgram, const ShaderVar& var);

        void renderUI(Gui::Widgets& widget);

        /** Fetches the latest stats generated by begin()/end().
            \param[out] stats The stats are copied here.
            \return True if stats are available, false otherwise.
        */
        bool getStats(PixelStats::Stats& stats);

        /** Returns the per-pixel ray count buffer or nullptr if not available.
            \return Texture in R32Uint format containing per-pixel ray counts, or nullptr if not available.
        */
        const Texture::SharedPtr getRayCountBuffer() const;

    protected:
        PixelStats() {}
        void copyStatsToCPU();

        // Internal state
        ComputeParallelReduction::SharedPtr mpParallelReduction;            ///< Helper for parallel reduction on the GPU.
        Buffer::SharedPtr                   mpReductionResult;              ///< Results buffer for stats readback (CPU mappable).
        GpuFence::SharedPtr                 mpFence;                        ///< GPU fence for sychronizing readback.

        // Configuration
        bool                                mStatsEnabled = false;          ///< UI variable to turn logging on/off.

        // Runtime data
        bool                                mRunning = false;               ///< True inbetween begin() / end() calls.
        bool                                mWaitingForData = false;        ///< True if we are waiting for data to become available on the GPU.
        glm::uvec2                          mFrameDim = { 0, 0 };           ///< Frame dimensions at last call to begin().

        bool                                mStatsValid = false;            ///< True if stats have been read back and are valid.
        Stats                               mStats;                         ///< Traversal stats.

        Texture::SharedPtr                  mpStatsRayCount;                ///< Stats for number of rays traced.
        Texture::SharedPtr                  mpStatsPathLength;              ///< Stats for path length.
        bool                                mStatsBuffersValid = false;     ///< True if per-pixel stats buffers contain valid data.
    };

}