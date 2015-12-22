#ifndef _MPxMayaAsciiFilter
#define _MPxMayaAsciiFilter
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS DESCRIPTION (MPxMayaAsciiFilter)
//
//  This class allows the output to the Maya Ascii (.ma) file format to be
//	filtered by a plug-in. This can be used to, for example, partition a scene
//	into several files which have been each filtered to write out different
//	parts of the scene.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MPxFileTranslator.h>
#include <maya/MFileObject.h>

// ****************************************************************************
// DECLARATIONS

class MObject;
class MPlug;
class MFileIO;
class MDagPath;
class MFileObject;

// MPxMayaAsciiFilterOutput is used to allow output to the Maya ASCII file.
// Simply use the output operator (<<) to write a char* buffer to the output
// stream.
class OPENMAYA_EXPORT MPxMayaAsciiFilterOutput
{
public:
	MPxMayaAsciiFilterOutput(void* outputFile);

	MPxMayaAsciiFilterOutput&		operator << (const char* data);

protected:
	// No protected members

private:
	void*				file;

};

// ****************************************************************************
// CLASS DECLARATION (MPxMayaAsciiFilter)

//! \ingroup OpenMaya MPx
//! \brief Translator to output filtered Maya ASCII files. 
/*!
MPxMayaAsciiFilter allows output to the Maya ASCII (.ma) file format to be
filtered by a plug-in. The file is output using the same internal mechanism
that is used for normal Maya ASCII output, but allows your plug-in to prevent
certain lines (such as createNodes or connectAttrs) from being output to the
file. In addition, it allows you to write directly to the file at specific
points during output.

MPxMayaAsciiFilter allows your plug-in to write out files similar to Maya ASCII
in a custom format. It can be used, for example, to partition a scene into several
files, each of which has been filtered to write out different parts of
the scene. Note that the order of the file cannot be changed - for example,
connectAttrs are always written after createNodes - but it does allow you to control
which connectAttrs and createNodes are output.

MPxMayaAsciiFilter inherits from MPxFileTranslator. It overrides the
haveWriteMethod(), haveReadMethod(), reader(), and writer() methods in
MPxFileTranslator in such a way that the internal Maya mechanism for reading and
writing Maya ASCII files is used. Because of this, it is not advised that you
override these methods in your plug-in. If you do, make sure you first call these
methods in MPxMayaAsciiFilter. A pseudo-code example is shown here:

\code
MyAsciiFilter::writer(...)
{
	// do something before writing to file here
	...

	// we will now write to the file

	MStatus result = MPxMayaAsciiFilter::writer(...);

	if (result == MStatus::kSuccess)
	{
		...	// do something after writing the file here
	}

	return result;
}
\endcode

To determine if a given line should be output, override the methods
writesCreateNode(), writesSetAttr() and so on. Each of these methods
takes as its arguments the relevant data which is about to be processed
and your plug-in can use this to determine whether or not the line will
be output.

For example, you may want to write out everything but render layer data.
The code to do this might look something like what is shown here:

\code
bool
MyAsciiFilter::writesCreateNode( const MObject& node )
{
	if (node.hasFn(MFn::kRenderLayer))
	{
		return true;
	}

	return false;
}

bool
MyAsciiFilter::writesConnectAttr( const MPlug& srcPlug,
								  const MPlug& destPlug )
{
	if (srcPlug.node().hasFn(MFn::kRenderLayer) ||
		dstPlug.node().hasFn(MFn::kRenderLayer))
	{
		return true;
	}

	return false;
}
\endcode

Note that writesConnectAttr() has to be overridden as well (as would
writesSetAttr(), writesSelectAttr(), and so on). This is because each line is
handled separately, and nothing clever is done in the base class' implementation
of writesConnectAttr(), such as having a default behaviour of only outputting
connectAttrs for nodes that are being output. This gives you the most flexible
control over what will be written.

If you were using this class to save a segmented file, a master file might have
to know about the locations of each of the segments. If your file saved everything
but render layers in one file, and render layers in another, the first file might
need to contain a pointer to the second, so that when the first file is loaded the
second gets read into memory too. To do this, you would use MPxMayaAsciiFilter's
ability to write directly to the stream during file output.

You can write to the stream during output by overriding the methods
writePreCreateNodesBlock(), writePostCreateNodesBlock() and so on. These give
you a simple mechanism to write directly to the file. Each of these methods
take an MPxMayaAsciiFilterOutput as its only argument. To write to the file,
simply use the output operator (MPxMayaAsciiFilterOutput::operator<<() )
and output the string you wish.

In the example above, the pointer to the render layer file might be inserted into
the main file after all nodes have been created. The code to do this would look
something like what is shown here:

\code
MStatus
MyAsciiFilter::writePostCreateNodesBlock( MPxMayaAsciiFilterOutput& fileIO )
{
	fileIO << "eval(\"importRenderLayerFile \\\"MyLayerFile.maf\\\";\")\n";
}
\endcode

In the above example, after the createNode block was output, the line

\code
eval("importRenderLayerFile \"MyLayerFile.maf\"");
\endcode

was output. When the file is read in, Maya would reach the line above
and execute it like any other MEL command. It is assumed in this
example that there would be a custom command plug-in called
importRenderLayerFile which would do the work of actually importing
the layer file.

Note:
MPxMayaAsciiFilter gives you the ability to create your own custom file formats
based on standard Maya ASCII files. However, Autodesk does not support such custom
files as they will not, by definition, be standard Maya ASCII files.
*/
class OPENMAYA_EXPORT MPxMayaAsciiFilter : public MPxFileTranslator
{
public:
						MPxMayaAsciiFilter ();
				virtual ~MPxMayaAsciiFilter ();
	virtual bool		haveWriteMethod() const;
	virtual bool		haveReadMethod() const;
	virtual MStatus		reader ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);
	virtual MStatus		writer ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);

protected:

	const MFileObject	file() const;
	virtual MStatus		processReadOptions(const MString& optionsString);
	virtual MStatus		processWriteOptions(const MString& optionsString);
	virtual	bool		writesRequirements(  ) const;
	virtual	bool		writesCreateNode( const MObject& node ) const;
	virtual	bool		writesSetAttr( const MPlug& srcPlug ) const;
	virtual	bool		writesConnectAttr( const MPlug& srcPlug,
										   const MPlug& destPlug ) const;
	virtual	bool		writesDisconnectAttr( const MPlug& srcPlug,
											  const MPlug& destPlug ) const;
	virtual	bool		writesParentNode( const MDagPath& parent, const MDagPath& child ) const;
	virtual	bool		writesSelectNode( const MObject& node ) const;
	virtual	bool		writesFileReference( const MFileObject& referenceFile ) const;
	virtual MStatus		writePostHeader( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePostRequires( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePreCreateNodesBlock( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePostCreateNodesBlock( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePreConnectAttrsBlock( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePostConnectAttrsBlock( MPxMayaAsciiFilterOutput& fileIO );
	virtual MStatus		writePreTrailer( MPxMayaAsciiFilterOutput& fileIO );

private:

	MFileObject			fileObject;

};

#endif /* __cplusplus */
#endif /* _MPxMayaAsciiFilter */
