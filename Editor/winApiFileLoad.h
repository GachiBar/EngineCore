#pragma once

#include <vector>


#include <commdlg.h>
#include <shlobj.h>
#include <string>



std::vector<std::wstring> split(std::wstring s, std::wstring delimiter);


#define TEXT(s) L ## s

std::vector<std::wstring> LoadFileFromExplorer(
    const std::wstring& DefaultPath,
    const std::wstring& FileTypes);

std::vector<std::wstring> SaveFileToExplorer(
    const std::wstring& DefaultPath,
    const std::wstring& DefaultName,
    const std::wstring& FileTypes);