#ifndef RENDER_PROJECTION_H
#define RENDER_PROJECTION_H

#include "AssetManager.h"
#include "MatrixMath.h"

#include <vector>
#include <cassert>
#include <type_traits>

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

class RenderProjectionData {
public:
    struct Coordinate {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
    struct Lookat {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
    struct Up {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
    struct Perspective {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 4>;
    };
    struct Ambient {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
    struct Diffuse {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
    struct Specular {
	using Type = std::vector<double>;
	using Size = std::integral_constant<int, 3>;
    };
};

class RenderProjection : public AssetManager< RenderProjectionData::Coordinate,    RenderProjectionData::Coordinate::Type >,
			 public AssetManager< RenderProjectionData::Lookat,        RenderProjectionData::Lookat::Type >,
			 public AssetManager< RenderProjectionData::Up,            RenderProjectionData::Up::Type >,
			 public AssetManager< RenderProjectionData::Perspective,   RenderProjectionData::Perspective::Type >,
			 public AssetManager< RenderProjectionData::Ambient,       RenderProjectionData::Ambient::Type >,
			 public AssetManager< RenderProjectionData::Diffuse,       RenderProjectionData::Diffuse::Type >,
			 public AssetManager< RenderProjectionData::Specular,      RenderProjectionData::Specular::Type >
{
public:
    using AssetManager< RenderProjectionData::Coordinate,    RenderProjectionData::Coordinate::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Lookat,        RenderProjectionData::Lookat::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Up,            RenderProjectionData::Up::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Perspective,   RenderProjectionData::Perspective::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Ambient,       RenderProjectionData::Ambient::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Diffuse,       RenderProjectionData::Diffuse::Type >::AddDataSingle;
    using AssetManager< RenderProjectionData::Specular,      RenderProjectionData::Specular::Type >::AddDataSingle;
    
    using AssetManager< RenderProjectionData::Coordinate,    RenderProjectionData::Coordinate::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Lookat,        RenderProjectionData::Lookat::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Up,            RenderProjectionData::Up::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Perspective,   RenderProjectionData::Perspective::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Ambient,       RenderProjectionData::Ambient::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Diffuse,       RenderProjectionData::Diffuse::Type >::GetDataSingle;
    using AssetManager< RenderProjectionData::Specular,      RenderProjectionData::Specular::Type >::GetDataSingle;

    bool CheckIsValid(){
	RenderProjectionData::Coordinate::Type check_coordinate;
	if( !GetDataSingle( RenderProjectionData::Coordinate(), check_coordinate ) || RenderProjectionData::Coordinate::Size::value > check_coordinate.size() ){
	    assert( 0 && "Light Coordinate not valid" );
	}
	RenderProjectionData::Lookat::Type check_lookat;
	if( !GetDataSingle( RenderProjectionData::Lookat(), check_lookat ) || RenderProjectionData::Lookat::Size::value > check_lookat.size() ){
	    assert( 0 && "Light Lookat not valid" );
	}
	RenderProjectionData::Up::Type check_up;
	if( !GetDataSingle( RenderProjectionData::Up(), check_up ) || RenderProjectionData::Up::Size::value > check_up.size() ){
	    assert( 0 && "Light Up not valid" );
	}
        RenderProjectionData::Perspective::Type check_perspective;
	if( !GetDataSingle( RenderProjectionData::Perspective(), check_perspective ) || RenderProjectionData::Perspective::Size::value > check_perspective.size() ){
	    assert( 0 && "Light Perspective not valid" );
	}
	RenderProjectionData::Ambient::Type check_ambient;
	if( !GetDataSingle( RenderProjectionData::Ambient(), check_ambient ) || RenderProjectionData::Ambient::Size::value > check_ambient.size() ){
	    assert( 0 && "Light Ambient not valid" );
	}
	RenderProjectionData::Diffuse::Type check_diffuse;
	if( !GetDataSingle( RenderProjectionData::Diffuse(), check_diffuse ) || RenderProjectionData::Diffuse::Size::value > check_diffuse.size() ){
	    assert( 0 && "Light Diffuse not valid" );
	}
	RenderProjectionData::Specular::Type check_specular;
	if( !GetDataSingle( RenderProjectionData::Specular(), check_specular ) || RenderProjectionData::Specular::Size::value > check_specular.size() ){
	    assert( 0 && "Light Specular not valid" );
	}
	return true;
    }
    bool Compute( RenderProjectionData::Perspective::Type & data_ambient,
		  RenderProjectionData::Diffuse::Type & data_diffuse,
		  RenderProjectionData::Specular::Type & data_specular,
		  RenderProjectionData::Coordinate::Type & data_coordinate,
		  mat4 & projectionMatrix,
		  mat4 & viewMatrix ){
	if( !CheckIsValid() ){
	    return false;
	}

	RenderProjectionData::Perspective::Type data_perspective;
	RenderProjectionData::Up::Type data_up;
	RenderProjectionData::Lookat::Type data_lookat;
	if( !GetDataSingle( RenderProjectionData::Ambient(), data_ambient ) )         return false;
	if( !GetDataSingle( RenderProjectionData::Diffuse(), data_diffuse ) )         return false;
	if( !GetDataSingle( RenderProjectionData::Specular(), data_specular ) )       return false;
	if( !GetDataSingle( RenderProjectionData::Coordinate(), data_coordinate ) )   return false;
	if( !GetDataSingle( RenderProjectionData::Perspective(), data_perspective ) ) return false;
	if( !GetDataSingle( RenderProjectionData::Lookat(), data_lookat ) )           return false;
	if( !GetDataSingle( RenderProjectionData::Up(), data_up ) )                   return false;

        /* projectionMatrix = glm::perspective( data_perspective[0], data_perspective[1], data_perspective[2], data_perspective[3] ); */
	float temp_projection_mat[16];
	MatrixMath::Perspective( data_perspective[0], data_perspective[1], data_perspective[2], data_perspective[3], temp_projection_mat );
	projectionMatrix = make_mat4( temp_projection_mat );

	viewMatrix = glm::lookAt( vec3( data_coordinate[0], data_coordinate[1], data_coordinate[2] ), 
				  vec3( data_lookat[0],     data_lookat[1],     data_lookat[2]     ),
				  vec3( data_up[0],         data_up[1],         data_up[2])        );
	return true;
    }
};

#endif
