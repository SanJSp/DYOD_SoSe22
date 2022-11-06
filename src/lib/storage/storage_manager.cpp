#include "storage_manager.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "utils/assert.hpp"

namespace opossum {

StorageManager& StorageManager::get() {
  static StorageManager instance;
  return instance;
}

void StorageManager::add_table(const std::string& name, std::shared_ptr<Table> table) {
  tableNameMap[name] = table;
}

void StorageManager::drop_table(const std::string& name) {
  DebugAssert(tableNameMap.contains(name), "Table does not exist");
  tableNameMap.erase(name);
}

std::shared_ptr<Table> StorageManager::get_table(const std::string& name) const {
  return tableNameMap.at(name);
}

bool StorageManager::has_table(const std::string& name) const {
  return tableNameMap.contains(name);
}

std::vector<std::string> StorageManager::table_names() const {
  // Implementation goes here
  Fail("Implementation is missing.");
}

void StorageManager::print(std::ostream& out) const {
  // Implementation goes here
  Fail("Implementation is missing.");
}

void StorageManager::reset() {
  tableNameMap.clear();
}

}  // namespace opossum
