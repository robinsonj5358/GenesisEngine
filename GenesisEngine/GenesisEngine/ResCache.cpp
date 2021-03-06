// ResCache.cpp : Defines a simple resource cache

#include <list>
#include <map>
#include <cctype>			// For std::tolower

#include "ResCache.h"
#include "EngineCode.h"
#include "String.h"

//============================================================================
// Resource::Resource
Resource::Resource(const std::string &name)
{
	m_name = name;
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
}

//============================================================================
// ResourceZipFile : Implements the IResourceFile interface with a ZipFile
ResourceZipFile::~ResourceZipFile()
{
	SAFE_DELETE(m_pZipFile);
}

bool ResourceZipFile::VOpen()
{
	m_pZipFile = GCC_NEW ZipFile;
	if (m_pZipFile)
	{
		return m_pZipFile->Init(m_resFileName.c_str());
	}
	return false;
}

int ResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == 1)
		return -1;

	return m_pZipFile->GetFileLen(resourceNum);
}

int ResourceZipFile::VGetRawResource(const Resource &r, char *buffer)
{
	int size = 0;
	optional<int> resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum.valid())
	{
		size = m_pZipFile->GetFileLen(*resourceNum);
		m_pZipFile->ReadFile(*resourceNum, buffer);
	}
	return size;
}

int ResourceZipFile::VGetNumResources() const
{
	return (m_pZipFile == NULL) ? 0 : m_pZipFile->GetNumFiles();
}

std::string ResourceZipFile::VGetResourceName(int num) const
{
	std::string resName = "";
	if (m_pZipFile != NULL && num >= 0 && num < m_pZipFile->GetNumFiles())
	{
		resName = m_pZipFile->GetFilename(num);
	}
	return resName;
}

//============================================================================
// DevelopmentResourceZipFile
DevelopmentResourceZipFile::DevelopmentResourceZipFile(const std::wstring resFileName, const Mode mode)
	: ResourceZipFile(resFileName)
{
	m_mode = mode;

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_AssetsDir = dir;
	int lastSlash = m_AssetsDir.find_last_of(L"\\");
	m_AssetsDir = m_AssetsDir.substr(0, lastSlash);
	m_AssetsDir += L"\\Assets\\";
}

int DevelopmentResourceZipFile::Find(const std::string &name)
{
	std::string lowerCase = name;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
	ZipContentsMap::const_iterator i = m_DirectoryContentsMap.find(lowerCase);
	if (i == m_DirectoryContentsMap.end())
		return -1;

	return i->second;
}

bool DevelopmentResourceZipFile::VOpen()
{
	if (m_mode != Editor)
	{
		ResourceZipFile::VOpen();
	}

	// open the asset directory and read in the non-hidden content
	if (m_mode == Editor)
	{
		ReadAssetsDirectory(L"*");
	}
	else
	{
		GCC_ASSERT(0 && "Not implemented yet");
	}

	return true;
}

int DevelopmentResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	int size = 0;

	if (m_mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		return m_AssetFileInfo[num].nFileSizeLow;
	}

	return ResourceZipFile::VGetRawResourceSize(r);
}

int DevelopmentResourceZipFile::VGetRawResource(const Resource &r, char *buffer)
{
	if (m_mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		std::string fullFileSpec = ws2s(m_AssetsDir) + r.m_name.c_str();
		FILE *f = fopen(fullFileSpec.c_str(), "rb");
		size_t bytes = fread(buffer, 1, m_AssetFileInfo[num].nFileSizeLow, f);
		fclose(f);
		return bytes;
	}

	return ResourceZipFile::VGetRawResource(r, buffer);
}

int DevelopmentResourceZipFile::VGetNumResources() const
{
	return (m_mode == Editor) ? m_AssetFileInfo.size() : ResourceZipFile::VGetNumResources();
}

std::string DevelopmentResourceZipFile::VGetResourceName(int num) const
{
	if (m_mode == Editor)
	{
		std::wstring wideName = m_AssetFileInfo[num].cFileName;
		return ws2s(wideName);
	}

	return ResourceZipFile::VGetResourceName(num);
}

void DevelopmentResourceZipFile::ReadAssetsDirectory(std::wstring fileSpec)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	// get first file
	std::wstring pathSpec = m_AssetsDir + fileSpec;
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		// loop on all remaining entries in dir
		while (FindNextFile(fileHandle, &findData))
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			std::wstring fileName = findData.cFileName;
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName + L"\\*";
					ReadAssetsDirectory(fileName);
				}
			}
			else
			{
				fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName;
				std::wstring lower = fileName;
				std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower);
				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				m_DirectoryContentsMap[ws2s(lower)] = m_AssetFileInfo.size();
				m_AssetFileInfo.push_back(findData);
			}
		}
	}

	FindClose(fileHandle);
}

//============================================================================
// ResHandle
ResHandle::ResHandle(Resource &resource, char *buffer, unsigned int size, ResCache *pResCache)
	: m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_extra = NULL;
	m_pResCache = pResCache;
}

ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_buffer);
	m_pResCache->MemoryHasBeenFreed(m_size);
}

//
//============================================================================


ResCache::ResCache(const unsigned int sizeInMb, IResourceFile *resFile)
{
	m_cacheSize = sizeInMb * 1024 * 1024;		// total memory size
	m_allocated = 0;							// total memory allocated
	m_file = resFile;
}

ResCache::~ResCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_file);
}

bool ResCache::Init()
{
	bool retValue = false;
	if (m_file->VOpen())
	{
		RegisterLoader(shared_ptr<IResourceLoader>(GCC_NEW DefaultResourceLoader()));
		retValue = true;
	}
	return retValue;
}

