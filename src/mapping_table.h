
#ifndef _MAPPING_TABLE_H
#define _MAPPING_TABLE_H

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

#ifdef BWTREE_PELOTON
namespace peloton {
namespace index {
#endif

/*
 * class MappingTable() - Implementation of the mapping table that allows for 
 *                        extension
 */
class MappingTable {
  // This is a fixed size of a large enough mapping table
  static constexpr MAPPING_TABLE_SIZE = 0x1UL << 36;
 private:
  void **table;
  std::atomic<uint64_t> next_node_id;
  
 public:
  MappingTable() {
    void *p = mmap(NULL, MAPPING_TABLE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    table = reinterpret_cast<void **>()
  }
};

#ifdef BWTREE_PELOTON
}
}
#endif

#endif
