/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGenericSubdivisionErrorMetric.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkGenericSubdivisionErrorMetric - class to compute error during cell tessellation
// .SECTION Description
// This class is used to compute a tessellation error during cell
// subdivision.  Cell subdivision is performed in the context of the adaptor
// framework: higher-order, or complex cells, are automatically tessellated
// into simplices so that they can be processed with conventional
// visualization algorithms.
// 
// While this class implements a simple error measure based on geometric
// and attribute error (i.e., variation of edge from a straight line,
// variation of the attribute value from a linear ramp), it is designed
// to be subclassed.

// See Also
// vtkGenericCellTessellator

#ifndef __vtkGenericSubdivisionErrorMetric_h
#define __vtkGenericSubdivisionErrorMetric_h

#include "vtkObject.h"

class vtkGenericAttributeCollection;
class vtkGenericAdaptorCell;
class vtkGenericDataSet;

class VTK_FILTERING_EXPORT vtkGenericSubdivisionErrorMetric : public vtkObject
{
public:
  // Description:
  // Construct the tessellator.
  static vtkGenericSubdivisionErrorMetric *New();
  
  // Description:
  // Standard VTK type and error macros.
  vtkTypeRevisionMacro(vtkGenericSubdivisionErrorMetric,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Specify the number of component to interpolate
  // This is for internal use only
  vtkGetMacro(GeometricTolerance, double);
 
  // Description:
  // Set the geometric accuracy with an absolute value.
  // This is the geometric object-based accuracy.
  // Subdivision will be required if the square distance between the real
  // point and the interpolated point is greater than `value'. For instance
  // 0.01 will give better result than 0.1.
  // \pre positive_value: value>0
  void SetAbsoluteGeometricTolerance(double value);
  
  // Description:
  // Set the geometric accuracy with a value relative to the length of the
  // bounding box of the dataset. Internally compute the absolute tolerance.
  // For instance 0.01 will give better result than 0.1.
  // \pre valid_range_value: value>0 && value<1
  // \pre ds_exists: ds!=0
  void SetRelativeGeometricTolerance(double value,
                                     vtkGenericDataSet *ds);
  
  // Description:
  // Subdivision is required if the square distance between the projection of
  // the real point and the projection of the interpolated point is greater
  // than PixelTolerance.
  // This is the geometric screen-based accuracy.
  // An accuracy less or equal to 0.25 means that the two projected point are
  // on the same pixel.
  // CAN WE HAVE A VALUE <0.25 with antialiasing ?
  vtkGetMacro(PixelTolerance, double);
  
  // Description:
  // Set the pixel accuracy to `value'. See GetPixelTolerance() for details.
  // \pre valid_value: value>=0.25
  void SetPixelTolerance(double value);
  
  // Description:
  // Relative tolerance of the active scalar (attribute+component).
  // Subdivision is required if the square distance between the real attribute
  // at the mid point on the edge and the interpolated attribute is greater
  // than AttributeTolerance.
  // This is the attribute accuracy.
  // 0.01 will give better result than 0.1.
  vtkGetMacro(AttributeTolerance, double);
  
  // Description:
  // Set the relative attribute accuracy to `value'. See
  // GetAttributeTolerance() for details.
  // \pre valid_range_value: value>0 && value<1
  void SetAttributeTolerance(double value);

  // Description:
  // Return whether the indicated edge exceeds the error metric.
  // e1 and e2 are in parametric coordinates.
  virtual bool EvaluateEdge( double* e1, double* e2 );
  
  // Description:
  // The error metric is based on variation of the tessellation
  // from the actual cell geometry.
  virtual void SetGenericCell(vtkGenericAdaptorCell*);
  vtkGetObjectMacro(GenericCell,vtkGenericAdaptorCell);
  
  // Description:
  // The error metric may be based on attribute variation.
  virtual void SetAttributeCollection(vtkGenericAttributeCollection*);
  vtkGetObjectMacro(AttributeCollection,vtkGenericAttributeCollection);
  
  // Description:
  // Subclasses of this class may evaluate screen error by
  // overloading this method. This method returns the screen
  // error of a particular edge given the coordinates of the
  // endpoint of the edge. (It has been made virtual to avoid
  // dependencies on the VTK/Rendering subdirectory. Subclasses
  // of this class can be found in VTK/GenericFiltering.)
  virtual double EvaluateScreenError(double *vtkNotUsed(e1), 
                                     double *vtkNotUsed(e2))
    {return 0.0;}

protected:
  vtkGenericSubdivisionErrorMetric();
  virtual ~vtkGenericSubdivisionErrorMetric();
  
  // Description:
  // Compute the absolute attribute tolerance, only if the cached value is
  // obsolete.
  void ComputeAbsoluteAttributeTolerance();
  
  
  // Description:
  // Compute world coordinates of the vertices `e1' and `e2' defining the edge.
  // The result is in Edge1Cache and Edge2Cache. The middle of the straight
  // line is InterpolatedCenterCache, the middle of the arc is RealCenterCache.
  void ComputeCoordinates(double *e1,
                          double *e2);
  
  // Description:
  // Square distance between a straight line (defined by points x and y)
  // and a point z. Property: if x and y are equal, the line is a point and
  // the result is the square distance between points x and z.
  double Distance2LinePoint(double x[3],
                            double y[3],
                            double z[3]);
  
  double GeometricTolerance;
  double PixelTolerance;
  double AttributeTolerance;
  
  double AbsoluteAttributeTolerance; // cached value computed from
  // AttributeTolerance and active attribute/component
  vtkTimeStamp AbsoluteAttributeToleranceComputeTime;
  
  vtkGenericAttributeCollection *AttributeCollection;
  vtkGenericAdaptorCell *GenericCell;
  vtkTimeStamp SubdivisionMTime;

  // Format: Edge1Cache = xyz abc abc abc ...
  double *Edge1Cache;
  double *Edge2Cache;
  
  // Format: xyz
  double RealCenterCache[3];
  double InterpolatedCenterCache[3];
  
  double EvaluateGeometricError(double *e1, double *e2);
  double EvaluateAttributesError(double *e1, double *e2);

private:
  vtkGenericSubdivisionErrorMetric(const vtkGenericSubdivisionErrorMetric&);  // Not implemented.
  void operator=(const vtkGenericSubdivisionErrorMetric&);  // Not implemented.
};

#endif

