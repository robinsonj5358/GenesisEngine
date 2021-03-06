#pragma once

class ResHandle;
class ResCache;

#include "ZipFile.h"		// Needed for ZipContentsMap
#include "interfaces.h"		// Needed for Resource Interfaces

// IResourceExtraData
class IResourceExtraData
{
public:
	virtual std::string VToString() = 0;
};

// Resource
class Resource
{
public:
	std::string m_name;
	Resource(const std::string &name);
};

// ResourceZipFile: Implements the IResourceFile interface with a ZipFile.
class ResourceZipFile : public IResourceFile
{
	ZipFile *m_pZipFile;
	std::wstring m_resFileName;

public:
	ResourceZipFile(const std::wstring resFileName) { m_pZipFile = NULL; m_resFileName = resFileName; }
	virtual ~ResourceZipFile();

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource &r);
	virtual int VGetRawResource(const Resource &r, char *buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return false; }
};

// DevelopmentResourceZipFile: This class fakes a ZIP file from a normal directory, and is used in the
//							   editor
class DevelopmentResourceZipFile : public ResourceZipFile
{
public:
	enum Mode
	{
		Development,	// this mode checks the original asset directory for changes - helps during development.
		Editor			// this mode only checks the original asset directory - the ZIP file is left unopened.
	};

	Mode m_mode;
	std::wstring m_AssetsDir;
	std::vector<WIN32_FIND_DATA> m_AssetFileInfo;
	ZipContentsMap m_DirectoryContentsMap;

	DevelopmentResourceZipFile(const std::wstring resFileName, const Mode mode);

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource &r);
	virtual int VGetRawResource(const Resource &r, char *buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return true; }

	int Find(const std::string &path);

protected:
	void ReadAssetsDirectory(std::wstring fileSpec);
};

// ResHandle
class ResHandle
{
	friend class ResCache;

protected:
	Resource m_resource;
	char *m_buffer;
	unsigned int m_size;
	shared_ptr<IResourceExtraData> m_extra;
	ResCache *m_pResCache;

public:
	ResHandle(Resource &resource, char *buffer, unsigned int size, ResCache *pResCache);

	virtual ~ResHandle();

	const std::string GetName() { return m_resource.m_name; }
	unsigned int Size() const { return m_size; }
	char *Buffer() const { return m_buffer; }
	char *WritableBuffer() { return m_buffer; }

	shared_ptr<IResourceExtraData> GetExtra() { return m_extra; }
	void SetExtra(shared_ptr<IResourceExtraData> extra) { m_extra = extra; }
};

// DefaultResourceLoader
class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile(){ return true; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) { return true; }
	virtual std::string VGetPattern() { return "*"; }
};

// ResCache
typedef std::list<shared_ptr<ResHandle>> ResHandleList;					// lru list
typedef std::map<std::string, shared_ptr<ResHandle>> ResHandleMap;		// maps identifiers to resource data
typedef std::list<shared_ptr<IResourceLoader>> ResourceLoaders;

class ResCache
{
	friend class ResHandle;

	ResHandleList m_lru;				// lru list
	ResHandleMap m_resources;
	ResourceLoaders m_resourceLoaders;

	IResourceFile *m_file;

	unsigned int m_cacheSize;			// total memory size
	unsigned int m_allocated;			// total memory allocated

protected:
	bool MakeRoom(unsigned int size);
	char *Allocate(unsigned int size);
	void Free(shared_ptr<ResHandle> gonner);

	shared_ptr<ResHandle> Load(Resource *r);
	shared_ptr<ResHandle> Find(Resource *r);
	void Update(shared_ptr<ResHandle> handle);

	void FreeOneResource();
	void MemoryHasBeenFreed(unsigned int size);

public:
	ResCache(const unsigned int sizeInMb, IResourceFile *file);
	virtual ~ResCache();

	bool Init();

	void RegisterLoader(shared_ptr<IResourceLoader> loader);

	shared_ptr<ResHandle> GetHandle(Resource *r);

	int Preload(const std::string pattern, void(*progressCallback)(int, bool &));
	std::vector<std::string> Match(const std::string pattern);

	void Flush(void);

	bool IsUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_file); return m_file->VIsUsingDevelopmentDirectories(); }
};