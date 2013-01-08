/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __D3D9HARDWAREVERTEXBUFFER_H__
#define __D3D9HARDWAREVERTEXBUFFER_H__

#include "CmD3D9Prerequisites.h"
#include "CmVertexBuffer.h"
#include "CmD3D9Resource.h"

namespace CamelotEngine {

    /// Specialisation of HardwareVertexBuffer for D3D9
    class CM_D3D9_EXPORT D3D9VertexBuffer : public VertexBuffer, public D3D9Resource
    {   

    public:
		D3D9VertexBuffer(HardwareBufferManager* mgr, UINT32 vertexSize, 
			UINT32 numVertices, GpuBufferUsage usage, bool useSystemMem);
        ~D3D9VertexBuffer();
        /** See HardwareBuffer. */
        void readData(UINT32 offset, UINT32 length, void* pDest);
        /** See HardwareBuffer. */
        void writeData(UINT32 offset, UINT32 length, const void* pSource,
				bool discardWholeBuffer = false);
	
		// Called immediately after the Direct3D device has been created.
		virtual void notifyOnDeviceCreate(IDirect3DDevice9* d3d9Device);

		// Called before the Direct3D device is going to be destroyed.
		virtual void notifyOnDeviceDestroy(IDirect3DDevice9* d3d9Device);

		// Called immediately after the Direct3D device has entered a lost state.
		virtual void notifyOnDeviceLost(IDirect3DDevice9* d3d9Device);

		// Called immediately after the Direct3D device has been reset
		virtual void notifyOnDeviceReset(IDirect3DDevice9* d3d9Device);

		// Create the actual vertex buffer.
		void createBuffer(IDirect3DDevice9* d3d9Device, D3DPOOL ePool);
		
        /// Get D3D9-specific vertex buffer
        IDirect3DVertexBuffer9* getD3D9VertexBuffer(void);

	protected:	
		struct BufferResources
		{
			IDirect3DVertexBuffer9*		mBuffer;
			bool						mOutOfDate;
			UINT32						mLockOffset;
			UINT32						mLockLength;
			GpuLockOptions					mLockOptions;
			UINT32						mLastUsedFrame;
		};

	protected:		
		/** See HardwareBuffer. */
		void* lockImpl(UINT32 offset, UINT32 length, GpuLockOptions options);		
		/** See HardwareBuffer. */
		void unlockImpl(void);			
		// updates buffer resources from system memory buffer.
		bool updateBufferResources(const char* systemMemoryBuffer, BufferResources* bufferResources);		

	protected:
		typedef map<IDirect3DDevice9*, BufferResources*>::type	DeviceToBufferResourcesMap;
		typedef DeviceToBufferResourcesMap::iterator			DeviceToBufferResourcesIterator;

		DeviceToBufferResourcesMap	mMapDeviceToBufferResources;	// Map between device to buffer resources.
		D3DVERTEXBUFFER_DESC		mBufferDesc;					// Buffer description.		
		char*						mSystemMemoryBuffer;			// Consistent system memory buffer for multiple devices support.
    };

}
#endif

