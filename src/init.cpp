#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

#include <memory>
#include <vector>

class SomeClass {
public:
  std::vector<int> some_member;
};

std::unique_ptr<SomeClass> some_function(int* data, int n) {
  std::unique_ptr<SomeClass> some_instance = std::unique_ptr<SomeClass>(new SomeClass());
  for (int i = 0; i < n; i++) {
    some_instance->some_member.push_back(data[i]);
  }

  return some_instance;
}

SEXP some_function_wrapper(SEXP data_sexp) {
  if (Rf_length(data_sexp) == 0) {
    return R_NilValue;
  } else {
    std::unique_ptr<SomeClass> some_unique_ptr =
      some_function(INTEGER(data_sexp), Rf_length(data_sexp));
    return Rf_ScalarInteger(some_unique_ptr->some_member.size());
  }
}

static const R_CallMethodDef CallEntries[] = {
  {"some_function_wrapper", (DL_FUNC) &some_function_wrapper, 1},
  {NULL, NULL, 0}
};

extern "C" void R_init_rchkdeleter(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
