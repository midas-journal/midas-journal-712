#include "vtkPTXReaderPlugin.h"
#include "vtkPTXReader.h"
#include "vtkPTXData.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkCxxRevisionMacro(vtkPTXReaderPlugin, "$Revision: 1.70 $");
vtkStandardNewMacro(vtkPTXReaderPlugin);

vtkPTXReaderPlugin::vtkPTXReaderPlugin()
{
  //this is a reader
  this->SetNumberOfInputPorts(0);
  
  this->FileName = NULL;
}

int vtkPTXReaderPlugin::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the output object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  
  vtkPolyData* output = vtkPolyData::SafeDownCast(
		  outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
  vtkSmartPointer<vtkPTXReader> reader = 
      vtkSmartPointer<vtkPTXReader>::New();
  reader->SetFileName(this->FileName);
  reader->Update();
  
  vtkSmartPointer<vtkPTXData> data = reader->GetOutput();
  
  vtkSmartPointer<vtkPolyData> polydata = 
      vtkSmartPointer<vtkPolyData>::New();
  
  if(!this->Triangulate)
    {
    data->GeneratePointPolyData(polydata);
    }
  if(this->Triangulate && !this->SpanGaps)
    {
    data->GenerateTriangulatedPolyData(polydata);
    }
  if(this->Triangulate && this->SpanGaps)
    {
    data->GenerateTriangulatedPolyDataSpanGaps(polydata);
    }
  
  output->ShallowCopy(polydata);
  
  return 1;
}

//----------------------------------------------------------------------------
void vtkPTXReaderPlugin::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "File Name: " 
      << (this->FileName ? this->FileName : "(none)") << endl;
}
