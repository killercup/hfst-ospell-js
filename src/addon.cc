#include "spellchecker.h"
#include <nan.h>

void InitAll(v8::Local<v8::Object> exports) { SpellChecker::Init(exports); }

NODE_MODULE(addon, InitAll)