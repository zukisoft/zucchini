//---------------------------------------------------------------------------
// StoragePropertySetEnumerator Implementation
//
// ZukiSoft Structured Storage
//
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or any other,
// from this software.
//
// Contributor(s):
//	Michael G. Brehm (original author)
//---------------------------------------------------------------------------

#include "stdafx.h"							// Include project pre-compiled headers
#include "StoragePropertySetEnumerator.h"	// Include this class' declarations
#include "StructuredStorage.h"				// Include StructuredStorage declarations
#include "StoragePropertySet.h"				// Include StoragePropertySet decls

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(data)
BEGIN_NAMESPACE(structured)

//---------------------------------------------------------------------------
// StoragePropertySetEnumerator Constructor (internal)
//
// Arguments:
//
//	root			- Reference to the root Storage object
//	storage			- Reference to the current ComStorage object

StoragePropertySetEnumerator::StoragePropertySetEnumerator(StructuredStorage^ root, 
	ComStorage^ storage) : m_root(root), m_storage(storage), m_current(-1)
{
	Generic::ICollection<Guid>^		guids;		// Enumerated storage GUIDs

	if(m_root == nullptr) throw gcnew ArgumentNullException();
	if(m_storage == nullptr) throw gcnew ArgumentNullException();

	// This used to have all kinds of code that was quite frankly stupid, since it
	// physically enumerated the storage looking for things, and then checked with
	// the name mapper to see if that thing is valid.  NOW it just asks the name mapper
	// for everything it knows about, saving both execution time and resources

	guids = m_storage->PropertySetNameMapper->ToDictionary()->Values;
	m_items = gcnew array<Guid>(guids->Count);
	if(m_items->Length > 0) guids->CopyTo(m_items, 0);
}

//---------------------------------------------------------------------------
// StoragePropertySetEnumerator::Current::get
//
// Retrieves the object at the current position in the enumerator.  In this
// implementation, we construct a new StoragePropertySet object each time the
// user happens to ask for it
//
// Arguments:
//
//	NONE

StoragePropertySet^ StoragePropertySetEnumerator::Current::get(void)
{
	Guid					propsetid;			// Property set ID code
	ComPropertyStorage^		propStorage;		// ComPropertyStorage instance
	IPropertyStorage*		pPropStorage;		// Pointer to the property storage
	String^					name;				// BASE64 object name
	PinnedStringPtr			pinName;			// Pinned object name
	HRESULT					hResult;			// Result from function call

	CHECK_DISPOSED(m_disposed || m_storage->IsDisposed());
	
	// The enumerator must be positioned somewhere between the first and last elements

	if((m_current < 0) || (m_current >= m_items->Length)) throw gcnew InvalidOperationException();

	propsetid = m_items[m_current];				// Grab the current object GUID

	lock cacheLock(m_root->ComPropStorageCache->SyncRoot);	// <--- THREAD SAFETY

	// If an instance of this property set already exists somewhere, we
	// can get a copy of that pointer and just wrap with a new class

	if(m_root->ComPropStorageCache->TryGetValue(propsetid, propStorage))
		return gcnew StoragePropertySet(m_storage, propStorage);

	// The interface hasn't been cached (or is dead), so we need to make a new one

	name = StorageUtil::SysGuidToBase64(propsetid);	// Convert GUID to BASE64
	pinName = PtrToStringChars(name);				// Pin it down

	// Attempt to open up the object's IPropertyStorage interface using the same mode
	// flags as the parent IStorage interface

	hResult = m_storage->OpenPropertySet(StorageUtil::SysGuidToUUID(propsetid), 
		StorageUtil::GetStorageMode(m_storage), &pPropStorage);
	if(FAILED(hResult)) throw gcnew StorageException(hResult, propsetid.ToString("D"));

	// Create a new ComPropertyStorage wrapper for the property set, and cache it
	// off in case anyone else tries to access it later on ...

	propStorage = gcnew ComPropertyStorage(pPropStorage);
	pPropStorage->Release();

	m_root->ComPropStorageCache->Add(propsetid, propStorage);
	return gcnew StoragePropertySet(m_storage, propStorage);
}

//---------------------------------------------------------------------------
// StoragePropertySetEnumerator::MoveNext
//
// Advances the enumerator to the next item in the collection
//
// Arguments:
//
//	NONE

bool StoragePropertySetEnumerator::MoveNext(void)
{
	CHECK_DISPOSED(m_disposed || m_storage->IsDisposed());

	m_current++;								// Move to the next element
	return (m_current < m_items->Length);		// True/False based on position
}

//---------------------------------------------------------------------------
// StoragePropertySetEnumerator::Reset
//
// Resets the enumerator to right before the first collection element
//
// Arguments:
//
//	NONE

void StoragePropertySetEnumerator::Reset(void)
{
	CHECK_DISPOSED(m_disposed || m_storage->IsDisposed());
	m_current = -1;
}

//---------------------------------------------------------------------------

END_NAMESPACE(structured)
END_NAMESPACE(data)
END_NAMESPACE(zuki)

#pragma warning(pop)
