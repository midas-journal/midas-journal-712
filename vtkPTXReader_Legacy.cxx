
#if 0
void vtkPTXReader::GenerateTriangulatedPolyData(vtkPolyData* result)
{
    
  //create a rectangular grid of 2D points
  vtkSmartPointer<vtkPoints> points2D = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points3D = vtkSmartPointer<vtkPoints>::New();
  
  for ( unsigned int t = 0; t < this->NumberOfThetaPoints; t++ )
{
    for ( unsigned int p = 0; p < this->NumberOfPhiPoints; p++ )
{
      bool valid = this->ValidGrid->GetValue(t,p);
      if(!valid)
{ 
        continue;
}
        
      Point point = this->PointGrid->GetValue(t,p);
      points2D->InsertNextPoint(t,p, 0.0); //make the columns parallel with the y axis
      points3D->InsertNextPoint(point.x, point.y, point.z);

      
} //end p for
}//end t for
    
  vtkstd::cout << "2d points: " << points2D->GetNumberOfPoints() << vtkstd::endl;
  vtkstd::cout << "3d points: " << points3D->GetNumberOfPoints() << vtkstd::endl;
  vtkstd::cout << "Should be: " << this->NumberOfPhiPoints * this->NumberOfThetaPoints << vtkstd::endl;
  
  //add the grid points to a polydata object
  vtkSmartPointer<vtkPolyData> polydata2D = vtkSmartPointer<vtkPolyData>::New();
  polydata2D->SetPoints(points2D);
	
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetInput(polydata2D);
  writer->SetFileName("2D.vtp");
  writer->Write();
  
  vtkSmartPointer<vtkPolyData> polydata3D = vtkSmartPointer<vtkPolyData>::New();
  polydata3D->SetPoints(points3D);
  writer->SetInput(polydata3D);
  writer->SetFileName("3D.vtp");
  writer->Write();
  
  //triangulate the grid points
  vtkSmartPointer<vtkDelaunay2D> delaunayFilter = vtkSmartPointer<vtkDelaunay2D>::New();
  delaunayFilter->SetInput(polydata2D);
  delaunayFilter->Update();
  
  vtkPolyData* triangulated2D = delaunayFilter->GetOutput();
  
  writer->SetInput(triangulated2D);
  writer->SetFileName("2DTriangle.vtp");
  writer->Write();

  vtkPolyData* triangulated3D = delaunayFilter->GetOutput();
  triangulated3D->SetPoints(points3D);
  
  //triangulated3D->Update();
  writer->SetInput(triangulated3D);
  writer->SetFileName("3DTriangle.vtp");
  writer->Write();

  //put the 2d points and their connectivity into a polydata object
  result->ShallowCopy(triangulated3D);
  
  //replace the 2d points with the original 3d points
  
  
  /*
  //throw away triangles that violate the size restriction
  
  //get the resulting triangles from the triangulation
  vtkSmartPointer<vtkCellArray> cells = result->GetPolys();

	
  cout << "NumTris in Delaunay: " << result->GetNumberOfPolys() << endl; //the result is not stored as Polys, so don't use this!
  cout << "NumTris in Delaunay: " << result->GetNumberOfCells() << endl; 
	
  vtkIdType npts;
  vtkIdType * pts;

	//go through all the triangles of the Delaunay Triangulation
  cells->InitTraversal();
  while (cells->GetNextCell(npts,pts))
{
		//get the three points of the triangle
    vgl_point_3d<double> x = Points[pts[0]];
    vgl_point_3d<double> y = Points[pts[1]];
    vgl_point_3d<double> z = Points[pts[2]];
		
		//find the length of all the edges
    vector<double> dists(3);
    dists[0] = (x-y).length();
    dists[1] = (x-z).length();
    dists[2] = (y-z).length();

		//throw away triangles that are bigger than a threshold
		//sort the edge lengths
    sort(dists.begin(),dists.end());
		//if the longest edge is larger than the tolerance, don't include this triangle
    if (dists[2] > MaxSize)
      continue;
		
		//newcells->InsertNextCell(npts,pts);

     	///ADD IT AS A VALID TRIAGLE
    vector<unsigned int> V;
    V.push_back(pts[0]);
    V.push_back(pts[1]);
    V.push_back(pts[2]);
    TriangleVertexList.push_back(V);
}
  //result->SetPolys(newcells);
  */
	

}
#endif