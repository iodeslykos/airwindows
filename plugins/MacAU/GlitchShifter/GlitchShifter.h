/*
*	File:		GlitchShifter.h
*	
*	Version:	1.0
* 
*	Created:	2/19/07
*	
*	Copyright:  Copyright � 2007 Airwindows, Airwindows uses the MIT license
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "AUEffectBase.h"
#include "GlitchShifterVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __GlitchShifter_h__
#define __GlitchShifter_h__


#pragma mark ____GlitchShifter Parameters

// parameters
static const float kDefaultValue_ParamOne = 0;
static const float kDefaultValue_ParamTwo = 0.0;
static const float kDefaultValue_ParamThree = 0.5;
static const float kDefaultValue_ParamFour = 0.0;
static const float kDefaultValue_ParamFive = 0.5;
//let's assume we always have a default of 0.0, for no effect

static CFStringRef kParameterOneName = CFSTR("Note");
static CFStringRef kParameterTwoName = CFSTR("Trim");
static CFStringRef kParameterThreeName = CFSTR("Tightness");
static CFStringRef kParameterFourName = CFSTR("Feedback");
static CFStringRef kParameterFiveName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	//Add your parameters here...
	kNumberOfParameters=5
};

#pragma mark ____GlitchShifter
class GlitchShifter : public AUEffectBase
{
public:
	GlitchShifter(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~GlitchShifter () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new GlitchShifterKernel(this); }
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*32767.0;}
    virtual Float64				GetLatency() {return 0.0;}
	/*! @method Version */
	virtual ComponentResult	Version() { return kGlitchShifterVersion; }
	
    
	
protected:
		class GlitchShifterKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		GlitchShifterKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
	}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
		private: 
		SInt32 p[131076];
		SInt32 offset[258];
		SInt32 pastzero[258];
		SInt32 previous[258];
		SInt32 third[258];
		SInt32 fourth[258];
		SInt32 lastwidth;
		SInt32 temp;
		SInt32 lasttemp;
		SInt32 thirdtemp;
		SInt32 fourthtemp;
		SInt32 sincezerocross;
		int gcount;
		int crosses;
		int realzeroes;
		Float64 airPrev;
		Float64 airEven;
		Float64 airOdd;
		Float64 airFactor;
		Float64 position;
		bool flip;
		bool splicing;
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif