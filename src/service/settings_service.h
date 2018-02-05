#ifndef FACE_SMASH_SERVICE_SETTINGS_SERVICE_H
#define FACE_SMASH_SERVICE_SETTINGS_SERVICE_H


#include "settings_service.h"
#include <cstdint>
#include <string>


namespace gamee {


struct SettingsService {
    virtual ~SettingsService() = default;

    virtual void write(const std::string&, bool) = 0;
    virtual void write(const std::string&, int64_t) = 0;
    virtual void write(const std::string&, float) = 0;
    virtual void write(const std::string&, std::string) = 0;
    virtual void write(const std::string&, const char*) = 0;

    virtual bool read(const std::string&, bool fallback) const = 0;
    virtual int64_t read(const std::string&, int64_t fallback) const = 0;
    virtual float read(const std::string&, float fallback) const = 0;
    virtual std::string read(const std::string&, std::string fallback) const = 0;
    virtual std::string read(const std::string&, const char* fallback) const = 0;
};


}


#endif // FACE_SMASH_SERVICE_SETTINGS_SERVICE_H
