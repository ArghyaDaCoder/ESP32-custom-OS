#pragma once
#include <FS.h>   // fs::FS, fs::File — from Arduino ESP32 core

class IFileSystem {
public:
    virtual ~IFileSystem() = default;

    // One-time mount. Returns false if mount fails.
    virtual bool mount() = 0;

    // Unmount safely (important before SD card removal).
    virtual void unmount() = 0;

    // Human-readable name for Serial debug output.
    virtual const char* name() = 0;

    // The only real method: hand out the underlying fs::FS.
    // All file operations go through this — open, read,
    // write, seek, directory listing, everything.
    // Callers use fs::File handles directly (streaming, not bulk).
    virtual fs::FS& fs() = 0;

    // Convenience helpers built ON TOP of fs() — not virtual,
    // not overridable, same for every backend.
    bool exists(const char* path)  { return fs().exists(path);  }
    bool remove(const char* path)  { return fs().remove(path);  }
    bool mkdir(const char* path)   { return fs().mkdir(path);   }
};