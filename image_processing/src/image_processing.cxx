// boost python related
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/exception_translator.hpp>
#include <numpy/arrayobject.h>
#include <numpy/noprefix.h>

// vigra numpy array converters
#include <vigra/numpy_array.hxx>
#include <vigra/numpy_array_converters.hxx>

// vigra
//#include <vigra/multi_array.hxx>
//#include <vigra/impex.hxx>
//#include <vigra/accumulator.hxx>
//#include <vigra/multi_blocking.hxx>
//#include <vigra/for_each_coord.hxx>

// standard c++ headers (whatever you need (string and vector are just examples))
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <utility>

// my headers  ( in my_module/include )
#include "image_processing.hxx"


// this functions show how to loop over an 
// entire image and do something with each pixel
// and store the result in a new image

vigra::NumpyArray<2,float>  imageLoop(

    vigra::NumpyArray<2,float> inputImage
){

    // allocate output image (also a float image in this case)
    typedef vigra::NumpyArray<2,float> ArrayType;
    typedef ArrayType::difference_type ArrayShape;
    size_t dimX=inputImage.shape(0);
    size_t dimY=inputImage.shape(1);

    ArrayType resultArray(ArrayShape(dimX,dimY));

    for(size_t y=0;y<dimY;++y){
        for(size_t x=0;x<dimX;++x){
            
            // do *something* a pixel  and store in in the result array
            resultArray(x,y)=2*inputImage(x,y)+1;
        }   
    }

    std::cout << "This is C++ speaking: \n\n";
    std::cout << "function 'imageLoop()' is done! 'resultArray(0,0)' is \n";
    std::cout << resultArray(0,0) << '\n';
    std::cout << "\nC++ end.\n\n";

    return resultArray;
}

int  testo(

    int a
){
    return a+3;
}

int testFunction(int a) {

	return a+2;
}

typedef std::pair<int, int> Coord;
//    typedef int[2] Coord;
typedef std::pair<int, int> EdgeID;
typedef std::set< Coord > EdgeCoords;
typedef std::map< EdgeID, EdgeCoords > Edges;
typedef std::list< Coord > CoordList;
//CoordList coordList;

bool appendCoord( const Coord & start, CoordList & coordList, const bool front, const EdgeID & edgeID, EdgeCoords & edgeCoords ) {

//	std::cout << "start[0] = " << start.first << '\n';
	// Horizontal edge
	if ( start.first%2 == 0 ) {
//		std::cout << "if\n";
		const Coord offsets[6] = {
				Coord(-1, -1),
				Coord( 1, -1),
				Coord(-2,  0),
				Coord( 2,  0),
				Coord(-1,  1),
				Coord( 1,  1)
		};

//		std::cout << "Loop over coord offsets...\n";
		for ( size_t i = 0; i < 6; i++ ) {

			Coord addOffset(start.first + offsets[i].first, start.second + offsets[i].second);
//			std::cout << "addOffset = (" << addOffset.first << ", " << addOffset.second << ")" << '\n';

			EdgeCoords::iterator fRes = edgeCoords.find(addOffset);
//			std::cout << "fRes = (" << fRes->first << ", " << fRes->second << ")" << '\n';

			if ( fRes != edgeCoords.end() ) {
				Coord ecoord = *fRes;
				if (front) {
					coordList.push_front(ecoord);
				} else {
					coordList.push_back(ecoord);
				}
				edgeCoords.erase(fRes);
				return true;
			}
		}

	} else {
//		std::cout << "else\n";
		const Coord offsets[6] = {
				Coord(-1, -1),
				Coord( 0, -2),
				Coord( 1, -1),
				Coord(-1,  1),
				Coord( 1,  1),
				Coord( 0,  2)
		};

//		std::cout << "Loop over coord offsets...\n";
		for ( size_t i = 0; i < 6; i++ ) {

			Coord addOffset(start.first + offsets[i].first, start.second + offsets[i].second);
//			std::cout << "addOffset = (" << addOffset.first << ", " << addOffset.second << ")" << '\n';

			EdgeCoords::iterator fRes = edgeCoords.find(addOffset);
//			std::cout << "fRes = (" << fRes->first << ", " << fRes->second << ")" << '\n';

			if ( fRes != edgeCoords.end() ) {
				Coord ecoord = *fRes;
				if (front) {
					coordList.push_front(ecoord);
				} else {
					coordList.push_back(ecoord);
				}
				edgeCoords.erase(fRes);
				return true;
			}
		}

	}

	return false;

};

std::vector< CoordList > lines_;
std::map< EdgeID, std::vector<UInt64> > edgeToLines_;

