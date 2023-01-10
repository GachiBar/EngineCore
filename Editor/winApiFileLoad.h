#pragma once

#include <vector>


#include <commdlg.h>
#include <shlobj.h>
#include <Winver.h>


template<typename T>
class TComPtr
{
public:

	typedef T PointerType;

public:

	/** Default constructor (initialized to null). */
	TComPtr()
		:	RawPointer(NULL)
	{ }

	/**
	 * Create and initialize a new instance.
	 *
	 * @param Object The object to point to.
	 */
	TComPtr(PointerType* const Object)
		: RawPointer(Object)
	{
		if (RawPointer)
		{
			RawPointer->AddRef();
		}
	}

	/**
	 * Copy constructor.
	 *
	 * @param Other The instance to copy.
	 */
	TComPtr(const TComPtr<PointerType>& Other)
		: RawPointer(Other.RawPointer)
	{
		if (RawPointer)
		{
			RawPointer->AddRef();
		}
	}

	/**
	 * Move constructor.
	 *
	 * @param Other The instance to move.
	 */
	TComPtr(TComPtr<PointerType>&& Other)
		: RawPointer(Other.RawPointer)
	{	
		Other.RawPointer = NULL;
	}	

	/**
	 * Assignment operator.
	 *
	 * @param Object The object to point to.
	 */
	TComPtr<PointerType>& operator=(PointerType* const Object) 
	{
		if (RawPointer != Object)
		{
			if (Object)
			{
				Object->AddRef();
			}

			if (RawPointer)
			{
				RawPointer->Release();
			}

			RawPointer = Object;
		}

		return *this;
	}

	/**
	 * Copy assignment operator.
	 *
	 * @param Other The instance to copy.
	 */
	TComPtr<PointerType>& operator=(const TComPtr<PointerType>& Other) 
	{
		if (RawPointer != Other.RawPointer)
		{
			if (Other.RawPointer)
			{
				Other->AddRef();
			}

			if (RawPointer)
			{
				RawPointer->Release();
			}

			RawPointer = Other.RawPointer;
		}

		return *this;
	}

	/**
	 * Move assignment operator.
	 *
	 * @param Other The instance to move.
	 */
	TComPtr<PointerType>& operator=(TComPtr<PointerType>&& Other) 
	{			
		if (RawPointer != Other.RawPointer)
		{
			if (RawPointer)
			{
				RawPointer->Release();
			}

			RawPointer = Other.RawPointer;
			Other.RawPointer = NULL;
		}

		return *this;
	}

	/** Destructor. */
	~TComPtr() 
	{
		if (RawPointer)
		{
			RawPointer->Release();
		}
	}

public:

	FORCEINLINE PointerType** operator&()
	{
		return &(RawPointer);
	}

	FORCEINLINE PointerType* operator->() const 
	{
		assert(RawPointer != NULL);
		return RawPointer;
	}

	FORCEINLINE bool operator==(PointerType* const Object) const
	{
		return RawPointer == Object;
	}

	FORCEINLINE bool operator!=(PointerType* const Object) const
	{
		return RawPointer != Object;
	}

	FORCEINLINE operator PointerType*() const
	{
		return RawPointer;
	}

public:

	/**
	 * Set the pointer without adding a reference.
	 *
	 * @param InRawPointer The object to point to.
	 * @see Detach
	 */
	void Attach(PointerType* Object)
	{
		if (RawPointer)
		{
			RawPointer->Release();
		}

		RawPointer = Object;
	}

	/**
	 * Reset the pointer without releasing a reference.
	 *
	 * @see Attach
	 */
	void Detach()
	{
		RawPointer = NULL;
	}

	/**
	 * Initialize this pointer from a COM interface to be queried.
	 *
	 * @param Riid The ID of the interface to be queried.
	 * @param Unknown The object to query the interface from.
	 * @return The result code of the query.
	 */
	HRESULT FromQueryInterface(REFIID Riid, IUnknown* Unknown)
	{
		if (RawPointer)
		{
			RawPointer->Release();
			RawPointer = NULL;
		}

		return Unknown->QueryInterface(Riid, reinterpret_cast<void**>(&(RawPointer)));
	}

	/**
	 * Get raw pointer to the object pointed to.
	 *
	 * @return Pointer to the object, or NULL if not valid.
	 * @see IsValid
	 */
	FORCEINLINE PointerType* Get() const
	{
		return RawPointer;
	}

	/**
	 * Whether this pointer is pointing to an actual object.
	 *
	 * @return true if the pointer is valid, false otherwise.
	 * @see Get
	 */
	FORCEINLINE const bool IsValid() const
	{
		return (RawPointer != NULL);
	}

