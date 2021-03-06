//---------------------------------------------------------------------------
// WebServiceCertificateCollection Declarations
//
// Zucchini Service Configuration Library
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

#ifndef __WEBSERVICECERTIFICATECOLLECTION_H_
#define __WEBSERVICECERTIFICATECOLLECTION_H_
#pragma once

#pragma warning(push, 4)					// Enable maximum compiler warnings

using namespace System;

BEGIN_NAMESPACE(zuki)
BEGIN_NAMESPACE(web)
BEGIN_NAMESPACE(config)

//---------------------------------------------------------------------------
// Class WebServiceCertificateCollection
//
// WebServiceCertificateCollection implements a collection that is used to
// manage the HTTP.SYS SSL certificates on this system
//---------------------------------------------------------------------------

public ref class WebServiceCertificateCollection sealed
{
public:

	//-----------------------------------------------------------------------
	// Properties

internal:

	// INTERNAL CONSTRUCTOR
	WebServiceCertificateCollection() {}

private:

	//-----------------------------------------------------------------------
	// Member Variables
};

//---------------------------------------------------------------------------

END_NAMESPACE(config)
END_NAMESPACE(web)
END_NAMESPACE(zuki)

#pragma warning(pop)

#endif	// __WEBSERVICECERTIFICATECOLLECTION_H_