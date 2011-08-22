#include "vtkPTXData.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkDoubleArray.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkImageData.h"
#include "vtkDelaunay2D.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include "vtkDenseArray.h"
#include "vtkCellArray.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkTriangleFilter.h"
#include "vtkQuad.h"

//for testing only
#include "vtkVertexGlyphFilter.h"

#include <vtksys/ios/sstream>

vtkCxxRevisionMacro(vtkPTXData, "$Revision: 1.70 $");
vtkStandardNewMacro(vtkPTXData);

vtkPTXData::vtkPTXData()
{
  this->PointGrid = vtkSmartPointer<vtkDenseArray<vtkVector3d> >::New();
  this->ValidGrid = vtkSmartPointer<vtkDenseArray<bool> >::New() ;
  this->ColorGrid = vtkSmartPointer<vtkDenseArray<vtkColor3ub> >::New() ;
  this->IntensityGrid = vtkSmartPointer<vtkDenseArray<double> >::New() ;
  this->IdGrid = vtkSmartPointer<vtkDenseArray<int> >::New() ;
}

unsigned int vtkPTXData::GetNumberOfPhiPoints()
{
  return this->PointGrid->GetExtents()[1];
}

unsigned int vtkPTXData::GetNumberOfThetaPoints()
{
  return this->PointGrid->GetExtents()[0];
}
  
  
vtkSmartPointer<vtkDenseArray<vtkVector3d> > vtkPTXData::GetPointGrid()
{
  return this->PointGrid;
}

vtkSmartPointer<vtkDenseArray<bool> > vtkPTXData::GetValidGrid()
{
  return this->ValidGrid;
}

vtkSmartPointer<vtkDenseArray<vtkColor3ub> > vtkPTXData::GetColorGrid()
{
  return this->ColorGrid;
}

void vtkPTXData::CreateImage(vtkSmartPointer<vtkImageData> image)
{
  
  image->SetExtent(0, this->GetNumberOfThetaPoints(), 0, this->GetNumberOfPhiPoints(), 0, 0);
  image->SetNumberOfScalarComponents(3);
  image->SetScalarTypeToUnsignedChar ();

  for(unsigned int t = 0; t < this->GetNumberOfThetaPoints(); t++)
  {
    for(unsigned int p = 0; p < this->GetNumberOfPhiPoints(); p++)
    {
      vtkColor3ub color;
      
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
        {
        color.SetRed(0);
        color.SetGreen(0);
        color.SetBlue(0);
        }
      else
        {
        color = this->ColorGrid->GetValue(t,p);
        }    
      
      unsigned char* pixel = static_cast<unsigned char*>(image->GetScalarPointer(t,p,0));
      pixel[0] = color.GetRed();
      pixel[1] = color.GetGreen();
      pixel[2] = color.GetBlue();
    }
  }
}
//----------------------------------------------------------------------------
void vtkPTXData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent
      << "Number of theta points: " << this->GetNumberOfThetaPoints() << endl
      << "Number of phi points: " << this->GetNumberOfPhiPoints() << endl;	
}

void vtkPTXData::GeneratePointPolyData(vtkPolyData* polydata)
{
  //produce a vtk file of the points, colors, and intensities
  
  //setup points
  vtkSmartPointer<vtkPoints> points = 
      vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> vertices = 
      vtkSmartPointer<vtkCellArray>::New();
  
  //setup colors
  vtkSmartPointer<vtkUnsignedCharArray> colors = 
      vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");
  
  //setup intensities
  vtkSmartPointer<vtkDoubleArray> intensities = 
      vtkSmartPointer<vtkDoubleArray>::New();
  intensities->SetNumberOfComponents(1);
  intensities->SetName("Intensities");
  
  for(unsigned int t = 0; t < this->GetNumberOfThetaPoints(); t++)
    {
    for(unsigned int p = 0; p < this->GetNumberOfPhiPoints(); p++)
      {
      
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
        {
        continue;
        }
      
      //add the intensity
      double intensity = this->IntensityGrid->GetValue(t,p);
      intensities->InsertNextValue(intensity);
      
      //add the point
      vtkVector3d point = this->PointGrid->GetValue(t,p);
      vtkIdType pid[1];
      pid[0] = points->InsertNextPoint(point.X(), point.Y(), point.Z());
      vertices->InsertNextCell ( 1,pid );

      //colr the point
      vtkColor3ub color = this->ColorGrid->GetValue(t,p);
      colors->InsertNextTupleValue(color.GetData());
      
      }
    }
    
  polydata->SetPoints(points);
  polydata->SetVerts(vertices);
  polydata->GetPointData()->AddArray(colors);
  polydata->GetPointData()->AddArray(intensities);
  
  cout << "Created " << polydata->GetNumberOfPoints() << " points." << endl;

}