	/** Reset this pointer to null. */
	void Reset()
	{
		if (RawPointer)
		{
			RawPointer->Release();
			RawPointer = NULL;
		}
	}

private:

	/** Pointer to the actual object, if any. */
	PointerType* RawPointer;
};

std::vector<std::wstring> split (std::wstring s, std::wstring delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::wstring token;
	std::vector<std::wstring> res;

	while ((pos_end = s.find (delimiter, pos_start)) != std::wstring::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
};


#define TEXT(s) L ## s

std::vector<std::wstring> LoadFileFromExplorer(
	const std::wstring& DefaultPath,
	const std::wstring& FileTypes = L"All Files (*.geojson;*.json)|*.geojson;*.json|JSON Files (*.json)|*.json|GeoJSON Files (*.geojson)|*.geojson")
{
    std::vector<std::wstring> OutFilenames;


    IFileDialog* FileDialog;
    if (SUCCEEDED(::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, IID_PPV_ARGS_Helper(&FileDialog))))
    {
        // Set up common settings
        FileDialog->SetTitle(TEXT("Choose A File"));
        if (!DefaultPath.empty())
        {
            // SHCreateItemFromParsingName requires the given path be absolute and use \ rather than / as our normalized paths do
            std::wstring DefaultWindowsPath = DefaultPath;
        	std::replace(DefaultWindowsPath.begin(), DefaultWindowsPath.end(),TEXT('/'), TEXT('\\'));

            TComPtr<IShellItem> DefaultPathItem;
            if (SUCCEEDED(::SHCreateItemFromParsingName(DefaultWindowsPath.c_str(), nullptr, IID_PPV_ARGS(&DefaultPathItem))))
            {
                FileDialog->SetFolder(DefaultPathItem);
            }
        }

        // Set-up the file type filters
        std::vector<std::wstring> UnformattedExtensions;
        std::vector<COMDLG_FILTERSPEC> FileDialogFilters;
        {
            const std::wstring DefaultFileTypes = FileTypes;
            UnformattedExtensions = split(DefaultFileTypes, TEXT("|"));

            if (UnformattedExtensions.size() % 2 == 0)
            {
                FileDialogFilters.reserve(UnformattedExtensions.size() / 2);
                for (int32 ExtensionIndex = 0; ExtensionIndex < UnformattedExtensions.size();)
                {
                	FileDialogFilters.push_back({});
                    COMDLG_FILTERSPEC& NewFilterSpec = FileDialogFilters[FileDialogFilters.size()-1];
                    NewFilterSpec.pszName = UnformattedExtensions[ExtensionIndex++].c_str();
                    NewFilterSpec.pszSpec = UnformattedExtensions[ExtensionIndex++].c_str();
                }
            }
        }
        FileDialog->SetFileTypes(FileDialogFilters.size(), FileDialogFilters.data());

        // Show the picker
        if (SUCCEEDED(FileDialog->Show(NULL)))
        {
            int32 OutFilterIndex = 0;
            if (SUCCEEDED(FileDialog->GetFileTypeIndex((UINT*)&OutFilterIndex)))
            {
                OutFilterIndex -= 1; // GetFileTypeIndex returns a 1-based index
            }

            auto AddOutFilename = [&OutFilenames](const std::wstring& InFilename)
            {
            	OutFilenames.push_back(InFilename);
                //std::wstring& OutFilename = OutFilenames[OutFilenames.size()-1];
                //OutFilename = IFileManager::Get().ConvertToRelativePath(*OutFilename);
                //FPaths::NormalizeFilename(OutFilename);
            };

            {
                IFileOpenDialog* FileOpenDialog = static_cast<IFileOpenDialog*>(FileDialog);

                TComPtr<IShellItemArray> Results;
                if (SUCCEEDED(FileOpenDialog->GetResults(&Results)))
                {
                    DWORD sizeResults = 0;
                    Results->GetCount(&sizeResults);
                    for (DWORD ResultIndex = 0; ResultIndex < sizeResults; ++ResultIndex)
                    {
                        TComPtr<IShellItem> Result;
                        if (SUCCEEDED(Results->GetItemAt(ResultIndex, &Result)))
                        {
                            PWSTR pFilePath = nullptr;
                            if (SUCCEEDED(Result->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath)))
                            {
                                AddOutFilename(pFilePath);
                                ::CoTaskMemFree(pFilePath);
                            }
                        }
                    }
                }
            }
        }
    }
    FileDialog->Release();

    return OutFilenames;
};
