#include "vtkPTXReader.h"

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

vtkCxxRevisionMacro(vtkPTXReader, "$Revision: 1.70 $");
vtkStandardNewMacro(vtkPTXReader);

int vtkPTXReader::FillOutputPortInformation(
    int vtkNotUsed(port), vtkInformation* info)
{
  // now add our info
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPTXData");
  return 1;
}

int vtkPTXReader::RequestDataObject(
                                        vtkInformation* vtkNotUsed(request),
    vtkInformationVector** vtkNotUsed(inputVector),
                                      vtkInformationVector* outputVector )
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkPTXData* output = vtkPTXData::SafeDownCast(
                                          outInfo->Get( vtkDataObject::DATA_OBJECT() ) );
  if ( ! output )
    {
    output = vtkPTXData::New();
    outInfo->Set( vtkDataObject::DATA_OBJECT(), output );
    output->FastDelete();
    output->SetPipelineInformation( outInfo );
    this->GetOutputPortInformation(0)->Set(
                                    vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType() );
    }
  return 1;
}

vtkPTXData* vtkPTXReader::GetOutput()
{
  return vtkPTXData::SafeDownCast(this->GetOutputDataObject(0));
}
 

vtkPTXReader::vtkPTXReader()
{
  this->FileName = NULL;
  
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);

}

void vtkPTXReader::ReadFile(vtkSmartPointer<vtkPTXData> data)
{
  vtkstd::ifstream infile;
  infile.open(this->FileName);
  if(!infile)
    {
    cout << "Could not open ptx file " << this->FileName << "!" << endl;
    return;
    }

  //read the header
  vtkstd::string line;

  unsigned int numberOfThetaPoints;
  unsigned int numberOfPhiPoints;
  
  getline(infile, line);
  unsigned int phiPoints;
  vtkstd::stringstream(line) >> numberOfThetaPoints;

  getline(infile, line);
  unsigned int thetaPoints;
  vtkstd::stringstream(line) >> numberOfPhiPoints;

  //skip 8 lines (identity matrices)
  for(int i = 0; i < 8; i++)
    {
    getline(infile, line);
    }
  
  cout << "PhiPoints: " << numberOfPhiPoints << endl;
  cout << "ThetaPoints: " << numberOfThetaPoints << endl;
  
  //setup the grids
  data->PointGrid->Resize(numberOfThetaPoints, numberOfPhiPoints);
  data->ValidGrid->Resize(numberOfThetaPoints, numberOfPhiPoints);
  data->ColorGrid->Resize(numberOfThetaPoints, numberOfPhiPoints);
  data->IntensityGrid->Resize(numberOfThetaPoints, numberOfPhiPoints);
  data->IdGrid->Resize(numberOfThetaPoints, numberOfPhiPoints);
  
  unsigned int invalidCounter = 0;
  for(unsigned int counter = 0; counter < numberOfPhiPoints*numberOfThetaPoints; counter++)
    {
    //vtkstd::cout << "counter = " << counter << vtkstd::endl;
    getline(infile, line);
    //std::cout << "line: " << line << std::endl;
    
    vtkVector3d p;
    double intensity;
    double colorInt[3];
    //unsigned char colorChar[3];
    
    std::stringstream ParsedLine(line);
    double coord[3];
    ParsedLine >> coord[0] >> coord[1] >> coord[2] >> intensity >> colorInt[0] >> colorInt[1] >> colorInt[2];
    p.SetX(coord[0]);
    p.SetY(coord[1]);
    p.SetZ(coord[2]);
    
    unsigned int phi = counter % numberOfPhiPoints;
    unsigned int theta = counter / numberOfPhiPoints;
    
    //vtkstd::cout << "theta: " << theta << " phi: " << phi << vtkstd::endl;
    
    //set the point
    data->PointGrid->SetValue(theta, phi, p);
    
    data->IntensityGrid->SetValue(theta, phi, intensity);
    //set the validity
    if(intensity == 0.50) //the Leica HDS3000 scanner marks invalid points as "0 0 0 .5 0 0 0"
      {
      data->ValidGrid->SetValue(theta, phi, false);
      invalidCounter++;
      }
    else
      {
      data->ValidGrid->SetValue(theta, phi, true);
      }
    
    
    //convert color ints to chars
      /*
    for(int i = 0; i < 3; i++)
      {
      colorChar[i] = static_cast<unsigned char>(colorInt[i]);
      }
      
      Color c(colorChar);
      */
      vtkColor3ub c;
      c.SetRed(static_cast<unsigned char>(colorInt[0]));
      c.SetGreen(static_cast<unsigned char>(colorInt[1]));
      c.SetBlue(static_cast<unsigned char>(colorInt[2]));
      
      data->ColorGrid->SetValue(theta, phi, c);

    }//end for

  cout << invalidCounter << " points marked as invalid." << endl;
    
  //close the input file
  infile.close();
  
}

int vtkPTXReader::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  
  // get the ouptut
  vtkPTXData* output = vtkPTXData::SafeDownCast(
		  outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
  ReadFile(output);
  
  return 1;
}

int vtkPTXReader::ProcessRequest(vtkInformation* request,
                                     vtkInformationVector** inputVector,
                                     vtkInformationVector* outputVector)
{
  // Create an output object of the correct type.
  if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA_OBJECT()))
  {
    return this->RequestDataObject(request, inputVector, outputVector);
  }
  // generate the data
  if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
  {
    return this->RequestData(request, inputVector, outputVector);
  }
 
  if(request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
  {
    return this->RequestUpdateExtent(request, inputVector, outputVector);
  }
 
  // execute information
  if(request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
  {
    return this->RequestInformation(request, inputVector, outputVector);
  }
 
  return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

int vtkPTXReader::RequestInformation(
                                         vtkInformation* vtkNotUsed(request),
    vtkInformationVector** vtkNotUsed(inputVector),
                                      vtkInformationVector* vtkNotUsed(outputVector))
{
  // do nothing let subclasses handle it
  return 1;
}

int vtkPTXReader::RequestUpdateExtent(
                                          vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* vtkNotUsed(outputVector))
{
  int numInputPorts = this->GetNumberOfInputPorts();
  for (int i=0; i<numInputPorts; i++)
  {
    int numInputConnections = this->GetNumberOfInputConnections(i);
    for (int j=0; j<numInputConnections; j++)
    {
      vtkInformation* inputInfo = inputVector[i]->GetInformationObject(j);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);
    }
  }
  return 1;
}

//----------------------------------------------------------------------------
void vtkPTXReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "File Name: " 
      << (this->FileName ? this->FileName : "(none)") << endl;
}
