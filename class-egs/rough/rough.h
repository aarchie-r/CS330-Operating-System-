#ifndef HEADER_TRANSACTION
#define HEADER_TRANSACTION

#include<stdio.h>
#include<sys/syscall.h> // SYS_gettid
#include<unistd.h> // syscall
#include<unordered_set>
#include <utility>
#include <stack>
#include"graph.h"
#include"./ioctl.h"

using std::unordered_set;
using std::pair;
using std::stack;

class TNode {
public:
/// TODO: (MAYBE) replace with TBB concurrent_unordered_map for garbage collection 
  thread_local static TNode* head; // very first transaction: needed for garbage collection
  thread_local static TNode* latest; // latest outermost transaction
  thread_local static unsigned int nested_level; // 0 -> no active transaction, n -> nth nested transaction
  TNode *parent; // parent tnode of nested transaction, nullptr for outermost transaction
  pid_t tid;
  const char *file;
  const long line;
  unordered_set<unsigned long> violations; // ip^cs
  TNode(pid_t _tid, const char* _file, const long _line) : tid(_tid), file(_file), line(_line), parent(nullptr) {}
  static void enter(const char* file, const long line);
  static void exit();
};