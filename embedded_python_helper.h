#ifndef EMBEDDED_PYTHON_HELPER

#include <Python.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

#include "json.h"

#define CH_DECREF(ref)     \
{                          \
  if (ref) Py_DECREF(ref); \
}

bool check_object_pointer(PyObject *obj, const char *msg);

void debug_pystring_list(PyObject *obj);

// append syspath to load custom modules path
void append_syspath(const char *path);

// wrapper function to load python module from python path
PyObject *import_module(const char *module_name);

// get submodule part from the object
// used for getting class out of module
PyObject *get_submodule(PyObject *obj, const char *sub);

// call object to run and return value
// args mut be a python tuple
PyObject *call_object(PyObject *obj, PyObject *args);

// util to parse dict from python
// return 1 if the object is a dict
bool check_dict(PyObject *obj);
// return 1 if the object is a string
bool check_string(PyObject *obj);
// return 1 if the object is a dict of strings
bool check_string_dict(PyObject *obj);
// return 1 if the object is a dict of strings
bool check_gene_expression_result(PyObject *obj);
// return json value
Json::Value get_gene_expression_json(PyObject *obj);
#endif  // EMBEDDED_PYTHON_HELPER