boost::python::list findEdgePaths( vigra::NumpyArray<2, float> inVolume ) {

//    std::cout << "\nThis is C++ speaking: \n\n";

    boost::python::list result;
    boost::python::list pyEdges;
    boost::python::list pyEdgesCoords;

	Edges edges;

//	// Add item:
//	EdgeID edgeID = EdgeID(1, 2);
////	std::list< std::pair<int, int> > edgeCoords;
////	edgeCoords.push_back( Coord(10, 100) );
////	e.insert(edgeCoords);
////	e.insert( std::pair<int, int>(1, 2), edgeCoords );
//	edges[edgeID].push_back(Coord(10, 100));
//	edges[edgeID].push_back(Coord(11, 100));
//	edges[edgeID].push_back(Coord(11, 101));

	// Fill the coord map
	for ( int y = 0; y < inVolume.shape(1); y++ ) {
		for ( int x = 0; x < inVolume.shape(0); x++ ) {

//			float lu = inVolume(x, y);
//			int coordU[2] = { x, y };
//			Coord coordU = Coord(x, y);
//			Coord coordV = coordU;

//			std::cout << "inVolume(" << x << ", " << y << ") = " << lu << '\n';

			for ( size_t d = 0; d < 2; d++ ) {

//				if ( d == 0 ) coordV.first += 1; else coordV.second += 1;
				int coordV[2] = { x, y };
				coordV[d]++;
				if ( coordV[d] < inVolume.shape(d) ) {

//					std::cout << "coordV[d] = " << coordV[d] << '\n';
//					std::cout << "inVolume.shape(d) = " << inVolume.shape(d) << '\n';
					float lu = inVolume(x, y);
					float lv = inVolume( coordV[0], coordV[1] );
//					std::cout << "lu = " << lu << '\n';
//					std::cout << "lv = " << lv << '\n';
					if ( lu != lv ) {
						if ( lu > lv ) { const float t = lu; lu = lv; lv = t; };
//						std::cout << "lu/lv = " << lu << "/" << lv << '\n';
//						std::cout << "(x, y)                 = (" << x << ", " << y << ")" << '\n';
//						std::cout << "(coordV[0], coordV[1]) = (" << coordV[0] << ", " << coordV[1] << ")" << '\n';
//						edges[EdgeID(lu, lv)].insert(Coord(x, y));
//						edges[EdgeID(lu, lv)].insert(Coord(coordV[0], coordV[1]));
						edges[EdgeID(lu, lv)].insert(Coord(x + coordV[0], y + coordV[1]));
					}

				}

			}

		}

	}

	for (Edges::iterator it = edges.begin(); it!= edges.end(); it++) {

		EdgeCoords edgeCoords = (it->second);
		EdgeID edgeID = it->first;
//		std::cout << "First: " << it->first << "; Second: " << *(edgeCoords.begin()) << '\n';

//		CoordList coordList;
//		CoordList sorted;

		while ( !edgeCoords.empty() ) {

			EdgeCoords::iterator startEdgeIter = edgeCoords.begin();
			Coord startEdge = *startEdgeIter;
			edgeCoords.erase(startEdgeIter);

			CoordList coordList;
			coordList.push_back(startEdge);

//			int safety = 0;
			while ( true ) {
//				safety++;
//				if ( safety > 100 ) {
//					std::cout << "Warning: Safety exit after 100 iterations!\n";
//					break;
//				}

				bool toFront = appendCoord( coordList.front(), coordList, true, edgeID, edgeCoords );
				bool toBack = appendCoord( coordList.back(), coordList, false, edgeID, edgeCoords );
				if ( !(toFront || toBack) ){
					break;
				}

			}

//			std::cout << "lines_.size() = " << lines_.size() << '\n';
			boost::python::list pyEdgeID;
			pyEdgeID.append(edgeID.first);
			pyEdgeID.append(edgeID.second);
			pyEdges.append(pyEdgeID);
			boost::python::list pyEdgeCoords;
			boost::python::list pyEdgeXs;
			boost::python::list pyEdgeYs;
			for ( CoordList::iterator it = coordList.begin(); it != coordList.end(); it++ ) {
//				boost::python::list pyCoord;
//				pyCoord.append( (float)it->first / 2 );
//				pyCoord.append( (float)it->second / 2 );
//				pyEdgeCoords.append(pyCoord);
				pyEdgeXs.append( (float)it->first / 2 );
				pyEdgeYs.append( (float)it->second / 2 );
			}
//			pyEdgesCoords.append(pyEdgeCoords);
			pyEdgeCoords.append(pyEdgeXs);
			pyEdgeCoords.append(pyEdgeYs);
			pyEdgesCoords.append(pyEdgeCoords);

			edgeToLines_[edgeID].push_back(lines_.size());
			for ( CoordList::iterator it = coordList.begin(); it != coordList.end(); it++ ) {
//				std::cout << "coordListElement = (" << (float)it->first/2 << ", " << (float)it->second/2 << ")\n";
			}
			lines_.push_back(coordList);

		}

	}

//    std::cout << "function 'findEdgePaths()' is done!\n";
//    std::cout << "\nC++ end.\n\n";
    result.append(pyEdges);
    result.append(pyEdgesCoords);
	return result;

}



void export_image_processing(){
    // Do not change next 4 lines
    import_array(); 
    vigra::import_vigranumpy();
    boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
    boost::python::docstring_options docstringOptions(true,true,false);
    // No not change 4 line above


    // export the function to python
    boost::python::def("imageLoop", vigra::registerConverters(&imageLoop) ,
        (
            boost::python::arg("image")
        ),
        "loop over an image and do something with each pixels\n\n"
        "Args:\n\n"
        "   image : input image\n\n"
        "returns an an image with the same shape as the input image"
    );

    boost::python::def("testFunction", testFunction);

    boost::python::def("findEdgePaths", findEdgePaths,
    		(
    				boost::python::arg("inVolume")
    		),
    		"Find all edge paths in a label image"
    	);
}
