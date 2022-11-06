#include "table.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "value_segment.hpp"

#include "resolve_type.hpp"
#include "types.hpp"
#include "utils/assert.hpp"

namespace opossum {

Table::Table(const ChunkOffset target_chunk_size) {
  my_target_chunk_size = target_chunk_size;
  create_new_chunk();
}

void Table::add_column_definition(const std::string& name, const std::string& type) {
  column_name_index[name] = _contents.size();
  column_definition_by_index.emplace_back(name,type);
}

void Table::add_column(const std::string& name, const std::string& type) {
  add_column_definition(name, type);
  for(auto chunk : _contents){
    resolve_data_type(type, [&](const auto data_type_t) {
      using SegmentDataType = typename decltype(data_type_t)::type;
      const auto value_segment =
          std::make_shared<ValueSegment<SegmentDataType>>();
      chunk->add_segment(value_segment);
    });
  }
}

void Table::create_new_chunk() {
  auto chunk = std::make_shared<Chunk>();
  if(!_contents.empty()){
    for(auto [ name,  type ] : column_definition_by_index){
      resolve_data_type(type, [&](const auto data_type_t) {
        using SegmentDataType = typename decltype(data_type_t)::type;
        const auto value_segment =
            std::make_shared<ValueSegment<SegmentDataType>>();
        chunk->add_segment(value_segment);
      });
    }
  }
  _contents.emplace_back(chunk);
}

void Table::append(const std::vector<AllTypeVariant>& values) {
  if(_contents.empty() || _contents[_contents.size()-1]->size() == target_chunk_size()) {
    create_new_chunk();
  }
  auto last_chunk_index = _contents.size() - 1;
  _contents[last_chunk_index]->append(values);
}

ColumnCount Table::column_count() const {
  return static_cast<ColumnCount>(column_definition_by_index.size());
}

ChunkOffset Table::row_count() const {
  auto count = 0;
  for(auto chunk : _contents){
    auto segment = chunk->get_segment(ColumnID{0});
    count += segment->size();
  }
  return count;
}

ChunkID Table::chunk_count() const {
  return static_cast<ChunkID>(_contents.size());
}

ColumnID Table::column_id_by_name(const std::string& column_name) const {
  auto column_id = column_name_index.at(column_name);
  return static_cast<ColumnID>(column_id);
}

ChunkOffset Table::target_chunk_size() const {
  return my_target_chunk_size;
}

const std::vector<std::string>& Table::column_names() const {
  static std::vector<std::string> names;
  for(auto [name, _] : column_definition_by_index){
    names.push_back(name);
  }
  return names;
}

const std::string& Table::column_name(const ColumnID column_id) const {
  DebugAssert(column_id< column_definition_by_index.size(), "ColumnID greater than Column count");
  return column_definition_by_index[column_id].first;
}

const std::string& Table::column_type(const ColumnID column_id) const {
  DebugAssert(column_id< column_definition_by_index.size(), "ColumnID greater than Column count");
  return column_definition_by_index[column_id].second;
}

std::shared_ptr<Chunk> Table::get_chunk(ChunkID chunk_id) {
  DebugAssert(chunk_id<_contents.size(), "ChunkID greater than Chunk count");
  return _contents[chunk_id];
}

std::shared_ptr<const Chunk> Table::get_chunk(ChunkID chunk_id) const {
  Fail("Implementation is missing.");
}

void Table::compress_chunk(const ChunkID chunk_id) {
  Fail("Implementation is missing.");
}

}  // namespace opossum
