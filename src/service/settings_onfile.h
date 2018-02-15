#ifndef FACE_SMASH_SERVICE_SETTINGS_ONFILE_H
#define FACE_SMASH_SERVICE_SETTINGS_ONFILE_H


#include "settings_service.h"
#include <map>
#include <string>


namespace gamee {


struct SettingsOnFile : SettingsService {
    SettingsOnFile();

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
    void save();

    std::map<std::string, std::string> m_;
};


}


#endif // FACE_SMASH_SERVICE_SETTINGS_ONFILE_H
