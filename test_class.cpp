#include "embedded_python_helper.h"

void test_my_module() {
  PyObject *my_obj = import_module("my_test");
  int f = 0;
  f = PyObject_HasAttrString(my_obj, "test");
  fprintf(stderr, "%d has attr string test\n", f);

  PyObject *test = get_submodule(my_obj, "test");
  PyObject *res = call_object(test, NULL);

  try {
    Json::Value value = get_gene_expression_json(res);
  } catch (std::exception &ex) {
    std::string ex_string = ex.what();
    fprintf(stderr, "Caught exception %s\n", ex_string.c_str());
  } catch (...) {
    fprintf(stderr, "Undefined Exception\n");
  }

  assert(res);
  CH_DECREF(test);
  CH_DECREF(res);
}

void numpy_test() {
  PyObject *np_obj = import_module("numpy");
  if (!check_object_pointer(np_obj, "numpy object is null\n")) return;
  assert(np_obj);
  PyObject *obj = get_submodule(np_obj, "array");
  if (!check_object_pointer(obj, "array numpy object is null\n")) return;
  assert(obj);
  CH_DECREF(np_obj);
}

void example_test() {
  // decrease all object ref to feed the garbage collector
}

int main(int argc, char *argv[]) {
  fprintf(stderr, "starting embedded python-c\n");
  Py_Initialize();
  if (!Py_IsInitialized()) {
    puts("Unable to initialize Python interpreter.");
    return 1;
  }

  append_syspath("");
  test_my_module();
  fprintf(stderr, "Before Finalize\n");
  Py_Finalize();
  return 0;
}
