#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void debug_pystring_list(PyObject *obj)
{
	fprintf(stderr, "debug pylist of strings\n");
	int n = PyList_Size(obj);
	int i;
	for (i = 0; i < n; ++i) {
		PyObject *item = PyList_GetItem(obj, i);
		if (PyString_Check(item)) {
			char *sps = PyString_AsString(item);
			fprintf(stderr, "debug item %d %s\n", i, sps);
		} else {
			fprintf(stderr, "Item is not string\n");
		}
		Py_DECRE(item);
	}
}

// append syspath to load custom modules path
void append_syspath(const char *path)
{
	// setting syspath
	// could be used to add more modules path
	PyObject *syspath = PySys_GetObject((char*)"path");
	assert(syspath);
	if (syspath) {
		fprintf(stderr, "before set path\n");
		PyObject *pypath = PyString_FromString(path);
		assert(pypath);
		PyList_Append(syspath, pypath);
		// debug_pystring_list(syspath);
		Py_DECREF(syspath);
	} else {
		fprintf(stderr, "Cannot get syspath\n");
	}
}

// wrapper function to load python module from python path
PyObject *import_module(const char *module_name)
{
	PyObject *lmod = NULL;
	lmod = PyImport_ImportModule(module_name);
	if (!lmod) {
		PyErr_Print();
		fprintf(stderr, "Error loading module %s\n", module_name);
		return NULL;
	}
	return lmod;
}

// get submodule part from the object
PyObject *get_submodule(PyObject *obj, const char *sub)
{
	PyObject *submod = NULL;
	submod = PyObject_GetAttrString(obj, sub);
	if (!submod) {
		PyErr_Print();
		fprintf(stderr, "Error getting submodule %s\n", sub);
		return NULL;
	}
	return submod;
}

int main(int argc, char *argv[])
{
	fprintf(stderr, "starting embedded python-c\n");
	Py_Initialize();
	if( !Py_IsInitialized() ) {
		puts("Unable to initialize Python interpreter.");
		return 1;
	}

	append_syspath(".");

	PyObject *pip_obj = import_module("mock");
	assert(pip_obj);
	PyObject *os_obj = import_module("os");
	assert(os_obj);
	PyObject *curdir = get_submodule(os_obj, "curdir");
	assert(curdir);

	// assume that curdir is a string
	// check string
	if (PyString_Check(curdir)) {
		char *str = PyString_AsString(curdir);
		fprintf(stderr, "debug string %s\n", str);
	}

	// prepare to finalize
	// decrease all references
	if (os_obj)
		Py_DECREF(os_obj);
	if (curdir)
		Py_DECREF(curdir);
	if (pip_obj)
		Py_DECREF(pip_obj);

	// decrease all object ref to feed the garbage collector
	Py_Finalize();
	return 0;
}
