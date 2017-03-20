#include <list>
#include <vector>
#include <utility>
#include <cassert>

#include "file_md5_skel.hpp"
#include "Quat.hpp"

std::pair<bool, file_md5_skel::skel_collection> file_md5_skel::process( file_md5_anim::data_anim const & d ){
    skel_collection sc;
    if( d._frames.size() != d._bounds.size() ){
	assert( false && "number of frames and number of bounding box not equal" );
	return { false, {} };
    }
    auto it_frames = d._frames.begin();
    auto it_bounds = d._bounds.begin();
    
    while( it_frames != d._frames.end() ){
	std::pair<bool,skel_frame> ret = process_skel_frame( *it_frames, *it_bounds, d._hierarchies, d._baseframes );
	if( false == ret.first ){
	    assert( false && "process_skel_frame failed." );
	    return { false,{} };
	}
	sc._skels.push_back( std::move(ret.second) );
	++it_frames;
	++it_bounds;
    }
    sc._framerate = d._framerate;
    return std::pair<bool,skel_collection>(true,std::move(sc));
}

std::pair<bool,file_md5_skel::skel_frame> file_md5_skel::process_skel_frame( file_md5_anim::frame const & f, file_md5_anim::bound const & bbox, std::list<file_md5_anim::hierarchy> const & hier, std::list<file_md5_anim::baseframe> const & base ){
    if( hier.size() != base.size() ){
	assert( false && "hierarchy and baseframe size not equal" );
	return { false, {} };
    }
    skel_frame sf;
    auto it_hier = hier.begin();
    auto it_base = base.begin();
    while( it_hier != hier.end() ){
	int frame_data_start_index = it_hier->_start_index;
	int offset = 0;
	int flag = it_hier->_flags;
	//obtain rotation and position from base frame
	float sf_pos[3];
	float sf_rot[3];
	for( int i = 0; i < 3; ++i ){
	    sf_pos[i] = it_base->_pos[i];
	    sf_rot[i] = it_base->_orient[i];
	}

	//update rotation and position from frame data if neccessary
	if( flag & 1 ) //pos x
	    sf_pos[0] = f._data[ frame_data_start_index + offset++ ];
	if( flag & 2 ) //pos y
	    sf_pos[1] = f._data[ frame_data_start_index + offset++ ];
	if( flag & 4 ) //pos z
	    sf_pos[2] = f._data[ frame_data_start_index + offset++ ];
	if( flag & 8 ) //rot x
	    sf_rot[0] = f._data[ frame_data_start_index + offset++ ];
	if( flag & 16 ) //rot y
	    sf_rot[1] = f._data[ frame_data_start_index + offset++ ];
	if( flag & 32 ) //rot z
	    sf_rot[2] = f._data[ frame_data_start_index + offset++ ];
	
	//compute rotation quaternion
	Quat sf_orient( sf_rot[0], sf_rot[1], sf_rot[2] );

	int parent_joint_index = it_hier->_parent;

	joint_frame jf;
	jf._parent = parent_joint_index;
	jf._name = it_hier->_name;

	if( parent_joint_index >= 0 ){
	    if( parent_joint_index >= sf._joints.size() ){
		assert( false && "parent joint index out of range." );
		return { false, {} };
	    }
	    joint_frame & parent_joint_frame = sf._joints[ parent_joint_index ];
	    //chain transformation from parent joint

	    //update positions
	    float pos_rotated[3];
	    parent_joint_frame._orient.RotatePoint( sf_pos, pos_rotated );
	    for( int i = 0; i < 3; ++i ){
		jf._pos[i] = parent_joint_frame._pos[i] + pos_rotated[i];
	    }

	    //update orientation
	    jf._orient = parent_joint_frame._orient * jf._orient;
	    jf._orient.NormalizeQuatCurrent();
	}
	else{
	    //no parent
	    for( int i = 0; i < 3; ++i ){
		jf._pos[i] = sf_pos[i];
	    }
	    jf._orient = sf_orient;
	    jf._orient.NormalizeQuatCurrent();
	}
	sf._joints.push_back( std::move(jf) );
	++it_hier;
	++it_base;
    }
    return { true, std::move(sf) };
}