void ResCache::RegisterLoader(shared_ptr<IResourceLoader> loader)
{
	m_resourceLoaders.push_front(loader);
}

shared_ptr<ResHandle> ResCache::GetHandle(Resource *r)
{
	shared_ptr<ResHandle> handle(Find(r));
	if (handle == NULL)
	{
		handle = Load(r);
		GCC_ASSERT(handle);
	}
	else
	{
		Update(handle);
	}
	return handle;
}

shared_ptr<ResHandle> ResCache::Load(Resource *r)
{
	// Create a new resource and add it to the lru list and map

	shared_ptr<IResourceLoader> loader;
	shared_ptr<ResHandle> handle;

	for (ResourceLoaders::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> testLoader = *it;

		if (WildcardMatch(testLoader->VGetPattern().c_str(), r->m_name.c_str()))
		{
			loader = testLoader;
			break;
		}
	}

	if (!loader)
	{
		GCC_ASSERT(loader && _T("Default resource loader not found!"));
		return handle;			// Resource not loaded!
	}

	int rawSize = m_file->VGetRawResourceSize(*r);
	if (rawSize < 0)
	{
		GCC_ASSERT(rawSize > 0 && "Resource size returned -1 - Resource not found");
		return shared_ptr<ResHandle>();
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? (1) : (0));
	char *rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : GCC_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == NULL || m_file->VGetRawResource(*r, rawBuffer) == 0)
	{
		// resource cache out of memory
		return shared_ptr<ResHandle>();
	}

	char *buffer = NULL;
	unsigned int size = 0;

	if (loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r, buffer, rawSize, this));
	}
	else
	{
		size = loader->VGetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);
		if (rawBuffer == NULL || buffer == NULL)
		{
			// resource cach out of memory
			return shared_ptr<ResHandle>();
		}
		handle = shared_ptr<ResHandle>(GCC_NEW ResHandle(*r, buffer, size, this));
		bool success = loader->VLoadResource(rawBuffer, rawSize, handle);

		if (loader->VDiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			// resource cache out of memory
			return shared_ptr<ResHandle>();
		}
	}

	if (handle)
	{
		m_lru.push_front(handle);
		m_resources[r->m_name] = handle;
	}

	GCC_ASSERT(loader && _T("Default resource loader not found!"));
	return handle;
}

shared_ptr<ResHandle> ResCache::Find(Resource *r)
{
	ResHandleMap::iterator i = m_resources.find(r->m_name);
	if (i == m_resources.end())
		return shared_ptr<ResHandle>();

	return i->second;
}

void ResCache::Update(shared_ptr<ResHandle> handle)
{
	m_lru.remove(handle);
	m_lru.push_front(handle);
}

char *ResCache::Allocate(unsigned int size)
{
	if (!MakeRoom(size))
		return NULL;

	char *mem = GCC_NEW char[size];
	if (mem)
	{
		m_allocated += size;
	}

	return mem;
}

void ResCache::FreeOneResource()
{
	ResHandleList::iterator gonner = m_lru.end();
	gonner--;

	shared_ptr<ResHandle> handle = *gonner;

	m_lru.pop_back();
	m_resources.erase(handle->m_resource.m_name);
	// Note - you can't change the resource cache size yet - the resource bits could still actually be
	// used by some sybsystem holding onto the ResHandle. Only when it goes out of scope can the memory
	// be actually free again.
}

// ResCache::Flush - Frees every handle in the cache - this would be good to call if you are loading a new
//					 level, or if you wanted to force a refresh of all the data in the cache - which might be 
//					 good in a development environment.
void ResCache::Flush()
{
	while (!m_lru.empty())
	{
		shared_ptr<ResHandle> handle = *(m_lru.begin());
		Free(handle);
		m_lru.pop_front();
	}
}

bool ResCache::MakeRoom(unsigned int size)
{
	if (size > m_cacheSize)
	{
		return false;
	}

	// return null if there's no possible way to allocate the memory
	while (size > (m_cacheSize - m_allocated))
	{
		// The cache is empty, and there's still not enough room.
		if (m_lru.empty())
			return false;

		FreeOneResource();
	}

	return true;
}

// ResCache::Free
void ResCache::Free(shared_ptr<ResHandle> gonner)
{
	m_lru.remove(gonner);
	m_resources.erase(gonner->m_resource.m_name);
}

// ResCache::MemoryHasBeenFreed: This is called whenever the memory associated with 
//								 a resource is actually freed
void ResCache::MemoryHasBeenFreed(unsigned int size)
{
	m_allocated -= size;
}

// ResCache::Match: Seraches the resource cach assets for files mataching the pattern.
//					Useful for providing a list of levels for a main menu screen, for example.
std::vector<std::string> ResCache::Match(const std::string pattern)
{
	std::vector<std::string> matchingNames;
	if (m_file == NULL)
		return matchingNames;

	int numFiles = m_file->VGetNumResource();
	for (int i = 0; i < numFiles; ++i)
	{
		std::string name = m_file->VGetResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if (WildcardMatch(pattern.c_str(), name.c_str()))
		{
			matchingNames.push_back(name);
		}
	}
	return matchingNames;
}

int ResCache::Preload(const std::string pattern, void(*progressCallback)(int, bool &))
{
	if (m_file == NULL)
		return 0;

	int numFiles = m_file->VGetNumResource();
	int loaded = 0;
	bool cancel = false;
	for (int i = 0; i < numFiles; ++i)
	{
		Resource resource(m_file->VGetResourceName(i));

		if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
		{
			shared_ptr<ResHandle> handle = g_pApp->m_ResCache->GetHandle(&resource);
			++loaded;
		}

		if (progressCallback != NULL)
		{
			progressCallback(i * 100 / numFiles, cancel);
		}
	}
	return loaded;
}