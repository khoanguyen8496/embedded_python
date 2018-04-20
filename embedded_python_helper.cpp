#include "embedded_python_helper.h"

bool check_object_pointer(PyObject *obj, const char *msg) {
  if (!obj) {
    PyErr_Print();
    fprintf(stderr, "%s\n", msg);
    return 0;
  }
  return 1;
}

void debug_pystring_list(PyObject *obj) {
  assert(obj);
  fprintf(stderr, "debug pylist of strings\n");
  int n = PyList_Size(obj);
  int i;
  for (i = 0; i < n; ++i) {
    PyObject *item = PyList_GetItem(obj, i);
    // Py_INCREF(item);
    if (PyString_Check(item)) {
      char *sps = PyString_AsString(item);
      fprintf(stderr, "debug item %d %s\n", i, sps);
    } else {
      fprintf(stderr, "Item is not string\n");
    }
  }
}

// append syspath to load custom modules path
void append_syspath(const char *path) {
  // setting syspath
  // could be used to add more modules path
  PyObject *syspath = PySys_GetObject((char *)"path");
  assert(syspath);
  if (syspath) {
    fprintf(stderr, "before set path\n");
    PyObject *pypath = PyString_FromString(path);
    assert(pypath);
    PyList_Append(syspath, pypath);
    debug_pystring_list(syspath);
  } else {
    fprintf(stderr, "Cannot get syspath\n");
  }
}

// wrapper function to load python module from python path
PyObject *import_module(const char *module_name) {
  PyObject *pmn = Py_BuildValue("s", module_name);
  if (!check_object_pointer(pmn, "Error building value\n")) return NULL;
  PyObject *lmod = NULL;
  lmod = PyImport_Import(pmn);
  if (!check_object_pointer(lmod, "Error importing module\n")) return NULL;
  return lmod;
}

// get submodule part from the object
// used for getting class out of module
PyObject *get_submodule(PyObject *obj, const char *sub) {
  PyObject *submod = NULL;
  submod = PyObject_GetAttrString(obj, sub);
  if (!check_object_pointer(submod, "Error getting submodule %s\n"))
    return NULL;
  return submod;
}

// call object to run and return value
// args mut be a python tuple
PyObject *call_object(PyObject *obj, PyObject *args) {
  PyObject *result = NULL;
  if (check_object_pointer(obj, "Object not found to perform call\n")) {
    result = PyEval_CallObject(obj, args);
    if (!check_object_pointer(result, "Error calling object\n")) return NULL;
  }
  return result;
}

// check if an object is a dict
bool check_dict(PyObject *obj) { return 1; }

// check object is a string
bool check_string(PyObject *obj) {
  if (!check_object_pointer(obj, "check string object is null\n"))
    return 0;
  if (!PyObject_TypeCheck(obj, &PyBaseString_Type)) {
    PyErr_SetString(PyExc_ValueError, "object must be a string.");
    return 0;
  }
  return 1;
}

// check object is a dict of double
bool check_string_dict(PyObject *obj) { return 1; }

// check whether gene expression result is valid
bool check_gene_expression_result(PyObject *obj) { return 1; }

// check if the obj is a string and parse it to json value
Json::Value get_gene_expression_json(PyObject *obj) {
  if (!check_string(obj))
    throw std::logic_error("Object is not a string");
  Json::Value value;
  std::stringstream ss;
  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  JSONCPP_STRING errs;
  bool ok = Json::parseFromStream(builder, ss, &value, &errs);
  return value;
}