void vtkPTXData::GenerateTriangulatedPolyData(vtkPolyData* result)
{
  this->GeneratePointPolyData(result);
  
  unsigned int pointCounter = 0;
  //Re-number the points excluding the invalid points (create the IdGrid)
  //The IDs of invalid points are set to -1
  for ( unsigned int t = 0; t < this->GetNumberOfThetaPoints(); t++ )
    {
    for ( unsigned int p = 0; p < this->GetNumberOfPhiPoints(); p++ )
      {
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
        { 
        this->IdGrid->SetValue(t,p,-1);
        continue;
        }
      
      this->IdGrid->SetValue(t,p,pointCounter);
      pointCounter++;
      }
    }
  
  vtkSmartPointer<vtkCellArray> quads = 
      vtkSmartPointer<vtkCellArray>::New();
    
  //we are going to make a quad on the grid for every point that has a valid up, left, and up-left neighbor
  
  for ( unsigned int t = 0; t < this->GetNumberOfThetaPoints(); t++ )
    {
    for ( unsigned int p = 0; p < this->GetNumberOfPhiPoints(); p++ )
      {
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
        { 
        continue;
        }
      
      if(t == 0) //on the left of the grid
        {
        continue;
        }
      if(p == this->GetNumberOfPhiPoints() - 1) // on the top of the grid
        {
        continue;
        }

      //if any of the 3 points defining the quad to the top left are invalid
      if(!this->ValidGrid->GetValue(t-1, p+1)) // point one left, one up
        {
        continue;
        }
      
      if(!this->ValidGrid->GetValue(t-1, p)) // point one left
        {
        continue;
        }
      
      if(!this->ValidGrid->GetValue(t, p+1)) // point one up
        {
        continue;
        }
        
      {
      /*
        cout << "Four ids are: " << this->IdGrid->GetValue(t,p) << ", " <<
      this->IdGrid->GetValue(t,p+1) << ", " <<
      this->IdGrid->GetValue(t-1,p+1) << ", " <<
      this->IdGrid->GetValue(t-1,p) << endl;
        */
      }
      
      vtkSmartPointer<vtkQuad> quad = 
          vtkSmartPointer<vtkQuad>::New();
      quad->GetPointIds()->SetId(0,this->IdGrid->GetValue(t,p));
      quad->GetPointIds()->SetId(1,this->IdGrid->GetValue(t,p+1));
      quad->GetPointIds()->SetId(2,this->IdGrid->GetValue(t-1,p+1));
      quad->GetPointIds()->SetId(3,this->IdGrid->GetValue(t-1,p));
      
      quads->InsertNextCell(quad);
      
        
      } //end p for
    }//end t for
    
  result->SetPolys(quads);
  
  cout << "There are " << result->GetNumberOfPoints() << " points." << endl;
  cout << "There are " << result->GetNumberOfCells() << " cells." << endl;
    
  vtkSmartPointer<vtkTriangleFilter> triangleFilter = 
      vtkSmartPointer<vtkTriangleFilter>::New();
  triangleFilter->SetInput(result);
  triangleFilter->Update();
  
  result->ShallowCopy(triangleFilter->GetOutput());
  
}


void vtkPTXData::GenerateTriangulatedPolyDataSpanGaps(vtkPolyData* result)
{
  this->GeneratePointPolyData(result);
  
  // this function triangulates the points in 2D then applies the resulting 
  // topology to the 3D geometry
  vtkSmartPointer<vtkPoints> points = 
      vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points2D = 
      vtkSmartPointer<vtkPoints>::New();
  
  //create the 2D Grid
  for ( unsigned int t = 0; t < this->GetNumberOfThetaPoints(); t++ )
    {
    for ( unsigned int p = 0; p < this->GetNumberOfPhiPoints(); p++ )
      {
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
        { 
        continue;
        }
    
      points2D->InsertNextPoint(p, t, 0);
      }
    }
  
  vtkSmartPointer<vtkPolyData> polydata2D = 
      vtkSmartPointer<vtkPolyData>::New();
  polydata2D->SetPoints(points2D);
    
  vtkSmartPointer<vtkDelaunay2D> delaunayFilter = 
      vtkSmartPointer<vtkDelaunay2D>::New();
  delaunayFilter->SetInput(polydata2D);
  delaunayFilter->Update();
  
  //apply the new topology to the 3D points
  result->SetPolys(delaunayFilter->GetOutput()->GetPolys());
  
}
