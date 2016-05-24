#include <nan.h>

using v8::String;
using v8::Local;
using v8::Array;

#import "ospell.h"
#import "ZHfstOspeller.h"

using hfst_ol::ZHfstOspeller;
using hfst_ol::Transducer;


void Spellcheck(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    // Validate JS calling signature 
    if (args.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    if (!args[0]->IsString()) {
        Nan::ThrowTypeError("Argument should be a string");
        return;
    }

    v8::String::Utf8Value arg0(args[0]->ToString());
    std::string word = std::string(*arg0);
    
    // Get v8 context
    v8::Isolate* isolate = args.GetIsolate();

    // FIXME: Read file name from function arguments 
    const char* file_name = "etc/se.zhfst";

    // Initialize the spell checker
    //
    // TODO: Use node wrapper for native objects to initialize and free this
    //       instead of reading the file every time.
    ZHfstOspeller speller;
    
    try {
        speller.read_zhfst(file_name);
    } catch (hfst_ol::ZHfstMetaDataParsingError zhmdpe) {
        std::stringstream error;
        error << "cannot finish reading zhfst archive " << file_name << ":\n"
            << zhmdpe.what() << ".\n";
        Nan::ThrowError(String::NewFromUtf8(isolate, error.str().c_str()));
        return;
    } catch (hfst_ol::ZHfstZipReadingError zhzre) {
        std::stringstream error;
        error << "cannot read zhfst archive " << file_name << ":\n"
            << zhzre.what() << ".\n";
        Nan::ThrowError(String::NewFromUtf8(isolate, error.str().c_str()));
        return;
    } catch (hfst_ol::ZHfstXmlParsingError zhxpe) {
        std::stringstream error;
        error << "Cannot finish reading index.xml from " << file_name << ":\n"
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
    
    // Check spelling
    if (speller.spell(word)) {
        // That word actually spelled correctly!
        args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
        return;
    }
    
    hfst_ol::CorrectionQueue corrections = speller.suggest(word);

    v8::Handle<Array> correctionsArray = Array::New(isolate, corrections.size());

    // Return an empty result if there was an error creating the array.
    if (correctionsArray.IsEmpty()) {
        args.GetReturnValue().Set(v8::Handle<Array>());
        return;
    }
    
    // Populate the array
    unsigned int i = 0;
    while (corrections.size() > 0) {
        const std::string& corr = corrections.top().first;
        correctionsArray->Set(i, String::NewFromUtf8(isolate, corr.c_str()));
        corrections.pop();
        i++;
    }

    args.GetReturnValue().Set(correctionsArray);
}

void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("spellcheck").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Spellcheck)->GetFunction());
}

NODE_MODULE(Spellcheck, Init)
