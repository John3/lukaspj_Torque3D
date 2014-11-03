//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "particleEmitter.h"
#include "console/consoleTypes.h"
#include "core/stream/bitStream.h"

IMPLEMENT_NONINSTANTIABLE_CLASS(ParticleEmitterData,
   "")
   END_IMPLEMENT_CLASS;

static const float sgDefaultEjectionOffset = 0.f;
static const float sgDefaultPhiReferenceVel = 0.f;
static const float sgDefaultPhiVariance = 360.f;

ParticleEmitterData::ParticleEmitterData()
{
   mEjectionVelocity = 2.0f;
   mVelocityVariance = 1.0f;
   mEjectionOffset = sgDefaultEjectionOffset;
   mEjectionOffsetVariance = 0.0f;
}

void ParticleEmitterData::initPersistFields()
{
   addGroup("ParticleEmitterData");

   addField("EjectionVelocity", TypeF32, Offset(mEjectionVelocity, ParticleEmitterData),
      "Particle ejection velocity.");

   addField("VelocityVariance", TypeF32, Offset(mVelocityVariance, ParticleEmitterData),
      "Variance for ejection velocity, from 0 - ejectionVelocity.");

   addField("EjectionOffset", TypeF32, Offset(mEjectionOffset, ParticleEmitterData),
      "Distance along ejection Z axis from which to eject particles.");

   addField("EjectionOffsetVariance", TypeF32, Offset(mEjectionOffsetVariance, ParticleEmitterData),
      "Distance Padding along ejection Z axis from which to eject particles.");

   endGroup("ParticleEmitterData");

   Parent::initPersistFields();
}

void ParticleEmitterData::packData(BitStream* stream)
{
   Parent::packData(stream);

   stream->writeInt((S32)(mEjectionVelocity * 100), 16);
   stream->writeInt((S32)(mVelocityVariance * 100), 14);
   if (stream->writeFlag(mEjectionOffset != sgDefaultEjectionOffset))
      stream->writeInt((S32)(mEjectionOffset * 100), 16);
   if (stream->writeFlag(mEjectionOffsetVariance != 0.0f))
      stream->writeInt((S32)(mEjectionOffsetVariance * 100), 16);
}

void ParticleEmitterData::unpackData(BitStream* stream)
{
   Parent::unpackData(stream);

   mEjectionVelocity = stream->readInt(16) / 100.0f;
   mVelocityVariance = stream->readInt(14) / 100.0f;
   if (stream->readFlag())
      mEjectionOffset = stream->readInt(16) / 100.0f;
   else
      mEjectionOffset = sgDefaultEjectionOffset;
   if (stream->readFlag())
      mEjectionOffsetVariance = stream->readInt(16) / 100.0f;
   else
      mEjectionOffsetVariance = 0.0f;
}