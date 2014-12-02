#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h>
#include <GL/glut.h>

class GLTexture {
public:
    enum eTextureType {
        DEPTH,
        RGBA,
    };
    GLTexture();
    bool BindFbo();
    bool UnbindFbo();
    bool GetTextureId( GLuint & TextureId );
    bool GetFboId( GLuint & FboId );
    bool SetTexture( eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
private:
    bool SetTextureRgba( int iWidth, int iHeight, void * const Data, int iActiveTexture );
    bool SetTextureDepth( int iWidth, int iHeight, int iActiveTexture );
    GLuint _Tex;
    GLuint _Fbo;
};

#endif
