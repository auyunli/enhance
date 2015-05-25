//==================================
//
// Summary:      Winged Edge Facilities Implementation
// Notes:
// Dependencies: N/A
//==================================
#include "WingedEdge.h"
#include <map>
#include <vector>
#include <tuple>
#include <utility>

using namespace std;

namespace Winged_Edge {
    bool Get_Edge_CW_Next( WingedEdge * WEdge, WingedEdge * & Next )
    {
	if( !WEdge ){
	    return false;
	}
	Next = WEdge->E_CW_Next;
	if( !Next ){
	    return false;
	}
	return true;
    }
    
    bool Get_Edge_CW_Prev( WingedEdge * WEdge, WingedEdge * & Prev )
    {
	if( !WEdge ){
	    return false;
	}
	Prev = WEdge->E_CW_Prev;
	if( !Prev ){
	    return false;
	}
	return true;
    }

    bool Get_Edge_CCW_Next( WingedEdge * WEdge, WingedEdge * & Next )
    {
	if( !WEdge ){
	    return false;
	}
	Next = WEdge->E_CCW_Next;
	if( !Next ){
	    return false;
	}
	return true;
    }

    bool Get_Edge_CCW_Prev( WingedEdge * WEdge, WingedEdge * & Prev )
    {
	if( !WEdge ){
	    return false;
	}
	Prev = WEdge->E_CCW_Prev;
	if( !Prev ){
	    return false;
	}
	return true;
    }

    bool Get_Face_Left( WingedEdge * WEdge, Face * & FaceLeft )
    {
	if( !WEdge ){
	    return false;
	}
	FaceLeft = WEdge->F_Left;
	if( !FaceLeft ){
	    return false;
	}
	return true;    
    }

    bool Get_Face_Right( WingedEdge * WEdge, Face * & FaceRight )
    {
	if( !WEdge ){
	    return false;
	}
	FaceRight = WEdge->F_Right;
	if( !FaceRight ){
	    return false;
	}
	return true; 
    }

    bool Get_Vertex_Start( WingedEdge * WEdge, Vertex * & VertexStart )
    {
	if( !WEdge ){
	    return false;
	}
	VertexStart = WEdge->V_Start;
	if( !VertexStart ){
	    return false;
	}
	return true; 
    }

    bool Get_Vertex_End( WingedEdge * WEdge, Vertex * & VertexEnd )
    {
	if( !WEdge ){
	    return false;
	}
	VertexEnd = WEdge->V_End;
	if( !VertexEnd ){
	    return false;
	}
	return true; 
    }

    bool Generate_WingedEdge( MapEdge map_edge, MapFace map_face, vector< WingedEdge * > & Generated )
    {
	Generated.clear();
	map< pair< Vertex *, Vertex * >, pair< WingedEdge *, bool > > map_ExistingWEdges;

	for( auto i : map_edge ){
	    //save vertices, edge of the winged edge
	    WingedEdge * current_wedge = new WingedEdge;
	    current_wedge->V_Start = i.second.first;
	    current_wedge->V_End = i.second.second;
	    current_wedge->E_Current = i.first;

	    //link entities to current winged edge
	    current_wedge->V_Start->Neighbour_WEdges.insert( current_wedge );
	    current_wedge->V_End->Neighbour_WEdges.insert( current_wedge );
	    current_wedge->E_Current->WEdge = current_wedge;
   
	    //save edge for later checking and take directionality into account
	    bool bDirPositive = true;
	    map_ExistingWEdges[ i.second ] = std::make_pair( current_wedge, bDirPositive );
       
	    //save the edge, except the other direction
	    auto alt = std::make_pair( i.second.second, i.second.first );
	    map_ExistingWEdges[ alt ] = std::make_pair( current_wedge, !bDirPositive );

	    Generated.push_back( current_wedge );
	}

	//for each face
	for( auto i : map_face ){
	    Face * face = i.first;
	    //get vertices of the face, assuming CCW order
	    Vertex * v1 = std::get<0>( i.second );
	    Vertex * v2 = std::get<1>( i.second );
	    Vertex * v3 = std::get<2>( i.second );
	    bool bClockWise = std::get<3>( i.second );
	    auto e1 = std::make_pair( v1, v2 );
	    auto e2 = std::make_pair( v2, v3 );
	    auto e3 = std::make_pair( v3, v1 );
	    vector< pair< Vertex *, Vertex * > > current_edges;
	    current_edges.push_back( e1 );
	    current_edges.push_back( e2 );
	    current_edges.push_back( e3 );

	    vector< pair< WingedEdge *, bool > > triangle_winged_edges; //save edges of current triangles to be linked

	    //get each edge of current face
	    for( auto j : current_edges ){
		auto k = map_ExistingWEdges.find( j ); //find if vertice pair already exist
		if( k == map_ExistingWEdges.end() ){ //if edge doesn't exist, this function fails
		    for( auto z : Generated ){
			delete z;
		    }  
		    return false;
		}else{
		    //link face to current winged edge depending on winged edge directionality
		    bool bDirection = k->second.second;
		    WingedEdge * winged_edge = k->second.first;
		    if( bDirection ){
			//winged_edge->F_Right = face;
			if( bClockWise ){
			    winged_edge->F_Right = face;
			}else{
			    winged_edge->F_Left = face;
			}
			face->Neighbour_WEdges.insert( winged_edge );
		    }else{
			//winged_edge->F_Left = face;
			for( auto z : Generated ){
			    delete z;
			}
			return false;
		    }
		    triangle_winged_edges.push_back( make_pair( winged_edge, bClockWise ) );
		}
	    }

	    //save neighbour winged edges depending on directionality of current winged edge
	    for( auto j = triangle_winged_edges.begin(); j != triangle_winged_edges.end(); j++ ) {
		WingedEdge * winged_edge = j->first;
		bool bDirClockWise = j->second;
		if( bDirClockWise ){
		    auto next = j+1;
		    if( next == triangle_winged_edges.end() ){
			next = triangle_winged_edges.begin();
		    }
		    auto prev = j-1;
		    if( prev < triangle_winged_edges.begin() ){
			prev = triangle_winged_edges.end()-1;
		    }
		    winged_edge->E_CW_Next = next->first;
		    winged_edge->E_CW_Prev = prev->first;
		}else{
		    auto next = j+1;
		    if( next == triangle_winged_edges.end() ){
			next = triangle_winged_edges.begin();
		    }
		    auto prev = j-1;
		    if( prev < triangle_winged_edges.begin() ){
			prev = triangle_winged_edges.end()-1;
		    }
		    winged_edge->E_CCW_Next = next->first;
		    winged_edge->E_CCW_Prev = prev->first;
		}
	    }   
	}
	return true;
    }

