set(classes
  vtkPythonArgs
  vtkPythonCommand
  vtkPythonOverload
  vtkPythonUtil
  vtkSmartPyObject)

set(headers
  vtkPythonCompatibility.h)

set(sources
  PyVTKExtras.cxx
  PyVTKMethodDescriptor.cxx
  PyVTKNamespace.cxx
  PyVTKObject.cxx
  PyVTKReference.cxx
  PyVTKSpecialObject.cxx
  PyVTKTemplate.cxx)

set(private_headers
  PyVTKExtras.h
  PyVTKMethodDescriptor.h
  PyVTKNamespace.h
  PyVTKObject.h
  PyVTKReference.h
  PyVTKSpecialObject.h
  PyVTKTemplate.h)

vtk_module_add_module(VTK::WrappingPythonCore
  CLASSES ${classes}
  SOURCES ${sources}
  HEADERS ${headers}
  PRIVATE_HEADERS ${private_headers}
  LIBRARY_NAME_SUFFIX "${PYTHON_MAJOR_VERSION}${PYTHON_MINOR_VERSION}")
