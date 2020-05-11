#include <nan.h>
#include "graph.h"

using namespace std;
using namespace Nan;
using namespace v8;

NAN_METHOD(PassNumber) {

    Nan::Maybe<double> value = Nan::To<double>(info[0]); 
    Local<Number> retval = Nan::New(value.FromJust() + 42);
    info.GetReturnValue().Set(retval);   

}

NAN_METHOD(PassObject) {
    graph<string, int> G;

    G.addVertex("A");
    G.addVertex("B");

    info.GetReturnValue().Set(G);
}

NAN_MODULE_INIT(Init) {
   Nan::Set(target, New<String>("calc").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(PassNumber)).ToLocalChecked());
        
   Nan::Set(target, New<String>("buildGraph").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(PassObject)).ToLocalChecked());
}

NODE_MODULE(my_addon, Init)