    bool Search_WEdge_To_WEdge( WingedEdge * Start, WingedEdge * End, vector< WingedEdge * > & Path ){
	set< WingedEdge * > Searched;
	Path.clear();
	if( !Start || !End ){
	    return false;
	}
	if( Search_WEdge_To_WEdge_Aux( Start, End, Searched, Path ) ){
	    return true;
	}
	return false;
    }

    bool Search_WEdge_To_WEdge_Aux( WingedEdge * Start, WingedEdge * End, set< WingedEdge * > & Searched, vector< WingedEdge * > & Path ){
	if( !Start ){
	    return false;
	}
	//ignore if this edge was searched before
	if( Searched.end() != Searched.find( Start ) ){
	    return false;
	}
	Searched.insert( Start );
	Path.push_back( Start );
	//evaluate if end is found as a neighbour
	if( Is_WingedEdge_Neighour_WingedEdge( Start, End ) ){
	    Searched.insert( End );
	    Path.push_back( End );
	    return true;
	}

	set< WingedEdge * > neighbours;	
	if( !Get_WingedEdge_Neighour_WingedEdges( Start, neighbours ) ){
	    return false;
	}		             
        //search neighbouring edges
	for( auto i : neighbours ){
	    set< WingedEdge * > Searched_Copy = Searched;
	    vector< WingedEdge * > Path_Copy = Path;	    
	    if( Search_WEdge_To_WEdge_Aux( i, End, Searched_Copy, Path_Copy ) ){
		Searched = Searched_Copy; //save path if successful
		Path = Path_Copy;
		return true;
	    }	
	}
	return false;
    }

