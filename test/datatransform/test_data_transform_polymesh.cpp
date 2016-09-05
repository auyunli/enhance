#include "DataTransformDriver.h"
#include "PassParsePolyMesh.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "Filter_ParsePolyMesh.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main( int argc, char ** argv ){
    if( argc < 2 ){
	assert( 0 && "Require argument: path_to_polymesh_file" );
	return -1;
    }
    char * file_path_polymesh = argv[1];
    
    PassParsePolyMesh pass_parse_polymesh;
    PassParsePolyMesh * p_pass_parse_polymesh = & pass_parse_polymesh;
    DataTransformDriver data_transform_driver;
    DataTransformMetaInfo meta_info("sample_polymesh");
    
    string strFilePathPolyMesh = string( file_path_polymesh );
    meta_info.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
    meta_info.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
    meta_info.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");

    if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info ) ){
	assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
	return -1;
    }
    if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
	assert( 0 && "DataTransformDriver::RegisterPass failed.");
	return -1;
    }
    void * data_in;
    void * data_out;
    if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
	assert( 0 && "DataTransformDriver::ExecutePasses failed.");
	return -1;
    }

    Filter_ParsePolyMesh * filter_polymesh = ( Filter_ParsePolyMesh * ) data_out;

    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Vert ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Normal ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Edge ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_Face ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_TexSrc ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_TexCoord ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_BufferInfo ){
	i->PrintData();
    }
    for( auto i : filter_polymesh->_vec_PolyMesh_Data_BufferInfoSequence ){
    	i->PrintData();
    }

    delete filter_polymesh;
    filter_polymesh = nullptr;
    
    return 0;
}