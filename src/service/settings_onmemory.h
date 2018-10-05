#ifndef FACE_SMASH_SERVICE_SETTINGS_ONMEMORY_H
#define FACE_SMASH_SERVICE_SETTINGS_ONMEMORY_H


#include "settings_service.h"
#include <map>
#include <mutex>


namespace gamee {


struct SettingsOnMemory : SettingsService {
    void write(const std::string &, bool) override;
    void write(const std::string &, int64_t) override;
    void write(const std::string &, float) override;
    void write(const std::string &, std::string) override;
    void write(const std::string &, const char*) override;

    bool read(const std::string &, bool) const override;
    int64_t read(const std::string &, int64_t) const override;
    float read(const std::string &, float) const override;
    std::string read(const std::string &, std::string) const override;
    std::string read(const std::string &, const char *) const override;

private:
    std::map<std::string, std::string> m_;
    mutable std::mutex mtx_;
};


}


#endif // FACE_SMASH_SERVICE_SETTINGS_ONMEMORY_H
