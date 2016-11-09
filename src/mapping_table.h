
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
 *
 * The current implementation is based on mmap() where the mapping memory
 * region is far more than enough to support the maximum sized tree 
 */
class MappingTable {
  // This is a fixed size of a large enough mapping table
  static constexpr MAPPING_TABLE_SIZE = 0x1UL << 36;
  
  // We could not use 0 since 0 is designated as INVALID_NODE_ID
  static constexpr FIRST_NODE_ID = 1UL;
  
 private:
  void **table;
  std::atomic<uint64_t> next_node_id;
  
 public:
   
  /*
   * Constructor
   */
  MappingTable() {
    // 1. This maps a memory region that are determined by the kernel
    // 2. The virtual memory region is not backed by any swap space
    // 3. The memory is mapped first to zeroed page for read, and then
    //    copied on write
    void *p = mmap(NULL, 
                   MAPPING_TABLE_SIZE, 
                   PROT_READ | PROT_WRITE, 
                   MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE, 
                   -1, 
                   0);
    
    // If mmap failed just exit and print out the reason               
    if(p == MAP_FAILED) {
      fprintf(stderr, 
              "ERROR: mmap() returns -1; reason = \"%s\"\n", 
              strerror(errno));
      exit(1);
    } else {
      bwt_printf("Mapping table mmap() = %p\n", p); 
    }

    // If mmap() succeeds just use the returned mapping table
    table = reinterpret_cast<void **>(p);
    
    // Must start from 1 since INVALID_NODE_ID is 0
    next_node_id.store(FIRST_NODE_ID);
    
    return;
  }
  
  /*
   * Destructor - Unmaps memory
   */
  ~MappingTable() {
    
  }
};

#ifdef BWTREE_PELOTON
}
}
#endif

#endif
