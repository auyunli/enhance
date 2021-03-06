#ifndef FILE_MD5_ANIM_HPP
#define FILE_MD5_ANIM_HPP

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

#include "file_md5_common.hpp"

class file_md5_anim {
public:
    struct hierarchy {
	std::string _name;
	int _parent;
	int _flags;
	int _start_index;
    };
    struct bound {
	float _min[3];
	float _max[3];
    };
    struct baseframe {
	int _index;
	float _pos[3];
	float _orient[3];
    };
    struct frame {
	int _index;
	std::vector<float> _data; //size of num animated components
    };
    class data_anim {
    public:
	int _md5version;
	std::string _commandline;
	int _numframes;
	int _numjoints;
	int _framerate;
	int _num_animated_components;
	std::list<hierarchy> _hierarchies; //expected size = _numjoints
	std::list<bound> _bounds; //expected size = number of _frames = _numframes
	std::list<baseframe> _baseframes; //expected size = _numjoints
	std::vector< std::shared_ptr<frame> > _frames; //expected number of data per frame = _num_animated_components. expected size = _numframes
	~data_anim(){
	    _frames.clear();
	}
    };
    enum class process_type {
	md5version,
	commandline,
        numframes,
	numjoints,
	framerate,
        numanimatedcomponents,
	hierarchy,
	bounds,
        baseframe,
	frame,
    };
    
    static std::pair<bool, data_anim> process( std::string file_path );

private:

    static bool check_consistency( data_anim & );
    static bool process_token( std::pair<file_md5_common::token, std::string> t, std::fstream & f, void * d );
    //keyword processing
    static bool process_md5version( std::fstream &, void * );
    static bool process_commandline( std::fstream &, void * );
    static bool process_numframes( std::fstream &, void * ); 
    static bool process_numjoints( std::fstream &, void * ); 
    static bool process_framerate( std::fstream &, void * ); 
    static bool process_numanimatedcomponents( std::fstream &, void * ); 
    static bool process_hierarchy( std::fstream &, void * ); 
    static bool process_bounds( std::fstream &, void * ); 
    static bool process_baseframe( std::fstream &, void * ); 
    static bool process_frame( std::fstream &, void * ); 
	  
    //helpers
    static std::unordered_map<std::string, file_md5_anim::process_type > _keyword_map;
    static std::map<file_md5_anim::process_type, bool(*)(std::fstream &, void *) > _process_map;
};

#endif
