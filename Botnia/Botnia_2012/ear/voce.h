/*************************************************************************
 *                                                                       *
 * Voce                                                                  *
 * Copyright (C) 2005                                                    *
 * Tyler Streeter  tylerstreeter@gmail.com                               *
 * All rights reserved.                                                  *
 * Web: voce.sourceforge.net                                             *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file license-LGPL.txt.                                          *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file license-BSD.txt.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * license-LGPL.txt and license-BSD.txt for more details.                *
 *                                                                       *
 *************************************************************************/ 

#ifndef VOCE_H
#define VOCE_H

// This file contains a C++ interface for Voce's Java functions.  All of 
// the Java methods in Voce's API are instance methods, so we don't need 
// to handle class methods here.  For documentation on the specific 
// functions, refer to the API documentation for the Java source.

#include <jni.h>
#include <iostream>
#include <string>
#include <cstring>

namespace voce
{
extern const std::string pathSeparator;

/// Contains things that should only be accessed within Voce.
namespace internal
{
/// Global instance of the JNI environment.
extern JNIEnv* gEnv ;

/// Global instance of the Java virtual machine.
extern JavaVM *gJVM ;

/// Global reference to the main Voce Java class.
extern jclass gClass;

/// Global references to the various Java method IDs.  Storing
/// these saves lookup time later.
extern jmethodID gInitID;
extern jmethodID gDestroyID ;
extern jmethodID gSynthesizeID ;
extern jmethodID gStopSynthesizingID;
extern jmethodID gGetRecognizerQueueSizeID;
extern jmethodID gPopRecognizedStringID;
extern jmethodID gSetRecognizerEnabledID;
extern jmethodID gIsRecognizerEnabledID;

/// The name of the main Voce Java class.
extern const std::string gClassName;

/// A simple message logging function.  The message type gets printed
/// before the actual message.
extern void log(const std::string& msgType, const std::string& msg);

/// Finds and returns a method ID for the given function name and
/// Java method signature.
extern jmethodID loadJavaMethodID(const std::string& functionName,
                                  const std::string& functionSignature);
};

extern void init(const std::string& vocePath, bool initSynthesis,
                 bool initRecognition, const std::string& grammarPath,
                 const std::string& grammarName);


/// In addition to the usual Java Voce destroy call, this function
/// performs some extra stuff needed to destroy the virtual machine.
extern void destroy();


/// Requests that the given string be synthesized as soon as possible.
extern void synthesize(const std::string& message);

extern void stopSynthesizing();


/// Returns the number of recognized strings currently in the
/// recognizer's queue.
extern int getRecognizerQueueSize();


/// Returns and removes the oldest recognized string from the
/// recognizer's queue.
extern std::string popRecognizedString();


/// Enables and disables the speech recognizer.
extern void setRecognizerEnabled(bool e);

/// Returns true if the recognizer is currently enabled.
extern bool isRecognizerEnabled();
}

#endif
