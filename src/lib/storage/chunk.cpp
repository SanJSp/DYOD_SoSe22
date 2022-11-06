#include <iomanip>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "abstract_segment.hpp"
#include "chunk.hpp"

#include "utils/assert.hpp"

namespace opossum {

void Chunk::add_segment(const std::shared_ptr<AbstractSegment> segment) {
  _contents.push_back(segment);
}

void Chunk::append(const std::vector<AllTypeVariant>& values) {
  // Implementation goes here
  DebugAssert(values.size() == _contents.size(), "Not same size of segments");

  int index = 0;
  for(auto item: values){
    _contents[index]->append(item);
    index++;
  }
}

std::shared_ptr<AbstractSegment> Chunk::get_segment(const ColumnID column_id) const {
  // Implementation goes here
  DebugAssert(column_id < _contents.size(), "ColumnID greate than Column count");
  return _contents[column_id];
}

ColumnCount Chunk::column_count() const {
  // Implementation goes here
  auto temp = _contents.size();
  return static_cast<ColumnCount>(temp);
}

ChunkOffset Chunk::size() const {
  // Implementation goes here
   if(_contents.size() == 0){
     return 0;
   }
  return _contents[0]->size();
}

size_t Chunk::estimate_memory_usage() const {
  return 0;
}

}  // namespace opossum
