//
//  bordertrackingincludes.h
//  bordertracking
//
//  Created by Rigaud Stephane on 06/06/16.
//
//

#ifndef bordertrackingincludes_h
#define bordertrackingincludes_h

//
// ITK includes
//

#include <itkConnectedComponentImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkLabelGeometryImageFilter.h>
#include <itkPermuteAxesImageFilter.h>

//
// STD includes
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

//
// Custom includes
//

#include "tools.h"
#include "border.h"
#include "pixel.h"
#include "cell.h"

//
// Typedef
//

typedef unsigned int                    PixelType;  // pixel type
typedef itk::Image<PixelType, 2>        ImageType;  // ITK image type
typedef itk::ImageFileReader<ImageType> ReaderType; // Reader Filter

typedef itk::ConnectedComponentImageFilter<ImageType, ImageType>                  labelFilterType;        // Label Filter
typedef itk::LabelGeometryImageFilter<ImageType, ImageType>                       GeometryFilterType;     // Geometry Filter
typedef itk::BinaryBallStructuringElement<ImageType::PixelType, 2>                StructuringElementType; // structuring element
typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType> BinaryErodeFilterType;  // erosion filter
typedef itk::PermuteAxesImageFilter<ImageType> TransposeImageFilterType;

typedef std::map<int, std::vector<std::string> > CellTrackingMapType; // map to store the cell tracking
typedef std::map<unsigned int, Cell>             CellMapType;         // map to store the cell
typedef std::pair<unsigned int, unsigned int>    MapKeyType;          // key type

typedef itk::ImageRegionConstIterator<ImageType>  ConstIteratorType;             // Image Iterators
typedef itk::ConstNeighborhoodIterator<ImageType> ConstNeighborhoodIteratorType; // Const Image Iterators
typedef CellTrackingMapType::iterator   CellTrackingMapIteratorType;
typedef CellMapType::iterator           CellMapIteratorType;

//
// Version
//

const static std::string version = "BorderTracking ITK4 v2";




#endif /* bordertrackingincludes_h */
