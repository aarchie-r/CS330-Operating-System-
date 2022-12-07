#include "object.h"
#include <assert.h>

unordered_map<void*,Obj*> Obj::objMap;

void Obj::logAccess(void *addr, int isWrite, int tid, TNode* tnode, unsigned long cs, unsigned long ip) {
  assert(tnode);
  if(tnode->violations.find(ip^cs)!=tnode->violations.end()) return;
//   printf("LOGGING ACCESS: addr %p isWrite %d tid %d tnode %p ip %lu cs %lu\n", addr, isWrite, tid, tnode, ip, cs);
  tnode->violations.insert(ip^cs);
  if(Obj::objMap.find(addr)==Obj::objMap.end()) Obj::objMap[addr] = new Obj(addr); 
  Obj *obj = Obj::objMap[addr];
  Access *access = new Access(addr,tnode,isWrite,ip,cs);
  // update graph
  /// TODO: DOUBT - add edges simultaneously to check for cycle ?
  if(obj->write) {
    Graph::try_add_edge(obj->write,access);
  }
  for(Access *read : obj->reads) {
    if(!read) break;
    Graph::try_add_edge(read, access);
  }
  // update obj
  if(isWrite) obj->write=access;
  else {
    /// TODO: generalize it to any number of reads
    obj->reads[1] = obj->reads[0];
    obj->reads[0] = access;
  }
}
