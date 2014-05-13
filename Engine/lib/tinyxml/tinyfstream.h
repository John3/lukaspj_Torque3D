/*
 * Author: Lukas Joergensen
 */

class TiFileStream
{
public:
   enum AccessMode
   {
      Read         = 0,    ///< Open for read only.
      Write        = 1,    ///< Open for write only.
      ReadWrite    = 2,    ///< Open for read-write.
      WriteAppend  = 3     ///< Write-only, starting at end of file.
   };

   virtual bool writeStringBuffer(const char* buffer) = 0;
   virtual bool writeFormattedBuffer(const char* format, ...) = 0;
   virtual bool open(const char* &inFileName, AccessMode inMode) = 0;
   virtual void close() = 0;
   virtual unsigned int getStreamSize() = 0;
   virtual bool read(const unsigned int in_numBytes, char* out_pBuffer) = 0;
   virtual bool write(const unsigned int) = 0;

   virtual ~TiFileStream() = 0;
};