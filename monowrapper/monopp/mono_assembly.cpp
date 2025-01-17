#include "mono_assembly.h"
#include "mono_domain.h"
#include "mono_exception.h"

#include "mono_string.h"
#include "mono_type.h"
#include <sstream>

BEGIN_MONO_INCLUDE
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
END_MONO_INCLUDE

namespace mono
{
    mono_assembly::mono_assembly(const mono_domain& domain, const std::string& path)
    {
        assembly_ = mono_domain_assembly_open(domain.get_internal_ptr(), path.c_str());

        if (!assembly_)
            throw mono_exception("NATIVE::Could not open assembly with path : " + path);

        image_ = mono_assembly_get_image(assembly_);
    }

    auto mono_assembly::get_type(const std::string& name) const -> mono_type
    {
        return mono_type(image_, name);
    }

    auto mono_assembly::get_type(const std::string& name_space, const std::string& name) const -> mono_type
    {
        return mono_type(image_, name_space, name);
    }

    auto mono_assembly::dump_references() const -> std::vector<std::string>
    {
        std::vector<std::string> refs;
        /* Get a pointer to the AssemblyRef metadata table */
        auto table_info = mono_image_get_table_info(image_, MONO_TABLE_ASSEMBLYREF);

        /* Fetch the number of rows available in the table */
        int rows = mono_table_info_get_rows(table_info);

        refs.reserve(size_t(rows));
        /* For each row, print some of its values */
        for (int i = 0; i < rows; i++)
        {
            /* Space where we extract one row from the metadata table */
            uint32_t cols[MONO_ASSEMBLYREF_SIZE];

            /* Extract the row into the array cols */
            mono_metadata_decode_row(table_info, i, cols, MONO_ASSEMBLYREF_SIZE);

            std::stringstream s;
            s << i + 1;
            s << " Version=";
            s << cols[MONO_ASSEMBLYREF_MAJOR_VERSION];
            s << ".";
            s << cols[MONO_ASSEMBLYREF_MINOR_VERSION];
            s << ".";
            s << cols[MONO_ASSEMBLYREF_BUILD_NUMBER];
            s << ".";
            s << cols[MONO_ASSEMBLYREF_REV_NUMBER];
            s << "\n\tName=";
            s << mono_metadata_string_heap(image_, cols[MONO_ASSEMBLYREF_NAME]);

            refs.emplace_back(s.str());
        }

        return refs;
    }

    auto mono_assembly::dump_type_names() const -> std::vector<std::string>
    {
        std::vector<std::string> names;
        /* Get a pointer to the AssemblyRef metadata table */
        const MonoTableInfo* table_info = mono_image_get_table_info(image_, MONO_TABLE_TYPEDEF);

        /* Fetch the number of rows available in the table */
        const int rows = mono_table_info_get_rows(table_info);

        for (int i = 0; i < rows; i++)
        {
            // Get current row with MONO_TYPEDEF_SIZE columns number
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);

            // Decode data
            const char* nameSpace = mono_metadata_string_heap(image_, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image_, cols[MONO_TYPEDEF_NAME]);

            std::stringstream s;
            s << nameSpace << "." << name;

            names.emplace_back(s.str());
        }

        return names;
    }
    
} // namespace mono
