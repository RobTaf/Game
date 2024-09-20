#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <unordered_map>
#include <fstream>
#include <sstream>

#include "../utility/Utility.hpp"

template <typename Derived, typename T>
class ResourceManager {
public:
    ResourceManager(const std::string& pathsFile) { loadPaths(pathsFile); }
    virtual ~ResourceManager() { purgeResources(); }

    std::string getPath(const std::string& id) {
        auto path = m_paths.find(id);
        return path != m_paths.end() ? path->second : "";
    }

    bool requireResource(const std::string& id) {
        auto result = find(id);
        if (result) {
            ++result->second;
            return true;
        }
        auto path = m_paths.find(id);
        if (path == m_paths.end())
            return false;
        T* resource = load(path->second);
        if (not resource)
            return false;
        return m_resources.emplace(id, std::make_pair(resource, 1)).second;
    }

    bool releaseResource(const std::string& id) {
        auto result = find(id);
        if (not result)
            return false;
        --result->second;
        if (result->second == 0)
            unload(id);
        return true;
    }

    void purgeResources() {
        while (m_resources.begin() != m_resources.end()) {
            delete m_resources.begin()->second.first;
            m_resources.erase(m_resources.begin());
        }
    }

    bool unload(const std::string& id) {
        auto itr = m_resources.find(id);
        if (itr == m_resources.end())
            return false;
        delete itr->second.first;
        m_resources.erase(itr);
        return true;
    }

    T* load(const std::string& path) {
        // Derived classes will implement their own version of load()
        return static_cast<Derived*>(this)->load(path);
    }

    T* getResource(const std::string& id) {
        auto result = find(id);
        return result ? result->first : nullptr;
    }

private:
    void loadPaths(const std::string& pathFile) {
        std::ifstream paths;
        paths.open(Utility::workingDirectory() + pathFile);
        if (paths.is_open()) {
            std::string line;
            while (std::getline(paths, line)) {
                std::stringstream keystream(line);
                std::string pathName;
                std::string path;
                keystream >> pathName;
                keystream >> path;
                m_paths.emplace(pathName, path);
            }
            paths.close();
            return;
        }
        std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
    }

    std::pair<T*, unsigned int>* find(const std::string& id) {
        auto itr = m_resources.find(id);
        return itr != m_resources.end() ? &itr->second : nullptr;
    }

    std::unordered_map<std::string, std::pair<T*, unsigned int>> m_resources;
    std::unordered_map<std::string, std::string> m_paths;
};

#endif