#include <nan.h>

// #import <libhfstospell/ospell.h>
// #import <libhfstospell/ZHfstOspeller.h>

using v8::String;
using v8::Local;

void Hello(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!args[0]->IsString()) {
        Nan::ThrowTypeError("Argument should be a string");
        return;
    }

    v8::Isolate* isolate = args.GetIsolate();
    Local<String> hello = String::NewFromUtf8(isolate, "hello, ");
    Local<String> output = String::Concat(hello, args[0]->ToString());

    args.GetReturnValue().Set(output);
}

void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("hello").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Hello)->GetFunction());
}

NODE_MODULE(hello, Init)
