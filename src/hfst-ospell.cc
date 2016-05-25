#include <nan.h>

using v8::String;
using v8::Local;
using v8::Array;

#import "ZHfstOspeller.h"
#import "ospell.h"

using hfst_ol::ZHfstOspeller;
using hfst_ol::Transducer;

class SpellChecker : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("SpellChecker").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "check", Check);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("SpellChecker").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
  }

private:
  explicit SpellChecker(ZHfstOspeller speller) : speller_(speller) {}
  ~SpellChecker() {}

  static NAN_METHOD(New) {
    v8::Isolate *isolate = info.GetIsolate();

    if (info.IsConstructCall()) {
      // Called from JS like `new SpellChecker(filename)`
      if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments: Expected one string");
        return;
      }
      if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Argument should be a string");
        return;
      }

      v8::String::Utf8Value filename_v8(info[0]->ToString());
      std::string filename = std::string(*filename_v8);

      // Read dictionary
      ZHfstOspeller speller;

      try {
        speller.read_zhfst(filename);
      } catch (hfst_ol::ZHfstMetaDataParsingError zhmdpe) {
        std::stringstream error;
        error << "cannot finish reading zhfst archive `" << filename << "`:\n"
              << zhmdpe.what() << ".\n";
        Nan::ThrowError(String::NewFromUtf8(isolate, error.str().c_str()));
        return;
      } catch (hfst_ol::ZHfstZipReadingError zhzre) {
        std::stringstream error;
        error << "cannot read zhfst archive `" << filename << "`:\n"
              << zhzre.what() << ".\n";
        Nan::ThrowError(String::NewFromUtf8(isolate, error.str().c_str()));
        return;
      } catch (hfst_ol::ZHfstXmlParsingError zhxpe) {
        std::stringstream error;
        error << "Cannot finish reading index.xml from `" << filename << "`:\n"
              << zhxpe.what() << ".\n";
        Nan::ThrowError(String::NewFromUtf8(isolate, error.str().c_str()));
        return;
      }

      // Set some options
      //
      // TODO: Set options via arguments (option object)
      speller.set_queue_limit(10);
      // speller.set_weight_limit(-1.0);
      // speller.set_beam(-1.0);
      // speller.set_time_cutoff(0.0);

      SpellChecker *obj = new SpellChecker(speller);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      // Called from JS like `SpellChecker(filename)`: Treat as `new …`
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static inline Nan::Persistent<v8::Function> &constructor() {
    static Nan::Persistent<v8::Function> my_constructor;

    return my_constructor;
  }

  static NAN_METHOD(Check) {
    // Validate JS calling signature
    if (info.Length() < 1) {
      Nan::ThrowTypeError("Wrong number of arguments: Expected one string");
      return;
    }
    if (!info[0]->IsString()) {
      Nan::ThrowTypeError("Argument should be a string");
      return;
    }

    v8::String::Utf8Value arg0(info[0]->ToString());
    std::string word = std::string(*arg0);

    v8::Isolate *isolate = info.GetIsolate();
    SpellChecker *obj = Nan::ObjectWrap::Unwrap<SpellChecker>(info.Holder());

    // Check spelling
    if (obj->speller_.spell(word)) {
      // That word actually spelled correctly!
      info.GetReturnValue().Set(v8::Boolean::New(isolate, false));
      return;
    }

    hfst_ol::CorrectionQueue corrections = obj->speller_.suggest(word);

    v8::Handle<Array> correctionsArray =
        v8::Array::New(isolate, corrections.size());

    // Return an empty result if there was an error creating the array.
    if (correctionsArray.IsEmpty()) {
      info.GetReturnValue().Set(v8::Handle<v8::Array>());
      return;
    }

    // Populate the array
    unsigned int i = 0;
    while (corrections.size() > 0) {
      const std::string &corr = corrections.top().first;
      correctionsArray->Set(i, String::NewFromUtf8(isolate, corr.c_str()));
      corrections.pop();
      i++;
    }

    info.GetReturnValue().Set(correctionsArray);
  }

  ZHfstOspeller speller_;
};

NODE_MODULE(objectwrapper, SpellChecker::Init)
