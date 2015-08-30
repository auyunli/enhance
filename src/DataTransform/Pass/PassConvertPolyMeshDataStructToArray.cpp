#include <stdio.h>
#include <string>
#include <iostream>

#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "PolyMesh_Data_Arrays.h"

using namespace std;

bool PassConvertPolyMeshDataStructToArray::ExecutePass( void * & data_in, void * & data_out ){
    bool bRet = true;
    DataType::Enum data_type;
    string data_path;
    DataTransformMetaInfo * meta_info;
    if( !GetDataTransformMetaInfo( meta_info ) ){
	return false;
    }
    //check input data type
    if( !QueryFromMetaInfo( meta_info, DataQuery::INPUT, data_type, data_path ) ){
	return false;
    }
    if( DataType::DATASTRUCT_POLYMESH != data_type ){
	return false;
    }
    //check output data type
    if( !QueryFromMetaInfo( meta_info, DataQuery::OUTPUT, data_type, data_path ) ){
	return false;
    }
    if( DataType::DATASTRUCT_POLYMESH_ARRAY != data_type ){
	return false;
    }

    Filter_ParsePolyMesh * filter_polymesh = ( Filter_ParsePolyMesh * ) data_in;

    //get number of vertices
    unsigned int iNumVertexSet = filter_polymesh->_vec_PolyMesh_Data_Vert.size();
    unsigned int iNumNormalSet = filter_polymesh->_vec_PolyMesh_Data_Normal.size();

    unsigned int iNumVertexData = iNumVertexSet * 3;
    unsigned int iNumNormalData = iNumNormalSet * 3;

    float * vertex_data = new float [ iNumVertexData ];
    float * normal_data = new float [ iNumNormalData ];

    float * vertex_current = vertex_data;
    float * normal_current = normal_data;
    
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Vert ){
	double * data;
	int iCount;
    	bRet = i->GetArray( data, iCount );
	double * data_start = data;
	if( !bRet || iCount < 3 ){ //assume 3 counts per vertex
	    delete [] vertex_data;
	    delete [] data_start;
	    return false;
	}
	for( int j = 0; j < 3; j++ ){
	    *vertex_current++ = *data++;
	}
	delete [] data_start;
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Normal ){
	double * data;
	int iCount;
    	bRet = i->GetArray( data, iCount );
	double * data_start = data;
	if( !bRet || iCount < 3 ){ //assume 3 counts per normal
	    delete [] normal_data;
	    delete [] data_start;
	    return false;
	}
	for( int j = 0; j < 3; j++ ){
	    *normal_current++ = *data++;
	}
	delete [] data_start;
    }

    _ArrayData = new PolyMesh_Data_Arrays;
    bRet = _ArrayData->Set( PolyMesh_Data_Arrays_Type::VERTEX, vertex_data, iNumVertexData );
    bRet &= _ArrayData->Set( PolyMesh_Data_Arrays_Type::NORMAL, normal_data, iNumNormalData );

    delete [] vertex_data;
    delete [] normal_data;
    vertex_data = nullptr;
    normal_data = nullptr;
    data_out = ( void * ) _ArrayData;
    
    cout << "PassConvertPolyMeshDataStructToArray::ExecutePass returned" << endl;
    return bRet;
}
bool PassConvertPolyMeshDataStructToArray::CleanPass(){
    if( !_ArrayData ){
	delete _ArrayData;
	_ArrayData = nullptr;
    }
    cout << "PassConvertPolyMeshDataStructToArray::CleanPass returned" << endl;
    return true;
}