    bool Search_Face_To_Face( Face * Start, Face * End, std::vector< Face * > & Path_Faces, std::vector< WingedEdge * > & Path_WEdges ){
	Path_Faces.clear();
	Path_WEdges.clear();

	set< Face * > Searched_Faces;
	set< WingedEdge * > Searched_WEdges;
	WingedEdge * Start_WEdge = 0;
	if( Search_Face_To_Face_Aux( Start, Start_WEdge, End, Searched_Faces, Searched_WEdges, Path_Faces, Path_WEdges ) ){
	    return true;
	}
	return false;
    }
    bool Search_Face_To_Face_Aux( Face * Start_Face, WingedEdge * Start_WEdge, Face * End, std::set< Face * > & Searched_Faces, std::set< WingedEdge * > & Searched_WEdges, std::vector< Face * > & Path_Faces, std::vector< WingedEdge * > & Path_WEdges ){
	if( !Start_Face ){
	    return false;
	}	
	//reject if searched before
	if( Start_WEdge && 
	    Searched_WEdges.end() != Searched_WEdges.find( Start_WEdge ) ){
	    return false;
	}
	if( Searched_Faces.end() != Searched_Faces.find( Start_Face ) ){
	    return false;
	}
	Searched_Faces.insert( Start_Face );
	Path_Faces.push_back( Start_Face );
	if( Start_WEdge ){
	    Searched_WEdges.insert( Start_WEdge );
	    Path_WEdges.push_back( Start_WEdge );
	}       
	//check if end is found as a neighbour
	if( Start_WEdge && Is_WingedEdge_Neighour_Face( Start_WEdge, End ) ){
	    Searched_Faces.insert( End );
	    return true;
	}

	//choose an edge and then a face.
	set< WingedEdge * > WEdges;
	Get_Face_Neighbour_WingedEdges( Start_Face, WEdges );
	for( auto i : WEdges ){
	    set< Face * > faces;
	    Get_WingedEdge_Neighour_Faces( i, faces );
	    for( auto j : faces ){
		set< Face * > Searched_Faces_Copy = Searched_Faces;
		set< WingedEdge * > Searched_WEdges_Copy = Searched_WEdges;
		vector< Face * > Path_Faces_Copy = Path_Faces;
		vector< WingedEdge * > Path_WEdges_Copy = Path_WEdges;
		if( Search_Face_To_Face_Aux( j, i, End, Searched_Faces_Copy, Searched_WEdges_Copy, Path_Faces_Copy, Path_WEdges_Copy ) ){
		    Path_Faces = Path_Faces_Copy;
		    Path_WEdges = Path_WEdges_Copy;
		    Searched_Faces = Searched_Faces_Copy;
		    Searched_WEdges = Searched_WEdges_Copy;
		    return true;
		}
	    }
	}
	return false;
    }    
    bool Is_WingedEdge_Neighour_WingedEdge( WingedEdge * WEdge1, WingedEdge * WEdge2 ){
	if( !WEdge1 || !WEdge2 ){
	    return false;
	}
	if( WEdge2 == WEdge1->E_CW_Next ||
	    WEdge2 == WEdge1->E_CW_Prev ||
	    WEdge2 == WEdge1->E_CCW_Next ||
	    WEdge2 == WEdge1->E_CCW_Prev ){
	    return true;
	}       
	return false;
    }
    bool Is_WingedEdge_Neighour_Face( WingedEdge * WEdge, Face * face ){
	if( !WEdge || !face ){
	    return false;
	}
	if( WEdge->F_Left == face || WEdge->F_Right == face ){
	    return true;
	}       
	return false;
    }
    bool Is_WingedEdge_Neighbour_Vertex( WingedEdge * WEdge, Vertex * vertex ){
	if( !WEdge || !vertex ){
	    return false;
	}
	if( WEdge->V_Start == vertex || WEdge->V_End == vertex ){
	    return true;
	}       
	return false;
    }
    bool Get_WingedEdge_Neighour_Faces( WingedEdge * WEdge, set< Face * > & faces ){
	faces.clear();
	if( !WEdge ){
	    return false;
	}
	Face * f = WEdge->F_Left;
	if( f ){
	    faces.insert( f );
	}
	f = WEdge->F_Right;
	if( f ){
	    faces.insert( f );
	}
	return true;
    }
    bool Get_WingedEdge_Neighour_Vertices( WingedEdge * WEdge, set< Vertex * > & vertices ){	
	vertices.clear();
	if( !WEdge ){
	    return false;
	}
	Vertex * v = WEdge->V_Start;
	if( v ){
	    vertices.insert( v );
	}
	v = WEdge->V_End;
	if( v ){
	    vertices.insert( v );
	}
	return true;
    }
    bool Get_WingedEdge_Neighour_WingedEdges( WingedEdge * WEdge, set< WingedEdge * > & wedges ){
	wedges.clear();
	if( !WEdge ){
	    return false;
	}
	WingedEdge * CW_Next = WEdge->E_CW_Next;
	WingedEdge * CW_Prev = WEdge->E_CW_Prev;
	WingedEdge * CCW_Next = WEdge->E_CCW_Next;
	WingedEdge * CCW_Prev = WEdge->E_CCW_Prev;
	wedges.insert( CW_Next );
	wedges.insert( CW_Prev );
	wedges.insert( CCW_Next );
	wedges.insert( CCW_Prev );
	//remove null pointer if previously inserted
	wedges.erase( 0 );
	return true;
    }
    bool Get_Face_Neighbour_WingedEdges( Face * face, set< WingedEdge * > & WEdges ){
	WEdges.clear();
	if( !face ){
	    return false;
	}
	WEdges = face->Neighbour_WEdges;
	return true;
    }
    bool Get_Vertex_Neighbour_WingedEdges( Vertex * vertex, set< WingedEdge * > & WEdges ){
	WEdges.clear();
	if( !vertex ){
	    return false;
	}
	WEdges = vertex->Neighbour_WEdges;
	return true;
    }
    
}