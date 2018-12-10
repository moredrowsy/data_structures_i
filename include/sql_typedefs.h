#ifndef SQL_TYPEDEFS_H
#define SQL_TYPEDEFS_H

#include <memory>       // shared_ptr
#include <string>       // string
#include "bpt_map.h"    // MMap class
#include "queue.h"      // Queue class
#include "set.h"        // Set calss
#include "sql_token.h"  // SQLToken class
#include "stack.h"      // Stack class

namespace sql {

// ----- SQLParser -----
typedef bpt_map::Map<std::string, std::size_t> TokenType;
typedef bpt_map::Map<std::size_t, std::string> ParseKey;
typedef bpt_map::MMap<std::string, std::string> ParseTree;

// ----- SQLTable -----
// field pos to field name; ie: pos #1, "lName"; pos #2, "fName"
typedef bpt_map::Map<int, std::string> FieldPosMap;

// field name to field pos; ie: "fName", pos #2; "lName", pos #1
typedef bpt_map::Map<std::string, int> FiledNamesMap;

// map chains
typedef bpt_map::Map<std::string, set::Set<long>> IndexMap;
typedef bpt_map::Map<std::string, IndexMap> FieldMap;

// conditional WHERE
typedef std::shared_ptr<sql::SQLToken> token_ptr;
typedef queue::Queue<token_ptr> QueueTokens;
typedef stack::Stack<token_ptr> StackTokens;

// ----- SQL -----
typedef std::shared_ptr<set::Set<long>> set_ptr;

}  // namespace sql

#endif  // SQL_TYPEDEFS_H
