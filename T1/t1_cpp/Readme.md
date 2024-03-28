compile:

```
emcc t1.cpp -s MODULARIZE=1 -s EXPORT_ES6=1  -s EXPORTED_FUNCTIONS="['_bocchi_shut_up','_malloc']" -s "EXPORTED_RUNTIME_METHODS=['cwrap']" -o t1.ems.js
```