#ifndef EMBEDDED_PYTHON_HELPER

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

#define CH_DECREF(ref) \
{\
	if (ref) \
		Py_DECREF(ref);\
}

int check_object_pointer(PyObject *obj, const char *msg);

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
int check_dict(PyObject *obj);
// return 1 if the object is a string
int check_string(PyObject *obj);
// return 1 if the object is a dict of strings
int check_string_dict(PyObject *obj);
// return 1 if the object is a dict of strings
int check_gene_expression_result(PyObject *obj);
// return 1 if the retrieve process is sucessfully done
#endif // EMBEDDED_PYTHON_HELPER
