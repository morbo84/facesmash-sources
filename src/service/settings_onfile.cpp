#include "settings_onfile.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#ifdef __ANDROID__
#include <SDL.h>
#endif


namespace gamee {


#ifdef __ANDROID__
static const std::string filePath{std::string{SDL_AndroidGetInternalStoragePath()} + "/settings"};
#else
static const std::string filePath{"./settings"};
#endif


SettingsOnFile::SettingsOnFile() {
    std::ifstream infile{filePath};
    std::string key;

    while(infile >> key) {
        std::string value;
        infile >> value;
        m_[key] = value;
    }
}


void SettingsOnFile::write(const std::string &key, bool value) {
    m_[key] = std::to_string(value);
    save();
}


void SettingsOnFile::write(const std::string &key, int64_t value) {
    m_[key] = std::to_string(value);
    save();
}


void SettingsOnFile::write(const std::string &key, float value) {
    m_[key] = std::to_string(value);
    save();
}


void SettingsOnFile::write(const std::string &key, std::string value) {
    m_[key] = std::move(value);
    save();
}


void SettingsOnFile::write(const std::string &key, const char *value) {
    SettingsOnFile::write(key, std::string{value});
}


bool SettingsOnFile::read(const std::string &key, bool fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stoi(found->second);
}


int64_t SettingsOnFile::read(const std::string &key, int64_t fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stoll(found->second);
}


float SettingsOnFile::read(const std::string &key, float fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stof(found->second);
}


std::string SettingsOnFile::read(const std::string &key, std::string fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : found->second;
}


std::string SettingsOnFile::read(const std::string &key, const char *fallback) const {
    return SettingsOnFile::read(key, std::string{fallback});
}


void SettingsOnFile::save() {
    std::ofstream outfile{filePath};

    for(auto& p : m_) {
        outfile << p.first << "\t\t" << p.second << '\n';
    }
}


}
