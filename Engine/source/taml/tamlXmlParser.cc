//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
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

#include "taml/tamlXmlParser.h"

// Debug Profiling.
#include "platform/profiler.h"

//-----------------------------------------------------------------------------

bool TamlXmlParser::parse( const char* pFilename, TamlXmlVisitor& visitor, const bool writeDocument )
{
    // Debug Profiling.
    PROFILE_SCOPE(TamlXmlParser_Parse);

    // Sanity!
    AssertFatal( pFilename != NULL, "Cannot parse a NULL filename." );

    char filenameBuffer[1024];
    // TODO: Make sure this is a proper substitute for
    // Con::expandPath (T2D)
    Con::expandToolScriptFilename( filenameBuffer, sizeof(filenameBuffer), pFilename );
    /** T2D uses a custom version of TinyXML that supports FileStream.
      * We don't so we can't do this
      *
    FileStream stream;

    // File open for read?
    if ( !stream.open( filenameBuffer, Torque::FS::File::AccessMode::Read ) )
    {
        // No, so warn.
        Con::warnf("TamlXmlParser::parse() - Could not open filename '%s' for parse.", filenameBuffer );
        return false;
    }
    
     */

    TiXmlDocument xmlDocument;

    // Load document from stream.
    if ( !xmlDocument.LoadFile( filenameBuffer ) )
    {
        // Warn!
        Con::warnf("TamlXmlParser: Could not load Taml XML file from stream.");
        return false;
    }

    // Close the stream.
    // stream.close();

    // Set parsing filename.
    mpParsingFilename = filenameBuffer;

    // Parse root element.
    parseElement( xmlDocument.RootElement(), visitor );

    // Reset parsing filename.
    mpParsingFilename = NULL;

    // Are we writing the document?
    if ( writeDocument )
    {
       /**
         *
        // Yes, so open for write?
       if ( !stream.open( filenameBuffer, Torque::FS::File::Write ) )
        {
            // No, so warn.
            Con::warnf("TamlXmlParser::parse() - Could not open filename '%s' for write.", filenameBuffer );
            return false;
        }
        */

        // Yes, so save the document.
       if ( !xmlDocument.SaveFile( filenameBuffer ) )
        {
            // Warn!
            Con::warnf("TamlXmlParser: Could not save Taml XML document.");
            return false;
        }

        // Close the stream.
        // stream.close();
    }

    return true;
}

//-----------------------------------------------------------------------------

bool TamlXmlParser::parseElement( TiXmlElement* pXmlElement, TamlXmlVisitor& visitor )
{
    // Debug Profiling.
    PROFILE_SCOPE(TamlXmlParser_ParseElement);

    // Visit this element (stop processing if instructed).
    if ( !visitor.visit( pXmlElement, *this ) )
        return false;

    // Parse attributes (stop processing if instructed).
    if ( !parseAttributes( pXmlElement, visitor ) )
        return false;

    // Fetch any children.
    TiXmlNode* pChildXmlNode = pXmlElement->FirstChild();

    // Do we have any element children?
    if ( pChildXmlNode != NULL && pChildXmlNode->Type() == TiXmlNode::TINYXML_ELEMENT )
    {
        // Iterate children.
        for ( TiXmlElement* pChildXmlElement = dynamic_cast<TiXmlElement*>( pChildXmlNode ); pChildXmlElement; pChildXmlElement = pChildXmlElement->NextSiblingElement() )
        {
            // Parse element (stop processing if instructed).
            if ( !parseElement( pChildXmlElement, visitor ) )
                return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------

bool TamlXmlParser::parseAttributes( TiXmlElement* pXmlElement, TamlXmlVisitor& visitor )
{
    // Debug Profiling.
    PROFILE_SCOPE(TamlXmlParser_ParseAttribute);

    // Iterate attributes.
    for ( TiXmlAttribute* pAttribute = pXmlElement->FirstAttribute(); pAttribute; pAttribute = pAttribute->Next() )
    {
        // Visit this attribute (stop processing if instructed).
        if ( !visitor.visit( pAttribute, *this ) )
            return false;
    }

    return true;
}
