//
// Created by sol on 6/24/2025.
//

#include "PWEParser.h"

#include <iostream>

void PWEParser::pack(std::string folder, std::string outputfile) {
    // Check if the folder exists
    if (!std::filesystem::exists(folder)) {
        throw std::runtime_error("Specified folder does not exist");
    }

    // Create zip archive
    zip_error_t error;
    zip_error_init(&error);
    int err;
    zip_source_t* src = zip_source_file_create(outputfile.c_str(), 0, -1, &error);
    if (src == nullptr) {
        throw std::runtime_error("Failed to create zip source");
    }

    zip_t* zip = zip_open(outputfile.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
    if (zip == nullptr) {
        zip_source_free(src);
        throw std::runtime_error("Failed to create zip archive");
    }
    // Add files to the archive
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
            std::string entryPath = entry.path().string();
            std::string relativePath = std::filesystem::relative(entry.path(), folder).string();

            if (entry.is_directory()) {
                // Add directory
                relativePath += "/";
                zip_dir_add(zip, relativePath.c_str(), ZIP_FL_ENC_UTF_8);
            } else if (entry.is_regular_file()) {
                // Add file
                zip_source_t* file_src = zip_source_file(zip, entryPath.c_str(), 0, 0);
                if (file_src == nullptr) {
                    throw std::runtime_error("Failed to create source for file: " + entryPath);
                }

                if (zip_file_add(zip, relativePath.c_str(), file_src,
                    ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE) < 0) {
                    zip_source_free(file_src);
                    throw std::runtime_error("Failed to add file to archive: " + entryPath);
                    }
            }
        }
    } catch (const std::exception& e) {
        zip_close(zip);
        throw;
    }
    if (zip_close(zip) < 0) {
        throw std::runtime_error("Failed to close zip archive");
    }
}


void PWEParser::unpack(std::string pwefile, std::string outputfolder) {
    int err = 0;
    zip_t *zip = zip_open(pwefile.c_str(), 0, &err);
    if (!zip) {
        throw std::runtime_error("Failed to open PWE file");
    }

    // First, extract info.yaml to get password information
    zip_stat_t sb;
    if (zip_stat(zip, "info.yaml", 0, &sb) < 0) {
        zip_close(zip);
        throw std::runtime_error("Failed to find info.yaml in PWE file");
    }

    // Read info.yaml content from zip
    zip_file_t *info_file = zip_fopen(zip, "info.yaml", 0);
    if (!info_file) {
        zip_close(zip);
        throw std::runtime_error("Failed to open info.yaml from PWE file");
    }

    std::vector<char> buffer(sb.size + 1);
    zip_fread(info_file, buffer.data(), sb.size);
    buffer[sb.size] = '\0';
    zip_fclose(info_file);

    // Create output directory if it doesn't exist
    std::filesystem::create_directories(outputfolder);

    // Get number of entries in zip
    zip_int64_t num_entries = zip_get_num_entries(zip, 0);
    for (zip_int64_t i = 0; i < num_entries; i++) {
        zip_stat_index(zip, i, 0, &sb);
        std::string name = sb.name;
        std::string fullPath = outputfolder + "/" + name;

        // Create directories if needed
        std::filesystem::path dirPath = std::filesystem::path(fullPath).parent_path();
        std::filesystem::create_directories(dirPath);

        // Skip if it's a directory
        if (name.back() == '/') {
            continue;
        }

        // Open file in zip
        zip_file_t *zf = zip_fopen_index(zip, i, 0);
        if (!zf) {
            zip_close(zip);
            throw std::runtime_error("Failed to open file in PWE: " + name);
        }

        // Create output file
        std::ofstream outFile(fullPath, std::ios::binary);
        if (!outFile) {
            zip_fclose(zf);
            zip_close(zip);
            throw std::runtime_error("Failed to create output file: " + fullPath);
        }

        // Read and write in chunks
        std::vector<char> fileBuffer(8192);
        zip_uint64_t total_read = 0;
        while (total_read < sb.size) {
            zip_int64_t bytes_read = zip_fread(zf, fileBuffer.data(),
                std::min(fileBuffer.size(), sb.size - total_read));
            if (bytes_read < 0) {
                zip_fclose(zf);
                zip_close(zip);
                throw std::runtime_error("Failed to read file from PWE: " + name);
            }
            outFile.write(fileBuffer.data(), bytes_read);
            total_read += bytes_read;
        }

        zip_fclose(zf);
    }

    zip_close(zip);
}

