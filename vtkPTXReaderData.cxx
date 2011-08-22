#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkVertexGlyphFilter.h>

void VTP();

int main (int argc, char *argv[])
{
  if(argc != 2)
    {
    cout << "Required arguments: OutputFilename(ptx)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string outputFilename = argv[1];
  
  vtkSmartPointer<vtkPoints> points = 
      vtkSmartPointer<vtkPoints>::New();
  /*
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 1.0);
  points->InsertNextPoint(0.0, 2.0, 2.0);
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 1.0, 1.0);
  points->InsertNextPoint(1.0, 2.0, 2.0);
  */
  points->InsertNextPoint(0.0, 0.0, 0.0);
  points->InsertNextPoint(0.0, 1.0, 0.0);
  points->InsertNextPoint(0.0, 2.0, 0.0);
  points->InsertNextPoint(1.0, 0.0, 0.0);
  points->InsertNextPoint(1.0, 1.0, 0.0);
  points->InsertNextPoint(1.0, 2.0, 0.0);
  
  std::ofstream fout(outputFilename.c_str());
	
  fout << 3 << endl << 2 << endl
      << "0 0 0" << endl
      << "1 0 0" << endl
      << "0 1 0" << endl
      << "0 0 1" << endl
      << "1 0 0 0" << endl
      << "0 1 0 0" << endl
      << "0 0 1 0" << endl
      << "0 0 0 1" << endl;
  
  for(unsigned int counter = 0; counter < 6; counter++)
    {
    double coord[3];
    points->GetPoint(counter, coord);
    fout << coord[0] << " " << coord[1] << " " << coord[2] << " .4 0 0 0" << endl; //.5 is an invalid point
    }
    
  fout.close();
  
  return EXIT_SUCCESS;
}

void VTP()
{
/*
  vtkSmartPointer<vtkPolyData> polydata = 
      vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  
  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = 
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
  vertexFilter->SetInput(polydata);
  vertexFilter->Update();
  
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  //writer->SetInput(polydata);
  writer->SetInputConnection(vertexFilter->GetOutputPort());
  writer->Write();
  */
}