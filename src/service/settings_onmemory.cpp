#include "settings_onmemory.h"


namespace gamee {


void SettingsOnMemory::write(const std::string &key, bool value) {
    m_[key] = std::to_string(value);
}


void SettingsOnMemory::write(const std::string &key, int64_t value) {
    m_[key] = std::to_string(value);
}


void SettingsOnMemory::write(const std::string &key, float value) {
    m_[key] = std::to_string(value);
}


void SettingsOnMemory::write(const std::string &key, std::string value) {
    m_[key] = std::move(value);
}


void SettingsOnMemory::write(const std::string &key, const char *value) {
    m_[key] = std::string{value};
}


bool SettingsOnMemory::read(const std::string &key, bool fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stoi(found->second);
}


int64_t SettingsOnMemory::read(const std::string &key, int64_t fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stoll(found->second);
}


float SettingsOnMemory::read(const std::string &key, float fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : std::stof(found->second);
}


std::string SettingsOnMemory::read(const std::string &key, std::string fallback) const {
    auto found = m_.find(key);
    return found == m_.end() ? fallback : found->second;
}

std::string SettingsOnMemory::read(const std::string &key, const char *fallback) const {
    return SettingsOnMemory::read(key, std::string{fallback});
}


}
