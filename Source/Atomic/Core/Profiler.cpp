//
// Copyright (c) 2017 the Atomic project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"
#include "../Core/Profiler.h"
#include "../Core/StringUtils.h"


namespace Atomic
{

WeakPtr<Profiler> Profiler::instance_;

Profiler::Profiler(Context* context)
    : Object(context)
{
    instance_ = this;
    SetEnabled(true);    
}

Profiler::~Profiler()
{
    instance_ = 0;
}

void Profiler::SetEnabled(bool enabled)
{
    profiler::setEnabled(enabled);
}

bool Profiler::GetEnabled() const
{
    return profiler::isEnabled();
}

void Profiler::StartListen(unsigned short port)
{
    profiler::startListen(port);
}

void Profiler::StopListen()
{
    profiler::stopListen();
}

bool Profiler::GetListening() const
{
    return profiler::isListening();
}

void Profiler::SetEventTracingEnabled(bool enable)
{
    profiler::setEventTracingEnabled(enable);
}

bool Profiler::GetEventTracingEnabled()
{
    return profiler::isEventTracingEnabled();
}

void Profiler::SetLowPriorityEventTracing(bool isLowPriority)
{
    profiler::setLowPriorityEventTracing(isLowPriority);
}

bool Profiler::GetLowPriorityEventTracing()
{
    return profiler::isLowPriorityEventTracing();
}

void Profiler::SaveProfilerData(const String& filePath)
{
    profiler::dumpBlocksToFile(filePath.CString());
}

void Profiler::SetEventProfilingEnabled(bool enabled)
{
    enableEventProfiling_ = enabled;
}

bool Profiler::GetEventProfilingEnabled() const
{
    return enableEventProfiling_;
}

void Profiler::BeginBlock(const char* name, const char* file, int line, unsigned int color, unsigned char status)
{
    // Line used as starting hash value for efficiency.
    // This is likely to not play well with hot code reload.
    unsigned hash = StringHash::Calculate(file, (unsigned)line);
    HashMap<unsigned, profiler::BaseBlockDescriptor*>::Iterator it = blockDescriptorCache_.Find(hash);
    const profiler::BaseBlockDescriptor* desc = 0;
    if (it == blockDescriptorCache_.End())
    {
        String uniqueName = ToString("%s:%d", file, line);
        desc = ::profiler::registerDescription((profiler::EasyBlockStatus)status, uniqueName.CString(), name, file,
                                               line, profiler::BLOCK_TYPE_BLOCK, color, true);
    }
    else
        desc = it->second_;
    profiler::beginNonScopedBlock(desc, name);
}

void Profiler::BeginBlock(const String& name, const String& file, int line, unsigned int color, unsigned char status)
{
    BeginBlock(name.CString(), file.CString(), line, color, status);
}

void Profiler::EndBlock()
{
    profiler::endBlock();
}

}