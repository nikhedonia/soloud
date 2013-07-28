/*
SoLoud audio engine
Copyright (c) 2013 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#ifndef SOLOUD_H
#define SOLOUD_H

#include <stdlib.h> // rand
#include <math.h> // sin

#ifndef M_PI
#define M_PI 3.14159265359
#endif

namespace SoLoud
{
	typedef void (*mutexCallFunction)();

	class Fader
	{
	public:
		float mFrom;
		float mTo;
		float mDelta;
		float mTime;
		float mStartTime;
		float mEndTime;
		int mActive;
		Fader();
		void set(float aFrom, float aTo, float aTime, float aStartTime);
		float get(float aCurrentTime);
	}; 

	class AudioProducer
	{
	public:
		enum FLAGS
		{
			LOOPING = 1,
			STEREO = 2,
			PROTECTED = 4,
			PAUSED = 8
		};
		AudioProducer();
		virtual ~AudioProducer();
		unsigned int mPlayIndex;
		int mFlags;
		float mLVolume;
		float mRVolume;
		float mVolume;
		float mBaseSamplerate;
		float mSamplerate;
		float mRelativePlaySpeed;
		float mStreamTime;
		Fader mPanFader;
		Fader mVolumeFader;
		Fader mRelativePlaySpeedFader;
		void init(int aPlayIndex, float aBaseSamplerate, int aFactoryFlags);
		virtual void getAudio(float *aBuffer, int aSamples) = 0;
		virtual int hasEnded() = 0;
		virtual void seek(float aSeconds, float *mScratch, int mScratchSize);
		virtual int rewind();
	};

	class AudioFactory
	{
	public:
		enum FLAGS
		{
			SHOULD_LOOP = 1,
			STEREO = 2
		};
		int mFlags;
		float mBaseSamplerate;

		AudioFactory();
		void setLooping(int aLoop);
		virtual ~AudioFactory();
		virtual AudioProducer *createProducer() = 0;
	};


	class Soloud
	{
		float *mScratch;
		int mScratchSize;
		int mScratchNeeded;
		AudioProducer **mChannel;
		int mChannelCount;
		int mSamplerate;
		int mBufferSize;
		int mFlags;
		float mGlobalVolume;
		float mPostClipScaler;
		unsigned int mPlayIndex;
		Fader mGlobalVolumeFader;
		int findFreeChannel();
		float mStreamTime;
		int getChannelFromHandle(int aChannelHandle);
		void stopChannel(int aChannel);
		void setChannelPan(int aChannel, float aPan);
		void setChannelRelativePlaySpeed(int aChannel, float aSpeed);
	public:
		void * mMixerData;
		mutexCallFunction lockMutex;
		mutexCallFunction unlockMutex;

		Soloud();
		~Soloud();

		enum FLAGS
		{
			CLIP_ROUNDOFF = 1
		};

		void init(int aChannels, int aSamplerate, int aBufferSize, int aFlags);
		void mix(float *aBuffer, int aSamples);

		int play(AudioFactory &aSound, float aVolume = 1.0f, float aPan = 0.0f, int aPaused = 0);
		void seek(int aChannelHandle, float aSeconds);
		void stop(int aChannelHandle);
		void stopAll();

		float getStreamTime(int aChannelHandle);
		int getPause(int aChannelHandle);
		float getVolume(int aChannelHandle);
		float getSamplerate(int aChannelHandle);
		int getProtectChannel(int aChannelHandle);
		int getActiveVoiceCount();
		int isValidChannelHandle(int aChannelHandle);
		float getPostClipScaler();
		float getRelativePlaySpeed(int aChannelHandle);
		void setGlobalVolume(float aVolume);
		void setPause(int aChannelHandle, int aPause);
		void setPauseAll(int aPause);
		void setRelativePlaySpeed(int aChannelHandle, float aSpeed);
		void setPostClipScaler(float aScaler);
		void setProtectChannel(int aChannelHandle, int aProtect);
		void setSamplerate(int aChannelHandle, float aSamplerate);
		void setPan(int aChannelHandle, float aPan);
		void setPanAbsolute(int aChannelHandle, float aLVolume, float aRVolume);
		void setVolume(int aChannelHandle, float aVolume);

		void fadeVolume(int aChannelHandle, float aFrom, float aTo, float aTime);
		void fadePan(int aChannelHandle, float aFrom, float aTo, float aTime);
		void fadeRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime);
		void fadeGlobalVolume(float aFrom, float aTo, float aTime);
	};

	int sdl_init(SoLoud::Soloud *aSoloud, int aChannels = 32, int aFlags = Soloud::CLIP_ROUNDOFF, int aSamplerate = 44100, int aBuffer = 2048);
	void sdl_deinit(SoLoud::Soloud *aSoloud);
};

#endif 