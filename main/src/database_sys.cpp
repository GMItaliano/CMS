#include "database_sys.h"

#include <stdlib.h>

#include <iostream>
#include <cstdlib>  // for setenv

database_sys::database_sys() {
    // Get the current working directory
    const char* pythonPath;

    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    //------------------------------------ UBUNTU ------------------------------------
    // Append the Python script directory to the path -> Computer linux
    //std::string PythonPath = std::string(buffer) + "/main/src"; 

    //Set path
    //setenv("PYTHONPATH", PythonPath.c_str(), 1);
    //--------------------------------------------------------------------------------


    //------------------------------------- RASP -------------------------------------
    setenv("PYTHONPATH", ".", 1);
    //--------------------------------------------------------------------------------

    pythonPath = std::getenv("PYTHONPATH");
    std::cout << "PYTHONPATH: " << (pythonPath ? pythonPath : "not set") << std::endl;

    Py_Initialize();

    pModule = PyImport_ImportModule("py_database");
    

    if (pModule == nullptr) {
        PyErr_Print();
        fprintf(stderr, "Failed to load Python module.\n");
    } else {

         if (PyObject_HasAttrString(pModule, "innit_database")) {
            // Call the 'innit_database' function
            PyObject* pInnit = PyObject_GetAttrString(pModule, "innit_database");

            if (pInnit && PyCallable_Check(pInnit)) {
                PyObject_CallObject(pInnit, NULL);
                Py_DECREF(pInnit);
                std::cout << "--> INNIT DATABASE <--" << std::endl;
            } else {
                PyErr_Print();
                fprintf(stderr, "Error calling 'innit_database' function.\n");
            }
        } else {
            fprintf(stderr, "Function 'innit_database' not found in the Python module.\n");
        }

        // Your existing code for calling Python functions goes here
    }
}


database_sys::~database_sys() {
    Py_XDECREF(this->pModule);
    Py_Finalize();
}

void database_sys::send_data(const std::string& path, const std::string& key, const std::string& value) {
    
    if (this->pModule != nullptr) {
        PyObject* pSendData = PyObject_GetAttrString(this->pModule, "set_operation");

        if (PyCallable_Check(pSendData)) {
            PyObject* pArgs = PyTuple_Pack(3, Py_BuildValue("s", path.c_str()), Py_BuildValue("s", key.c_str()), Py_BuildValue("s", value.c_str()));
            PyObject_CallObject(pSendData, pArgs);

            Py_DECREF(pArgs);
            Py_DECREF(pSendData);
        } else {
            fprintf(stderr, "Error: The 'set_operation' function is not callable.\n");
        }
    }
}

void database_sys::push_data(const std::string& path, const std::string& data){
    
    if (this->pModule != nullptr) {
        PyObject* pPushData = PyObject_GetAttrString(this->pModule, "push_data");

        if (PyCallable_Check(pPushData)) {
            PyObject* pArgs = PyTuple_Pack(2, Py_BuildValue("s", path.c_str()), Py_BuildValue("s", data.c_str()));
            PyObject_CallObject(pPushData, pArgs);

            Py_DECREF(pArgs);
            Py_DECREF(pPushData);
        } else {
            fprintf(stderr, "Error: The 'push_data' function is not callable.\n");
        }
    }
}

std::string database_sys::receive_data(const std::string& path) {
    
    if (this->pModule != nullptr) {
        PyObject* pReceiveData = PyObject_GetAttrString(this->pModule, "get_data_path");

        if (PyCallable_Check(pReceiveData)) {
            // Call the Python function
            PyObject* pArgs = PyTuple_Pack(1, Py_BuildValue("s", path.c_str()));
            PyObject* pdata_read = PyObject_CallObject(pReceiveData, pArgs);

            // Check if the result is a string
            if (pdata_read != NULL && PyUnicode_Check(pdata_read)) {
                const char* data_read = PyUnicode_AsUTF8(pdata_read);

                // Clean up objects
                Py_DECREF(pArgs);
                Py_DECREF(pdata_read);
                Py_DECREF(pReceiveData);

                return data_read;
            } else {
                fprintf(stderr, "Error: The result is not a valid string.\n");
            }

            // Clean up
            Py_DECREF(pArgs);
            Py_DECREF(pReceiveData);
        } else {
            fprintf(stderr, "Error: The 'get_data_path' function is not callable.\n");
        }
    }

    return "";  
}

void database_sys::flags_update(int type, bool state) {
    std::string sensor_name;

    switch (type) {
        case 0:
            sensor_name = "Button";
            break;
        case 1:
            sensor_name = "Motion";
            break;
        case 2:
            sensor_name = "Magnetic";
            break;
    }

    if (this->pModule != nullptr) {
        PyObject* pFlagsUpdate = PyObject_GetAttrString(this->pModule, "update_flags");

        if (pFlagsUpdate != nullptr && PyCallable_Check(pFlagsUpdate)) {
            PyObject* pArgs = PyTuple_Pack(2, Py_BuildValue("s", sensor_name.c_str()), Py_BuildValue("O", state ? Py_True : Py_False));
            PyObject* pResult = PyObject_CallObject(pFlagsUpdate, pArgs);

            // Check for Python exceptions
            if (pResult == nullptr) {
                PyErr_Print();
                fprintf(stderr, "Error during flags_update function call.\n");
            }

            Py_XDECREF(pArgs);
            Py_XDECREF(pResult);
            Py_XDECREF(pFlagsUpdate);
        } else {
            fprintf(stderr, "Error: The 'flags_update' function is not callable.\n");
        }
    }
}

void database_sys::download_audio() {
    if (this->pModule != nullptr) {
        PyObject* pDownload_latest_audio = PyObject_GetAttrString(this->pModule, "download_latest_audio");

        if (pDownload_latest_audio != nullptr && PyCallable_Check(pDownload_latest_audio)) {
            // Call the download_audio_wrapper function with appropriate arguments
            PyObject* pArgs = PyTuple_Pack(0);
            PyObject* pResult = PyObject_CallObject(pDownload_latest_audio, pArgs);

            // Check for Python exceptions
            if (pResult == nullptr) {
                PyErr_Print();
                fprintf(stderr, "Error during download_latest_audio function call.\n");
            }

            Py_XDECREF(pArgs);
            Py_XDECREF(pResult);
            Py_XDECREF(pDownload_latest_audio);
        } else {
            fprintf(stderr, "Error: The 'download_latest_audio' function is not callable.\n");
        }
    }
}
