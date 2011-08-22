// .NAME vtkPTXReader - read a ptx file into a polydata object
// .SECTION Description
// vtkPTXReader reads a ptx file into a polydata object

#ifndef __vtkPTXData_h
#define __vtkPTXData_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"
#include "vtkDenseArray.h"
#include "vtkVector.h"
#include "vtkColor.h"
#include "vtkImageData.h"

class vtkPTXData : public vtkDataObject
{
public:
  vtkTypeRevisionMacro(vtkPTXData,vtkDataObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPTXData* New();
  
  unsigned int GetNumberOfPhiPoints();
  unsigned int GetNumberOfThetaPoints();
  
  //BTX
  vtkSmartPointer<vtkDenseArray<vtkVector3d> > GetPointGrid();
  vtkSmartPointer<vtkDenseArray<bool> > GetValidGrid();
  vtkSmartPointer<vtkDenseArray<vtkColor3ub> > GetColorGrid();  
  void CreateImage(vtkSmartPointer<vtkImageData> image);
  //ETX
  
  //funtions
  void GeneratePointPolyData(vtkPolyData* output);
  void GenerateTriangulatedPolyData(vtkPolyData* result);
  void GenerateTriangulatedPolyDataSpanGaps(vtkPolyData* result);
  
    //internal data - this should be made private and use the GetXXX accessors
  //BTX
  vtkSmartPointer<vtkDenseArray<vtkVector3d> > PointGrid;
  vtkSmartPointer<vtkDenseArray<bool> > ValidGrid;
  vtkSmartPointer<vtkDenseArray<vtkColor3ub> > ColorGrid;
  vtkSmartPointer<vtkDenseArray<double> > IntensityGrid;
  
  vtkSmartPointer<vtkDenseArray<int> > IdGrid;
  //ETX
  
protected:
  vtkPTXData();
  ~vtkPTXData(){}
  
private:
  vtkPTXData(const vtkPTXData&);  // Not implemented.
  void operator=(const vtkPTXData&);  // Not implemented.
  

};

#endif
